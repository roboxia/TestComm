#ifndef JSONFILE_H
#define JSONFILE_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariantMap>


typedef struct
{
   QString name;
   QString type;
   QString data;
   QString format;
} itemType;
//type = u8,u16,u32,float



typedef struct
{
    QString name;
    int itemNum;
    int itemCurrent;
    QVector<itemType> itemTable;
}framType;


typedef struct
{
    int framCurrent;
    int framNum;
    QVector<framType> framTable;
}allFramType;



class jsonFile
{
public:
    jsonFile();
    ~jsonFile();
    bool open(QString fileDir);  //打开文件
    void close();

    void framToJson(allFramType *allFram);
    void getFram(allFramType* allFram);

    void rxFramToJson(allFramType *allFram);
    void rxGetFram(allFramType* allFram);


private:
    QString m_fileDir;

    QJsonDocument jsonDoc;
    QJsonObject docObj;
};

#endif // JSONFILE_H
