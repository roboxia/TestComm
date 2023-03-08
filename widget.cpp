#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QtMath>

#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>




Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    findPortTimer = new QTimer(this);
    serialport = new QSerialPort(this);

    framTableModel = new QStandardItemModel();
    rxFramTableModel = new QStandardItemModel();
    itemTableModel = new QStandardItemModel();
    rxItemTableModel = new QStandardItemModel();


    //串口相关内容初始化
    findPortTimer->start(100);//周期100ms启动定时器
    connect(this->findPortTimer,SIGNAL(timeout()),
            this,SLOT(findPortTimer_timeout()));//连接定时器溢出信号和槽函数
    connect(serialport,SIGNAL(readyRead()),
            this,SLOT(serialport_readyread()));


    //UI初始化
    //tableviw创建表头
    //根据Fram数据生成ListView

    connect(this->framTableModel,SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this,SLOT(framTableChange(const QModelIndex &, const QModelIndex &)));
    connect(this->rxFramTableModel,SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this,SLOT(rxFramTableChange(const QModelIndex &, const QModelIndex &)));
    connect(this->itemTableModel,SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this,SLOT(itemTableChange(const QModelIndex &, const QModelIndex &)));
    connect(this->rxItemTableModel,SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this,SLOT(rxItemTableChange(const QModelIndex &, const QModelIndex &)));


//   ui->framTable->setColumnWidth(0,100);
//   ui->framTable->setColumnWidth(1,100);
//   ui->framTable->setColumnWidth(2,700);

//   ui->framTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    //打开配置文件
    configFile.open(QCoreApplication::applicationDirPath());//获取程序当前路径

    //读取Fram数据
    configFile.getFram(&allFram);
    configFile.rxGetFram(&rxAllFram);

   //初始设置表格
    initFramToTable();
}
Widget::~Widget()
{
    configFile.framToJson(&allFram);
    configFile.rxFramToJson(&rxAllFram);
    delete ui;
}


// 自动搜索并添加设备的定时器槽函数
void Widget::findPortTimer_timeout()
{
    if(ui->openPort->checkState())  //串口开启状态下不更新列表
        return;
    static int listsize = portlist.size();
    portlist = QSerialPortInfo::availablePorts();
    if(listsize != portlist.size()){//判断是否改变了设备数量
        ui->choicePort->clear(); //清除之前的显示
        for(int i=0;i<portlist.size();i++){//添加更新后的设备信息
            ui->choicePort->addItem(portlist[i].portName()
                            +":"+portlist[i].description());
        }
    }
    listsize = portlist.size(); // 记录设备数量，用以判断设备数量是否改变
}

//开启串口
void Widget::on_openPort_clicked(bool checked)
{
    if( checked == true &&ui->choicePort->currentIndex()>=0){
        QString name =
           portlist[ui->choicePort->currentIndex()].portName();//获取选中的串口的名字
        QString baud = ui->baudNum->currentText();//获取下拉框中设置波特率
        serialport->setPortName(name);//设置串口名字
        serialport->setBaudRate(baud.toInt());//设置串口波特率
        serialport->setDataBits(QSerialPort::Data8);//设置数据位
        serialport->setStopBits(QSerialPort::OneStop);//设置停止位
        serialport->setParity(QSerialPort::NoParity);//设置奇偶校验
        serialport->setFlowControl(QSerialPort::NoFlowControl);//设置流控制
        if(serialport->open(QIODevice::ReadWrite)){ //以可读可写的方式打开串口
            ui->baudNum->setDisabled(true);
            ui->choicePort->setDisabled(true);
        }
        else{// 串口打开失败
            QMessageBox::warning(this,"提示" ,"连接失败\n串口可能被占用",QMessageBox::Ok);
            ui->openPort->setChecked(false);
            ui->baudNum->setDisabled(false);
            ui->choicePort->setDisabled(false);
        }
    }else if(ui->choicePort->currentIndex()<0){//没有搜索到串口
        ui->openPort->setChecked(false);
        QMessageBox::warning(this,"提示","设备未连接",QMessageBox::Ok);
        ui->baudNum->setDisabled(false);
        ui->choicePort->setDisabled(false);
    }
    else{// 需要关闭串口
        serialport->clear();
        serialport->close();//关闭串口
        ui->baudNum->setDisabled(false);
        ui->choicePort->setDisabled(false);
    }
}

