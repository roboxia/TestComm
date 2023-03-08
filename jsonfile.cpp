#include "jsonfile.h"


jsonFile::jsonFile()
{

}

jsonFile::~jsonFile()
{
    close();
}



bool  jsonFile::open(QString fileDir)
{
    m_fileDir = fileDir;
    QFile file(fileDir+"/config.json");
    if (!file.open(QIODevice::ReadOnly))
    {
        QFile *ff = new QFile(fileDir+"/config.json");
        ff->open(QIODevice::ReadWrite); //如果文件不存在，就自己创建文件
        ff->write("{}");
        ff->close();
        return false;
    }
    QByteArray allData = file.readAll();
    file.close();

    QJsonParseError jsonError;
    jsonDoc = QJsonDocument::fromJson(allData, &jsonError);
    if (jsonError.error != QJsonParseError::NoError)
    {
        return false;

    }
    //打开成功
    docObj = jsonDoc.object();

    m_fileDir = fileDir;
    return true;
}


void jsonFile::close()
{
    QJsonDocument jsonDoc(docObj);
    QByteArray data = jsonDoc.toJson(QJsonDocument::Compact);
    QFile file(m_fileDir+"/config.json");
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(data);
        file.close();
    }
}




//使用 先打包再添加的方式
void jsonFile::framToJson(allFramType *allFram)
{
    QJsonObject all;
    all.insert("framNum",allFram->framNum);
    all.insert("framCurrent",allFram->framCurrent);
    //获取fram数量
    for (int i = 0;i<allFram->framNum;i++) {
        QJsonObject fram;
        fram.insert("name",allFram->framTable[i].name);
        fram.insert("itemCurrent",allFram->framTable[i].itemCurrent);

        fram.insert("itemNum",allFram->framTable[i].itemNum);
        for (int j = 0;j < allFram->framTable[i].itemNum;j++) {
            QJsonObject item;
            item.insert("name",allFram->framTable[i].itemTable[j].name);
            item.insert("type",allFram->framTable[i].itemTable[j].type);
            item.insert("data",allFram->framTable[i].itemTable[j].data);
            item.insert("format",allFram->framTable[i].itemTable[j].format);

            QString itemNo = "item"+QString::number(j);
            fram.insert(itemNo,item);
        }
        QString framNo = "fram"+QString::number(i);
        all.insert(framNo,fram); //添加fram
    }
    docObj.insert("allFram",all);
}

//获取Fram相关数据
void jsonFile::getFram(allFramType *allFram)
{
    QJsonObject all;
    all = docObj.value("allFram").toObject();
    allFram->framCurrent = all.value("framCurrent").toInt();
    allFram->framNum = all.value("framNum").toInt();
    //申请内存
    allFram->framTable.resize(allFram->framNum);

    for (int i =0;i < allFram->framNum;i++) {
        QString framNo = "fram"+QString::number(i);
        QJsonObject fram = all.value(framNo).toObject();

//        qDebug()<<fram.value("name").toString();
//        qDebug()<<fram.value("itemNum").toInt();


        allFram->framTable[i].name = fram.value("name").toString();
        allFram->framTable[i].itemCurrent = fram.value("itemCurrent").toInt();
        allFram->framTable[i].itemNum = fram.value("itemNum").toInt();

        //申请内存
        allFram->framTable[i].itemTable.resize(allFram->framTable[i].itemNum);

        for(int j = 0;j<allFram->framTable[i].itemNum;j++){
            QString itemNo = "item"+QString::number(j);
            QJsonObject item = fram.value(itemNo).toObject();
            allFram->framTable[i].itemTable[j].name = item.value("name").toString();
            allFram->framTable[i].itemTable[j].type = item.value("type").toString();
            allFram->framTable[i].itemTable[j].data = item.value("data").toString();
            allFram->framTable[i].itemTable[j].format = item.value("format").toString();
        }

    }
}

void jsonFile::rxFramToJson(allFramType *allFram)
{
    QJsonObject all;
    all.insert("framNum",allFram->framNum);
    all.insert("framCurrent",allFram->framCurrent);
    //获取fram数量
    for (int i = 0;i<allFram->framNum;i++) {
        QJsonObject fram;
        fram.insert("name",allFram->framTable[i].name);
        fram.insert("itemCurrent",allFram->framTable[i].itemCurrent);

        fram.insert("itemNum",allFram->framTable[i].itemNum);
        for (int j = 0;j < allFram->framTable[i].itemNum;j++) {
            QJsonObject item;
            item.insert("name",allFram->framTable[i].itemTable[j].name);
            item.insert("type",allFram->framTable[i].itemTable[j].type);
            item.insert("data",allFram->framTable[i].itemTable[j].data);
            item.insert("format",allFram->framTable[i].itemTable[j].format);

            QString itemNo = "item"+QString::number(j);
            fram.insert(itemNo,item);
        }
        QString framNo = "fram"+QString::number(i);
        all.insert(framNo,fram); //添加fram
    }
    docObj.insert("rxAllFram",all);
}

void jsonFile::rxGetFram(allFramType *allFram)
{
    QJsonObject all;
    all = docObj.value("rxAllFram").toObject();
    allFram->framCurrent = all.value("framCurrent").toInt();
    allFram->framNum = all.value("framNum").toInt();
    //申请内存
    allFram->framTable.resize(allFram->framNum);

    for (int i =0;i < allFram->framNum;i++) {
        QString framNo = "fram"+QString::number(i);
        QJsonObject fram = all.value(framNo).toObject();

//        qDebug()<<fram.value("name").toString();
//        qDebug()<<fram.value("itemNum").toInt();


        allFram->framTable[i].name = fram.value("name").toString();
        allFram->framTable[i].itemCurrent = fram.value("itemCurrent").toInt();
        allFram->framTable[i].itemNum = fram.value("itemNum").toInt();

        //申请内存
        allFram->framTable[i].itemTable.resize(allFram->framTable[i].itemNum);

        for(int j = 0;j<allFram->framTable[i].itemNum;j++){
            QString itemNo = "item"+QString::number(j);
            QJsonObject item = fram.value(itemNo).toObject();
            allFram->framTable[i].itemTable[j].name = item.value("name").toString();
            allFram->framTable[i].itemTable[j].type = item.value("type").toString();
            allFram->framTable[i].itemTable[j].data = item.value("data").toString();
            allFram->framTable[i].itemTable[j].format = item.value("format").toString();

        }

    }
}



//QString strText = QObject::tr("中文");

//// 从本地编码转换为utf-8编码,中文乱码处理
//QByteArray byteArray = strText.toLocal8Bit();
//// 这个是关键，转UTF8
//byteArray = QString::fromLocal8Bit(byteArray).toUtf8();

//QJsonParseError error;
//QJsonDocument  rootDoc = QJsonDocument::fromJson(byteArray, &error);
//QJsonObject rootObject = rootDoc.object();
//QString strCmd = rootObject.value(tagCmd).toString();





