#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSerialPort>
#include <QSerialPortInfo>

#include "jsonfile.h"

#include <QStandardItemModel>

//hexFlag
//
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void findPortTimer_timeout();
    void on_openPort_clicked(bool checked);

    void serialport_readyread();


    void on_framTable_clicked(const QModelIndex &index);

    void on_rxFramTable_clicked(const QModelIndex &index);

    void on_itemTable_clicked(const QModelIndex &index);

    void on_rxItemTable_clicked(const QModelIndex &index);

    void framTableChange(const QModelIndex &topLeft,const QModelIndex &bottomRight);
    void rxFramTableChange(const QModelIndex &topLeft,const QModelIndex &bottomRight);
    void itemTableChange(const QModelIndex &topLeft,const QModelIndex &bottomRight);
    void rxItemTableChange(const QModelIndex &topLeft,const QModelIndex &bottomRight);
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_saveFram_clicked();

    void on_rmFram_clicked();

    void on_tx_clicked();

    void on_checkBox_2_clicked();

    void on_checkBox_2_clicked(bool checked);

    void on_checkBox_clicked(bool checked);

    void on_addTx_clicked();

    void on_addRx_clicked();

    void on_rmTx_clicked();

    void on_rmRx_clicked();

    void on_itemTable_activated(const QModelIndex &index);

    void on_itemTable_pressed(const QModelIndex &index);

private:
    QSerialPort *serialport;//串口对象
    QTimer *findPortTimer; // 定时器，用于定时搜索串口
    QList<QSerialPortInfo> portlist;

    QByteArray recevieBuff;
    //配置文件
    jsonFile configFile;
    //fram数据
    allFramType allFram;
    allFramType rxAllFram;

    uint8_t framHead[2] = {0xAA,0x55};

    QStandardItemModel *framTableModel;
    QStandardItemModel *rxFramTableModel;
    QStandardItemModel *itemTableModel;
    QStandardItemModel *rxItemTableModel;

    Ui::Widget *ui;



    void initFramToTable();


    void FramDataToAsciiPack(framType fram,QByteArray *pack);
    void framDataToTableView(framType *fram, QString *p);

    QByteArray itemDataToByteArray(QString type, QString data);
    QString ByteArrayToitemData(QString type, QByteArray byte);

    QString itemDataToHexString(QString type, QString data);
    QString HexStringToitemData(QString type, QString hex);

//    QByteArray framDataToFramPack(framType *fram);
//    framType framPackToframData(QByteArray *pack);
    void framDataToFramPack(framType *fram, QByteArray *pack);
    void framPackToframData(framType *fram, QByteArray *pack);


    void showItemToTable(framType *fram, QStandardItemModel *model);
    void currentFramDisplayRefr(allFramType *all, QStandardItemModel *FBmodel);
    void appendNewFram(allFramType *all, QStandardItemModel *FBmodel);
    void deleteOneFram(allFramType *all, QStandardItemModel *FBmodel);
    void appendOneItem(framType *fram, QStandardItemModel *TBmodel);
    void deleteOneItem(framType *fram, QStandardItemModel *TBmodel);
    void modifyFramName(const QModelIndex *index, allFramType *all);
    void modifyItemValue(const QModelIndex *index, framType *fram);
    void currentItemDisplayRefr(framType *fram, QStandardItemModel *TBmodel);
    uint16_t modbusCRC16(QByteArray senddata);
    QString byteArrayToHexStr(const QByteArray &data);

    QByteArray readAnalysis(QByteArray newdata);
    void currentItemValueDisplayRefr(framType *fram, QStandardItemModel *TBmodel);
    void autoCalculate(framType *fram);
    void insertOneItem(framType *fram, QStandardItemModel *TBmodel);
};

#endif // WIDGET_H