//统计数据长度,赋值到数据长度字段,专用函数
void Widget::autoCalculate(framType *fram)
{
    uint16_t buffsize = 0;
    QByteArray pack;
    QByteArray buff;

    for(int i = 5;i<fram->itemNum-1;i++){
        //根据类型截取pack来调用函数
        if(fram->itemTable[i].type == "u8"){
           buffsize += 1;
        }else if (fram->itemTable[i].type == "u16") {
            buffsize += 2;
        }else if (fram->itemTable[i].type == "u32") {
            buffsize += 4;
        }else if (fram->itemTable[i].type == "float") {
            buffsize += 4;
        }
    }
    char *p = reinterpret_cast<char*>(&buffsize);

    buff.resize(2);
    buff[0] = *p++;
    buff[1] = *p++;
    fram->itemTable[4].data = ByteArrayToitemData(fram->itemTable[4].type,buff);

    for(int i = 2;i<fram->itemNum-1;i++){
        pack.append(itemDataToByteArray(fram->itemTable[i].type,fram->itemTable[i].data));
    }

    uint16_t crc = modbusCRC16(pack);
    p = reinterpret_cast<char*>(&crc);
    buff.resize(2);
    buff[0] = *p++;
    buff[1] = *p++;

    //最后一个是CRC
    fram->itemTable[fram->itemNum-1].data =
            ByteArrayToitemData(fram->itemTable[fram->itemNum-1].type,buff);
//    qDebug()<<crc;
}

//解析数据
QByteArray Widget::readAnalysis(QByteArray newdata)
{
    QByteArray analyBuff;
    QByteArray returnBuff;
    //qDebug()<<newdata;
    recevieBuff += newdata;
    int indexhead = recevieBuff.indexOf(reinterpret_cast<char*>(framHead) ,0);
    if(indexhead < 0)
    {
        //qDebug()<<"indexhead: "<<indexhead;
         return "no";
    }

    //丢掉帧头之前的数据
    recevieBuff.remove(0,indexhead);
    //qDebug()<<"remove"<<recevieBuff;
    //读取长度
    uint16_t dataNum = 0;
    char *p = reinterpret_cast<char*>(&dataNum);
    *p++ = recevieBuff[6];
    *p++ = recevieBuff[7];
    if(dataNum <= 0)
        return "no";
    //qDebug()<<"dataNum"<<dataNum;


    uint16_t framNum = 10 + dataNum;
    //qDebug()<<"framNum"<<framNum;

    //根据长度获取数组
    analyBuff = recevieBuff.left(framNum); //10是固定长度
    if(analyBuff.size() != framNum)
    {
        return "no";
    }
    returnBuff = analyBuff;
    recevieBuff.remove(0,framNum);
    if(analyBuff.at(5) == 0x15)
    {
        QTime time = QTime::currentTime();

        analyBuff.remove(0,9);
        analyBuff.remove(analyBuff.size()-2,2);
        qDebug()<<analyBuff;
        ui->logBrowser->append(time.toString("[ hh:mm:ss:zzz ]") + QString::fromLocal8Bit(analyBuff));

        return "no";
    }
    //解析数据
    framPackToframData(&rxAllFram.framTable[rxAllFram.framCurrent],&analyBuff);
    currentItemValueDisplayRefr(&rxAllFram.framTable[rxAllFram.framCurrent],rxItemTableModel);
    currentFramDisplayRefr(&rxAllFram,rxFramTableModel);
    //qDebug()<<recevieBuff;
    return returnBuff;
}

//串口接收函数
void Widget::serialport_readyread()
{
    //解析之后再显示
    QByteArray data = readAnalysis(serialport->readAll());
    if(data == "no")
        return;
    QTime time = QTime::currentTime();

    QString data_str ="[ " + time.toString("hh:mm:ss:zzz") +" RX "+"] : " +  byteArrayToHexStr(data);
    //qDebug()<<data_str;
    //显示在接收框,时间戳判断
    ui->rxBrowser->append(data_str);
    //1M缓存
}

void Widget::initFramToTable()
{
    framTableModel->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("名称")));
    framTableModel->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("数据")));

    rxFramTableModel->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("名称")));
    rxFramTableModel->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("数据")));

    itemTableModel->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("功能")));
    itemTableModel->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("类型")));
    itemTableModel->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("数据")));
    itemTableModel->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("HEX")));

    rxItemTableModel->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("功能")));
    rxItemTableModel->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("类型")));
    rxItemTableModel->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("数据")));
    rxItemTableModel->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("HEX")));

//    rxItemTableModel->setHeaderData(0,Qt::Vertical, "行0");
//    rxItemTableModel->setHeaderData(1,Qt::Vertical, "行1");
//    rxItemTableModel->setHeaderData(2,Qt::Vertical, "行2");

    //利用setModel()方法将数据模型与QTableView绑定

    ui->framTable->setModel(framTableModel);
    ui->rxFramTable->setModel(rxFramTableModel);
    ui->itemTable->setModel(itemTableModel);
    ui->rxItemTable->setModel(rxItemTableModel);


    for (int i = 0;i < rxAllFram.framNum;i++) {
        if(i >= rxAllFram.framNum)
            return;

        QList<QStandardItem*> col;
        QString data;
        col.append(new QStandardItem(rxAllFram.framTable[i].name));
        framDataToTableView(&rxAllFram.framTable[i],&data);
        col.append(new QStandardItem(data));

        //? inset2,得到的行号就是2?还是2+1?
        rxFramTableModel->appendRow(col);
    }

    for (int i = 0;i < allFram.framNum;i++) {
        if(i >= allFram.framNum)
            return;

        QList<QStandardItem*> col;
        QString data;
        col.append(new QStandardItem(allFram.framTable[i].name));
        framDataToTableView(&allFram.framTable[i],&data);
        col.append(new QStandardItem(data));

        //? inset2,得到的行号就是2?还是2+1?
        framTableModel->appendRow(col);
    }
    //应该需要根据HEX勾选情况进行切换
    showItemToTable(&allFram.framTable[allFram.framCurrent],itemTableModel);
    showItemToTable(&rxAllFram.framTable[rxAllFram.framCurrent],rxItemTableModel);
}


//


//重新显示一个item到itemTable,这里所有的操作入口都是一个fram
void Widget::showItemToTable(framType *fram, QStandardItemModel *model)
{
    model->clear();
    model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("功能")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("类型")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("数据")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("HEX")));

    for (int i = 0;i < fram->itemNum;i++) {
       // appendFramToTable();
        QList<QStandardItem*> col;
        QString data;
        col.append(new QStandardItem(fram->itemTable[i].name));
        col.append(new QStandardItem(fram->itemTable[i].type));
        if(fram->itemTable[i].format == "hex")
        {
            QString hex = itemDataToHexString(fram->itemTable[i].type,
                    fram->itemTable[i].data);
            col.append(new QStandardItem(hex));
        }else {
            col.append(new QStandardItem(fram->itemTable[i].data));
        }

        col.append(new QStandardItem(fram->itemTable[i].format));
        model->appendRow(col);
    }
}

//更新当前Current指向的fram的值
void Widget::currentFramDisplayRefr(allFramType *all, QStandardItemModel *FBmodel)
{
//    QList<QStandardItem*> col;
    QString data;
    framDataToTableView(&all->framTable[all->framCurrent],&data);
//    qDebug()<<data;
//    col.append(new QStandardItem(data));

//    FBmodel->index(all->framCurrent,1);
    FBmodel->item(all->framCurrent,1)->setText(data);
}

//更新当前Current指向的item的值
void Widget::currentItemDisplayRefr(framType *fram, QStandardItemModel *TBmodel)
{
//    QList<QStandardItem*> col;
    QString data;
    if(fram->itemTable[fram->itemCurrent].format == "hex")
    {
        data = itemDataToHexString(fram->itemTable[fram->itemCurrent].type
                ,fram->itemTable[fram->itemCurrent].data);
    }else {
        data = fram->itemTable[fram->itemCurrent].data;
    }
    TBmodel->item(fram->itemCurrent,2)->setText(data);
}

void Widget::currentItemValueDisplayRefr(framType *fram, QStandardItemModel *TBmodel)
{
    QString data;
    for (int i = 0;i < fram->itemNum;i++){

        if(fram->itemTable[i].format == "hex")
        {
            data = itemDataToHexString(fram->itemTable[i].type
                    ,fram->itemTable[i].data);
        }else {
            data = fram->itemTable[i].data;
        }
        if(TBmodel->item(i,2)->text() != data)
        TBmodel->item(i,2)->setText(data);
    }
}

//新增一条Fram,删除一条Fram
//新增到当前行的后面,删除到
void Widget::appendNewFram(allFramType *all,QStandardItemModel *FBmodel)
{
    framType buff;
    itemType item;
    buff.name = "new";

    item.name = "帧头";
    item.type = "u16";
    item.data = "21930";  //0x55AA - > 0xAA 0x55
    item.format = "dec";
    buff.itemTable.append(item);

    item.name = "地址";
    item.type = "u16";
    item.data = "1";  //0001
    item.format = "dec";
    buff.itemTable.append(item);

    item.name = "类型";
    item.type = "u8";
    item.data = "0";  //0001
    item.format = "dec";
    buff.itemTable.append(item);

    item.name = "功能码";
    item.type = "u8";
    item.data = "0";  //0001
    item.format = "dec";
    buff.itemTable.append(item);

    item.name = "数据长度";
    item.type = "u16";
    item.data = "1";  //0001
    item.format = "dec";
    buff.itemTable.append(item);

    item.name = "数据";
    item.type = "u16";
    item.data = "0";  //0001
    item.format = "dec";
    buff.itemTable.append(item);

    item.name = "CRC校验";
    item.type = "u16";
    item.data = "0";  //0001
    item.format = "dec";
    buff.itemTable.append(item);

    buff.itemNum = buff.itemTable.size();

    //? 这里是framnum还是 +?
    all->framTable.append(buff);
    all->framNum++;
    all->framCurrent = all->framNum-1;

    //添加到显示
    QList<QStandardItem*> col;
    QString data;
    col.append(new QStandardItem(buff.name));
    framDataToTableView(&buff,&data);
    col.append(new QStandardItem(data));

    FBmodel->appendRow(col);
}

void Widget::deleteOneFram(allFramType *all,QStandardItemModel *FBmodel)
{
    //至少保留一条数据
    if(all->framNum <= 1)
    {
        return;
    }

    FBmodel->removeRow(all->framCurrent);

    all->framTable.remove(all->framCurrent);
    all->framNum -- ;
    if(all->framCurrent >= all->framNum)
        all->framCurrent = all->framNum-1;

}

//新增一条item,删除一条item
//直接操作CurrentFram
void Widget::appendOneItem(framType *fram,QStandardItemModel *TBmodel)
{
    itemType item;
    int num = fram->itemNum;
    item.name = "name";
    item.type = fram->itemTable[num-1].type;
    item.data = fram->itemTable[num-1].data;
    item.format = fram->itemTable[num-1].format;

    fram->itemTable.append(item);
    fram->itemNum++;
    fram->itemCurrent = fram->itemNum-1;
//    qDebug()<<fram->itemNum;
//    qDebug()<<"q"<<fram->itemCurrent;
    //添加到显示
    QList<QStandardItem*> col;
    QString data;
    col.append(new QStandardItem(item.name));
    col.append(new QStandardItem(item.type));
    col.append(new QStandardItem(item.data));
    col.append(new QStandardItem(item.format));

    TBmodel->appendRow(col);
}


void Widget::insertOneItem(framType *fram,QStandardItemModel *TBmodel)
{
    itemType item;
    int num = fram->itemCurrent;
    item.name = "name";
    item.type = fram->itemTable[num-1].type;
    item.data = fram->itemTable[num-1].data;
    item.format = fram->itemTable[num-1].format;

    fram->itemTable.insert(num+1,item);
    fram->itemNum++;
    fram->itemCurrent ++;
//    qDebug()<<fram->itemNum;
//    qDebug()<<"q"<<fram->itemCurrent;
    //添加到显示
    QList<QStandardItem*> col;
    QString data;
    col.append(new QStandardItem(item.name));
    col.append(new QStandardItem(item.type));
    col.append(new QStandardItem(item.data));
    col.append(new QStandardItem(item.format));

    TBmodel->insertRow(num+1,col);
}

void Widget::deleteOneItem(framType *fram,QStandardItemModel *TBmodel)
{
    if(fram->itemNum <= 1)
    {
        return;
    }

    TBmodel->removeRow(fram->itemCurrent);

    fram->itemTable.remove(fram->itemCurrent);
    fram->itemNum -- ;
    if(fram->itemCurrent >= fram->itemNum)
        fram->itemCurrent = fram->itemNum-1;
//    qDebug()<<fram->itemNum;
//    qDebug()<<"q"<<fram->itemCurrent;
}



//修改一条item,修改一条fram
//修改一条fram,修改不用管显示
void Widget::modifyFramName(const QModelIndex *index,allFramType *all)
{
    all->framTable[ index->row()].name = index->data().toString();
}

//修改一条item 修改不用管显示(先把值放进去再把值读出来?
//我希望可以我直接输入定位值,就修改对应的值
void Widget::modifyItemValue(const QModelIndex *index,framType *fram)
{
    //m : 应该添加输入正确性判断
    switch (index->column()) {
    case 0:
        fram->itemTable[index->row()].name = index->data().toString();
        break;
    case 1:
        fram->itemTable[index->row()].type = index->data().toString();
        break;
    case 2:
        //如果是16进制
        if(fram->itemTable[index->row()].format == "hex")
        {
            fram->itemTable[index->row()].data = HexStringToitemData(
                        fram->itemTable[index->row()].type,index->data().toString());
        }
        else {
            fram->itemTable[index->row()].data = index->data().toString();
        }

        //如果是10进制
//

//        qDebug()<<fram->itemTable[index->row()].type;
//        qDebug()<<fram->itemTable[index->row()].data;
        break;
    case 3:
        fram->itemTable[index->row()].format = index->data().toString();
        break;
    }

//    qDebug()<<index->data().toString();
//    qDebug()<<fram->itemTable[index->row()].data;
//    qDebug()<<index->row();
}





//*****************************数据转换相关函数****************************************//
//显示形态:
// 1. 真实值显示(比如float类型,就显示float的值),
      //ASCII的字符串显示(字符串是真实值的一种,就是添加一个字符串类型)
// 2. HEX的字节形式显示(大端机和小端机不同)
// 总结:有两种显示方式 真实值/HEX显示,字符串是目前未被支持的一种真实值

//输入形态:
//1.输入真实值 -> QString
//2.输入16进制数据->QString -> 取2个字符,转成char型数据,拼接形成指定type的真值 -> 转成QString的真值

//转换函数
//注:数据保存在Item中使用的是 QString 表示,这个QString都保存的10进制数据, 勾选HEX显示之后,显示字节
// 1. 显示真实值(QString): 不用转换
// 2. 显示值HEX(QString显示): 真值QString -> 真值 -> HEX的byte表示的数据 -> HEX的QStrong(每个byte后面加空格)
// 3. 发送用的QByteArray: QString -> 真实数据类型的值 -> HEX的byte表示的数据


//规定
//每个Hex数据后面自动添加空格符号,用户没有空格或者空格过多就报错

//显示真实值 的QString

//用于显示HEX String
QString Widget::itemDataToHexString(QString type,QString data)
{
    QByteArray hexarray = itemDataToByteArray(type,data);

    ////qDebug()<<"size"<<hexarray.size();

    QString hexstring;
    hexstring = hexarray.toHex();
    for(int i = 1; i<hexarray.size()+1;i++){
        ////qDebug()<<3*i-1;

        hexstring.insert(3*i-1,' ');

//        qDebug()<<hexstring;
    }
//    qDebug()<<"hex--"<<hexstring;
    ////qDebug()<<hexstring.size();
    return hexstring;
}


//HEX的QString 转换为 真实值的QString
QString Widget::HexStringToitemData(QString type,QString hex)
{

    QByteArray hexarray;
    QByteArray datastring;
    //去除空格部分
    int size = hex.size();
    for(int i = 1; i<size/3+1;i++){
        hex.remove(2*i,1);
//        qDebug()<<"==="<<hex.size();
//        qDebug()<<hex;
    }
    //这样就切换成大端了,不能这样
//    size = hex.size();
//    for(int i = size/2-1; i >= 0;i--){
//        datastring.append(hex.at(2*i));
//        datastring.append(hex.at(2*i+1));
//    }
//   hexarray = datastring.toUtf8();

    size = hex.size();
    hexarray.resize(size/2);
    for(int i = 0; i < size / 2;i++){
        datastring.append(hex.at(2*i));
        datastring.append(hex.at(2*i+1));
        //qDebug()<<"dddd"<<datastring;
        char buff = static_cast<char>(datastring.toUShort(nullptr,16)&0xff);
        //0A,转换成整形的0x0a
        hexarray[i] = buff;
        datastring.clear();//清除
    }

    //qDebug()<<"---"<<hexarray;

    return ByteArrayToitemData(type,hexarray);
}

//统一使用10进制将数据写入到json

//只用于小端机
//用于发送数据---
QByteArray  Widget::itemDataToByteArray(QString type,QString data)
{
    uint8_t u8Value;
    uint16_t u16Value;
    uint32_t u32Value;
    float floatValue;

    QByteArray array;

    if(type == "u8"){
        u8Value = static_cast<uint8_t>(data.toUShort());

        //qDebug()<< u8Value;

        array.resize(1);

        array[0] = static_cast<char>(u8Value);

        //qDebug()<< array;

    }else if (type == "u16") {
        u16Value = data.toUShort();

        //qDebug()<< u16Value;

        array.resize(2);

        char *p = reinterpret_cast <char*>(&u16Value);
        array[0] = *p++;
        array[1] = *p++;

        //qDebug()<< array;
    }else if (type == "u32") {
        u32Value = data.toUInt();
        //qDebug()<< u32Value;

        array.resize(4);

        char *p = reinterpret_cast <char*>(&u32Value);
        array[0] = *p++;
        array[1] = *p++;
        array[2] = *p++;
        array[3] = *p;

//        qDebug()<< array;

    }else if (type == "float") {
        floatValue = data.toFloat();
//        qDebug()<< floatValue;

        array.resize(4);

        char *p = reinterpret_cast <char*>(&floatValue);
        array[0] = *p++;
        array[1] = *p++;
        array[2] = *p++;
        array[3] = *p;

//        qDebug()<< array;

    }
    return array;
}

//用于接收数据
QString Widget::ByteArrayToitemData(QString type,QByteArray byte)
{
    uint8_t u8Value;
    uint16_t u16Value;
    uint32_t u32Value;
    float floatValue;

    //qDebug()<<type;
    QString string;

    char *p;

    if(type == "u8"){
        u8Value = static_cast<uint8_t>(byte[0]);
        //qDebug()<< "qqq" << u8Value;

        string = QString::number(u8Value);
        //qDebug()<< "qqq" << string;
    }else if (type == "u16") {
        p = reinterpret_cast<char*>(&u16Value);
        *p++ = byte[0];
        *p++ = byte[1];

        //qDebug()<< "qqq" << u16Value;
        string = QString::number(u16Value);
        //qDebug()<< "qqq" << string;
    }else if (type == "u32") {
        p = reinterpret_cast<char*>(&u32Value);
        *p++ = byte[0];
        *p++ = byte[1];
        *p++ = byte[2];
        *p++ = byte[3];
        //qDebug()<< "qqq" << u32Value;
        string = QString::number(u32Value);
        //qDebug()<< "qqq" << string;
    }else if (type == "float") {
        p = reinterpret_cast<char*>(&floatValue);
        *p++ = byte[0];
        *p++ = byte[1];
        *p++ = byte[2];
        *p++ = byte[3];
        //qDebug()<< "qqq" << floatValue;
        string = QString::number(static_cast<double>(floatValue),'f',9);
        //qDebug()<< "qqq" << string;
    }
    return string;
}




////CRC16计算函数
uint16_t Widget::modbusCRC16(QByteArray senddata)
{
    int len=senddata.size();
    uint16_t wcrc=0XFFFF;//预置16位crc寄存器，初值全部为1
    uint8_t temp;//定义中间变量
    int i=0,j=0;//定义计数
    for(i=0;i<len;i++)//循环计算每个数据
    {
       temp=static_cast<uint8_t >(senddata.at(i)) ;
       wcrc^=temp;
       for(j=0;j<8;j++){
          //判断右移出的是不是1，如果是1则与多项式进行异或。
          if(wcrc&0X0001){
              wcrc>>=1;//先将数据右移一位
              wcrc^=0XA001;//与上面的多项式进行异或
          }
          else//如果不是1，则直接移出
              wcrc>>=1;//直接移出
       }
    }
    return wcrc;
}



//将fram数据打包成要 发送的 QByteArray内容
void Widget::framDataToFramPack(framType *fram, QByteArray *pack)
{
       for(int i = 0;i<fram->itemNum;i++){
        pack->append(itemDataToByteArray(fram->itemTable[i].type,fram->itemTable[i].data));
//        qDebug()<<fram->itemTable[i].data;
//        qDebug()<<itemDataToByteArray(fram->itemTable[i].type,fram->itemTable[i].data);
    }
}




//从数据包读取数据到fram格式
void Widget::framPackToframData(framType *fram, QByteArray *pack)
{
    int buffsize = 0;

    for(int i = 0;i<fram->itemNum;i++){
        //根据类型截取pack来调用函数
        if(fram->itemTable[i].type == "u8"){
           buffsize = 1;
        }else if (fram->itemTable[i].type == "u16") {
            buffsize = 2;
        }else if (fram->itemTable[i].type == "u32") {
            buffsize = 4;
        }else if (fram->itemTable[i].type == "float") {
            buffsize = 4;
        }
        QByteArray buff(pack->left(buffsize));
        pack->remove(0,buffsize);
//        qDebug()<<buff;
//        buff.resize(buffsize);

//        buff.append();

        fram->itemTable[i].data = ByteArrayToitemData(fram->itemTable[i].type,buff);

//        qDebug()<<fram->itemTable[i].data;
//        qDebug()<<itemDataToByteArray(fram->itemTable[i].type,fram->itemTable[i].data);
 }
}

//用于将一帧数据转换为ListView显示的Hex QString格式
//listview显示固定的值
void Widget::framDataToTableView(framType *fram,QString *p)
{
    p->clear();
    for(int i = 0; i<fram->itemNum;i++)
    {
        //qDebug()<<fram->itemTable[i].type <<fram->itemTable[i].data;
        p->append(itemDataToHexString(fram->itemTable[i].type,fram->itemTable[i].data));
    }

}


QString Widget::byteArrayToHexStr(const QByteArray &data)
{
    QString temp = "";
    QString hex = data.toHex();

    for (int i = 0; i < hex.length(); i = i + 2) {
        temp += hex.mid(i, 2) + " ";
    }

    return temp.trimmed().toUpper();

}



//信号处理
void Widget::on_framTable_clicked(const QModelIndex &index)
{
    allFram.framCurrent = index.row();
    //同步显示点击的内容
    showItemToTable(&allFram.framTable[index.row()],itemTableModel);
}

void Widget::on_rxFramTable_clicked(const QModelIndex &index)
{
    rxAllFram.framCurrent = index.row();
    //同步显示点击的内容
    showItemToTable(&rxAllFram.framTable[index.row()],rxItemTableModel);
}



void Widget::on_itemTable_clicked(const QModelIndex &index)
{
     allFram.framTable[allFram.framCurrent].itemCurrent = index.row();
     if(index.column() == 3) //3 = hex单元
     {
         if(index.data().toString() == "hex")
         {
             itemTableModel->item(index.row(),index.column())->setText("dec");

         }
         else {
             itemTableModel->item(index.row(),index.column())->setText("hex");

         }
         modifyItemValue(&index,&allFram.framTable[allFram.framCurrent]);
         //更新显示data的值
         currentItemDisplayRefr(&allFram.framTable[allFram.framCurrent],itemTableModel);
     }
}

void Widget::on_rxItemTable_clicked(const QModelIndex &index)
{
    rxAllFram.framTable[rxAllFram.framCurrent].itemCurrent = index.row();

    if(index.column() == 3) //3 = hex单元
    {
        if(index.data().toString() == "hex")
        {
            rxItemTableModel->item(index.row(),index.column())->setText("dec");
        }
        else {
            rxItemTableModel->item(index.row(),index.column())->setText("hex");
        }
        //更新显示data的值
        modifyItemValue(&index,&rxAllFram.framTable[rxAllFram.framCurrent]);
        currentItemDisplayRefr(&rxAllFram.framTable[rxAllFram.framCurrent],rxItemTableModel);
    }
}

//fram数据改变
void Widget::framTableChange(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    if(topLeft.column() == 0)
        modifyFramName(&topLeft,&allFram);
}

//接收fram数据改变
void Widget::rxFramTableChange(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    if(topLeft.column() == 0)
    modifyFramName(&topLeft,&rxAllFram);
}

//item改变
void Widget::itemTableChange(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    modifyItemValue(&topLeft,&allFram.framTable[allFram.framCurrent]);

    //数值改变,计算数据长度
    autoCalculate(&allFram.framTable[allFram.framCurrent]);

    //同步显示
    currentFramDisplayRefr(&allFram,framTableModel);
    currentItemValueDisplayRefr(&allFram.framTable[allFram.framCurrent],itemTableModel);

}

//framitem改变
void Widget::rxItemTableChange(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    //修改
     modifyItemValue(&topLeft,&rxAllFram.framTable[rxAllFram.framCurrent]);
     //同步显示
     currentFramDisplayRefr(&rxAllFram,rxFramTableModel);
     currentItemDisplayRefr(&rxAllFram.framTable[rxAllFram.framCurrent],rxItemTableModel);
}


//添加接收的帧
void Widget::on_pushButton_2_clicked()
{
    appendNewFram(&rxAllFram,rxFramTableModel);
    showItemToTable(&rxAllFram.framTable[rxAllFram.framCurrent],rxItemTableModel);
}

//删除接收帧
void Widget::on_pushButton_clicked()
{
    deleteOneFram(&rxAllFram,rxFramTableModel);
    showItemToTable(&rxAllFram.framTable[rxAllFram.framCurrent],rxItemTableModel);
}
//添加发送的帧
void Widget::on_saveFram_clicked()
{
    appendNewFram(&allFram,framTableModel);
    showItemToTable(&allFram.framTable[allFram.framCurrent],itemTableModel);
}
//删除发送帧
void Widget::on_rmFram_clicked()
{
    deleteOneFram(&allFram,framTableModel);
    showItemToTable(&allFram.framTable[allFram.framCurrent],itemTableModel);
}



//发送按钮
void Widget::on_tx_clicked()
{
    QTime time = QTime::currentTime();
    QByteArray pack;
    framDataToFramPack(&allFram.framTable[allFram.framCurrent],&pack);

    serialport->write(pack,pack.size());

    QString data_str ="[ " + time.toString("hh:mm:ss:zzz") +" TX "+"] : " +  byteArrayToHexStr(pack);

    ui->rxBrowser->append(data_str);
}

void Widget::on_checkBox_2_clicked()
{

}

//发送的16进制显示
void Widget::on_checkBox_2_clicked(bool checked)
{
    framType *fram = &allFram.framTable[allFram.framCurrent];
    //勾选了16进制
    if(checked)
    {
        for(int i = 0 ; i< fram->itemNum;i++)
        {
            itemTableModel->item(i,3)->setText("hex");
            QModelIndex index = itemTableModel->index(i,3);
            modifyItemValue(&index,fram);
        }
    }
    else {
        for(int i = 0 ; i< fram->itemNum;i++)
        {
            itemTableModel->item(i,3)->setText("dec");
            QModelIndex index = itemTableModel->index(i,3);
            modifyItemValue(&index,fram);
        }
    }
    //全部更新显示
    showItemToTable(fram,itemTableModel);
}

void Widget::on_checkBox_clicked(bool checked)
{
    framType *fram = &rxAllFram.framTable[rxAllFram.framCurrent];
    //勾选了16进制
    if(checked)
    {
        for(int i = 0 ; i< fram->itemNum;i++)
        {
            rxItemTableModel->item(i,3)->setText("hex");
            QModelIndex index = rxItemTableModel->index(i,3);
            modifyItemValue(&index,fram);
        }
    }
    else {
        for(int i = 0 ; i< fram->itemNum;i++)
        {
            rxItemTableModel->item(i,3)->setText("dec");
            QModelIndex index = rxItemTableModel->index(i,3);
            modifyItemValue(&index,fram);
        }
    }
    showItemToTable(fram,rxItemTableModel);
}

//发送添加item
void Widget::on_addTx_clicked()
{
    insertOneItem(&allFram.framTable[allFram.framCurrent],itemTableModel);
}

//接收添加item
void Widget::on_addRx_clicked()
{
    insertOneItem(&rxAllFram.framTable[rxAllFram.framCurrent],rxItemTableModel);
}

//发送删除item
void Widget::on_rmTx_clicked()
{
    deleteOneItem(&allFram.framTable[allFram.framCurrent],itemTableModel);
}
//接收添加item
void Widget::on_rmRx_clicked()
{
    deleteOneItem(&rxAllFram.framTable[rxAllFram.framCurrent],rxItemTableModel);
}

void Widget::on_itemTable_activated(const QModelIndex &index)
{

}

void Widget::on_itemTable_pressed(const QModelIndex &index)
{

}
