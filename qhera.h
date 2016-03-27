#ifndef QHERA_H

#define QHERA_H

#include <QObject>
#include <QDebug>
#include <QMetaProperty>
#include <QFile>
#include <QXmlQuery>
#include <QXmlResultItems>
#include <QBuffer>
#include <QXmlFormatter>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlField>
#define PROPERTY_TCMAPPER_STRUCT "TCmapper"
#define PROPERTY_CTMAPPER_STRUCT "CTmapper"
#define PROPERTY_TABLE_NAME "tableName"

//#define QHERA(name,type) Q_PROPERTY(type name READ getName WRITE setName)\
//    private:\
//        type ##name;\
//    public:\
//    inline void set##name(type v) {\
//        ##name = v;\
//    }\
//    inline type get##name() {\
//        return ##name;\
//    }

enum KeyType
{
  PRIMARY,
  AUTOINCREAMENT,
  ONLY,
  NORMAL
};

enum PropertyType
{
    INT,
    STRING,
};

struct PropertyInfo
{
    QString name;
    PropertyType type;
    KeyType keyType;
    PropertyInfo()
    {
        name="";
        type = STRING;
        keyType = NORMAL;
    }

    QString toString()
    {
        return name+":"+type;
    }
};
struct DbConfig
{
    QString driverName;
    QString hostName;
    QString dbName;
    QString userName;
    QString password;
    QString connectionName;
    int port;
    bool isLoad;
    DbConfig()
    {
        driverName = "";
        hostName = "";
        dbName = "";
        userName ="";
        password = "";
        isLoad = false;
        port = -1;
        connectionName = "qhera";
    }
};

Q_DECLARE_METATYPE(PropertyInfo)
template<class T>  class QHera
{
public:
    static QString configPath;
    static DbConfig dbConfig;
public:
    QHera()
    {
        loadMapConfig();
        loadDbConfig();
    }
    QList<T*> queryAll()
    {
        QList<T*> lstRet;
        if(connectDb())
        {
            QMap<QString,PropertyInfo> mapperInfo = targetModel.property(PROPERTY_TCMAPPER_STRUCT).value<QMap<QString,PropertyInfo>>();
            QString tableName = targetModel.property(PROPERTY_TABLE_NAME).toString();
            QString sql = "select * from %1;";
            sql = sql.arg(tableName);
            QSqlQuery query(*db);
            if (query.exec(sql))
            {
                while (query.next())
                {
                    T *result = new T();
                    QSqlRecord record = query.record();
                    int columnCount = record.count();
                    for(int i = 0 ; i < columnCount; ++ i)
                    {
                        QSqlField field =  record.field(i);
                        QString name = field.name();
                        QVariant value = field.value();
                        if(mapperInfo.contains(name))
                        {
                            PropertyInfo &propertyInfo = mapperInfo[name];
                            QVariant data;
                            switch (propertyInfo.type)
                            {
                            case INT:
                                data.setValue(value.toInt());
                                break;
                            default:
                                data.setValue(value.toString());
                                break;
                            }
                            result->setProperty(propertyInfo.name.toUtf8(),data);
                        }
                    }
                    lstRet<<result;
                }
            }
            else
            {
                qDebug() << query.lastError()<<sql;
            }
        }
        return lstRet;
    }
    QList<T*> queryByConditions(T& model,QStringList conditionProperties)
    {
        QList<T*> lstRet;
        if(connectDb())
        {
            QMap<QString,PropertyInfo> mapperInfo = targetModel.property(PROPERTY_TCMAPPER_STRUCT).value<QMap<QString,PropertyInfo>>();
            QString tableName = targetModel.property(PROPERTY_TABLE_NAME).toString();
            QString term = assembleSqlConditions(model,conditionProperties);
            QString sql = "select * from %1 %2;";
            sql = sql.arg(tableName).arg(term);
            QSqlQuery query(*db);
            if (query.exec(sql))
            {
                while (query.next())
                {
                    T *result = new T();
                    QSqlRecord record = query.record();
                    int columnCount = record.count();
                    for(int i = 0 ; i < columnCount; ++ i)
                    {
                        QSqlField field =  record.field(i);
                        QString name = field.name();
                        QVariant value = field.value();
                        if(mapperInfo.contains(name))
                        {
                            PropertyInfo &propertyInfo = mapperInfo[name];
                            QVariant data;
                            switch (propertyInfo.type)
                            {
                            case INT:
                                data.setValue(value.toInt());
                                break;
                            default:
                                data.setValue(value.toString());
                                break;
                            }
                            result->setProperty(propertyInfo.name.toUtf8(),data);
                        }
                    }
                    lstRet<<result;
                }
            }
            else
            {
                qDebug() << query.lastError()<<sql;
            }
        }
        return lstRet;
    }
    QList<T*> query(QString sql)
    {
        QList<T*> lstRet;
        if(connectDb())
        {
            QMap<QString,PropertyInfo> mapperInfo = targetModel.property(PROPERTY_TCMAPPER_STRUCT).value<QMap<QString,PropertyInfo>>();
            QSqlQuery query(*db);
            if (query.exec(sql))
            {
                while (query.next())
                {
                    T *result = new T();
                    QSqlRecord record = query.record();
                    int columnCount = record.count();
                    for(int i = 0 ; i < columnCount; ++ i)
                    {
                        QSqlField field =  record.field(i);
                        QString name = field.name();
                        QVariant value = field.value();
                        if(mapperInfo.contains(name))
                        {
                            PropertyInfo &propertyInfo = mapperInfo[name];
                            QVariant data;
                            switch (propertyInfo.type)
                            {
                            case INT:
                                data.setValue(value.toInt());
                                break;
                            default:
                                data.setValue(value.toString());
                                break;
                            }
                            result->setProperty(propertyInfo.name.toUtf8(),data);
                        }
                    }
                    lstRet<<result;
                }
            }
            else
            {
                qDebug() << query.lastError()<<sql;
            }
        }
        return lstRet;
    }

    int insert(T& model)
    {
        int iRet = -1;
        if(connectDb())
        {
            QString term = assembleInsertSqlTerm(model);
            QString tableName = targetModel.property(PROPERTY_TABLE_NAME).toString();
            QString sql = "INSERT INTO %1 %2";
            sql = sql.arg(tableName).arg(term);
            QSqlQuery query(*db);
            if (query.exec(sql))
            {
                iRet = query.numRowsAffected();
            }
            else
            {
                qDebug() << query.lastError()<<sql;
            }
        }

        return iRet;
    }
    int update(T& model,QStringList changedProperties,QStringList conditions)
    {
        int iRet = -1;
        if(connectDb())
        {
            QString term = assembleUpdateSqlTerm(model,changedProperties);
            QString sconditions =assembleSqlConditions(model,conditions);
            QString tableName = targetModel.property(PROPERTY_TABLE_NAME).toString();
            QString sql = "UPDATE %1 SET %2 %3;";
            sql = sql.arg(tableName).arg(term).arg(sconditions);
            QSqlQuery query(*db);
            if (query.exec(sql))
            {
                iRet = query.numRowsAffected();
            }
            else
            {
                qDebug() << query.lastError()<<sql;
            }
        }
        return iRet;
    }
    bool connectDb()
    {
        if(db->isOpen())
        {
            return true;
        }
        else
        {
            if (!db->open())
            {
                qDebug() << db->lastError();
                return false;
            }
            return true;
        }
    }
    bool disconnectDb()
    {
        if(db->isOpen())
        {
            db->close();
        }
        return true;
    }
    bool exist(T& model,QStringList conditionProperties)
    {
        bool bRet = false;
        if(connectDb())
        {
            QString condition = assembleSqlConditions(model,conditionProperties);
            QString tableName = targetModel.property(PROPERTY_TABLE_NAME).toString();
            QString sql = "SELECT COUNT(*) FROM %1 %2";
            sql = sql.arg(tableName).arg(condition);
            QSqlQuery query(*db);
            if (query.exec(sql))
            {
                if(query.next())
                {
                    if(query.value(0).toInt() > 0)
                        bRet = true;
                }
            }
            else
            {
                qDebug() << query.lastError()<<sql;
            }
        }
        return bRet;
    }

private:
    void loadMapConfig()
    {
        QString className = T::staticMetaObject.className();
        QFile xmlFile(configPath+className+".xml");
        if(!xmlFile.open(QIODevice::ReadWrite))
        {
            qDebug()<<"Open xml config file error,please check.";
            return;
        }
        QXmlQuery query;
        query.setFocus(&xmlFile);
        query.setQuery(QUrl::fromLocalFile("./map.xq"));
        //query result mapper
        if(query.isValid())
        {
            QStringList propertyMaper;
            query.evaluateTo(&propertyMaper);
            if(propertyMaper.count()> 0)
            {
                //iterate the properties map
                QMap<QString,PropertyInfo> tableClassMap;
                QMap<QString,PropertyInfo> classTableMap;
                foreach (QString record, propertyMaper)
                {
                    if(record.isEmpty() || record.isNull())
                        continue;
                    QStringList tmp = record.split('|');
                    if(tmp.count() == 4)
                    {
                        QString column = tmp[0];
                        QString property = tmp[1];
                        PropertyType type = STRING;
                        if(tmp[2] == "INT")
                        {
                            type = INT;
                        }
                        else/*(tmp[2] == "STRING")*/
                        {
                            type = STRING;
                        }
                        PropertyInfo info;
                        info.name = property;
                        info.type = type;
                        tableClassMap.insert(column,info);
                        if(tmp[3] == "PRIMARY")
                        {
                            info.keyType = PRIMARY;
                        }
                        else if(tmp[3] == "AUTOINCREAMENT")
                        {
                            info.keyType = AUTOINCREAMENT;
                        }
                        else if(tmp[3] == "ONLY")
                        {
                            info.keyType = ONLY;
                        }
                        else
                        {
                            info.keyType = NORMAL;
                        }
                        info.name = column;
                        classTableMap.insert(property,info);
                    }
                    else if(tmp.count() == 2)
                    {
                        QString column = tmp[0];
                        QString property = tmp[1];
                        PropertyType type = STRING;
                        PropertyInfo info;
                        info.name = property;
                        info.type = type;
                        tableClassMap.insert(column,info);
                    }
                    else
                    {
                        //xml property count error
                        qDebug()<<"map record is invalid,error:"<<tmp;
                    }

                }
                QVariant data;
                data.setValue(tableClassMap);
                targetModel.setProperty(PROPERTY_TCMAPPER_STRUCT,data);
                data.setValue(classTableMap);
                targetModel.setProperty(PROPERTY_CTMAPPER_STRUCT,data);
            }
            else
            {
                qDebug()<<"Model-Table map is empty!mapper file is invalid,please check the config xml.";
            }
        }
        else
        {
            qDebug()<<"load xq file error,please check the map.xq file";
        }
        //query table name
        query.setQuery(QUrl::fromLocalFile("./name.xq"));
        if(query.isValid())
        {
            QString tableName = "";
            query.evaluateTo(&tableName);
            tableName =  tableName.simplified();
            QVariant data;
            data.setValue(tableName);
            targetModel.setProperty(PROPERTY_TABLE_NAME,data);
        }
        else
        {
            qDebug()<<"load xq file error,please check the name.xq file";
        }
    }
    void loadDbConfig()
    {
        if(!dbConfig.isLoad)
        {
            QFile xmlFile(configPath+"DbConnection.xml");
            if(!xmlFile.open(QIODevice::ReadWrite))
            {
                qDebug()<<"Open DbConnection.xml config file error,please check.";
                return;
            }
            QXmlQuery query;
            query.setFocus(&xmlFile);
            query.setQuery(QUrl::fromLocalFile("./db.xq"));
            if(query.isValid())
            {
                QString config = "";
                query.evaluateTo(&config);
                config =  config.simplified();
                QStringList configs = config.split('|');
                if(configs.count() == 7)
                {
                    dbConfig.driverName = configs[0];
                    dbConfig.hostName = configs[1];
                    dbConfig.dbName = configs[2];
                    dbConfig.userName = configs[3];
                    dbConfig.password = configs[4];
                    dbConfig.port = configs[5].toInt();
                    dbConfig.connectionName = configs[6];
                    dbConfig.isLoad = true;
                }
                else if(configs.count() == 3)
                {
                    dbConfig.driverName = configs[0];
                    dbConfig.hostName = configs[1];
                    dbConfig.dbName = configs[2];
                    dbConfig.isLoad = true;
                }
                else
                {
                    qDebug()<<"db config file is invalid.";
                    return;
                }
                static QSqlDatabase db  = QSqlDatabase::addDatabase(dbConfig.driverName,dbConfig.connectionName);
                db.setDatabaseName(dbConfig.dbName);
                db.setUserName(dbConfig.userName);
                db.setHostName(dbConfig.hostName);
                db.setPassword(dbConfig.password);
                db.setPort(dbConfig.port);
                this->db = &db;
            }
            else
            {
                qDebug()<<"load xq file error,please check the db.xq file";
            }
        }
    }
    QString assembleUpdateSqlTerm(T& model,QStringList changedProperties)
    {
        QString term = "";
        QMap<QString,PropertyInfo> mapperInfo = targetModel.property(PROPERTY_CTMAPPER_STRUCT).value<QMap<QString,PropertyInfo>>();
        foreach (QString p, changedProperties)
        {
            //the model has this property
            if(mapperInfo.contains(p))
            {
                PropertyInfo &info = mapperInfo[p];
                if(info.keyType == AUTOINCREAMENT)continue;
                switch (info.type)
                {
                case INT:
                    term += " %1 = %2 ,";
                    term = term.arg(info.name).arg(model.property(p.toUtf8()).toInt());
                    break;
                default:
                    term += " %1 = '%2' ,";
                    term = term.arg(info.name).arg(model.property(p.toUtf8()).toString());
                    break;
                }
            }
        }
        //remove last ,
        if(term != "")
        {
            term.remove(term.length()-1,1);
        }
        return term;
    }
    QString assembleSqlConditions(T& model,QStringList conditions)
    {
        QString term = " WHERE 1= 1 ";
        QMap<QString,PropertyInfo> mapperInfo = targetModel.property(PROPERTY_CTMAPPER_STRUCT).value<QMap<QString,PropertyInfo>>();
        foreach (QString p, conditions)
        {
            //the model has this property
            if(mapperInfo.contains(p))
            {
                PropertyInfo &info = mapperInfo[p];
                switch (info.type)
                {
                case INT:
                    term += " AND %1 = %2 ";
                    term = term.arg(info.name).arg(model.property(p.toUtf8()).toInt());
                    break;
                default:
                    term += " AND %1 = '%2' ";
                    term = term.arg(info.name).arg(model.property(p.toUtf8()).toString());
                    break;
                }
            }
        }

        return term;
    }
    QString assembleInsertSqlTerm(T& model)
    {
        QString term = "(%1)VALUES(%2)";
        QMap<QString,PropertyInfo> mapperInfo = targetModel.property(PROPERTY_CTMAPPER_STRUCT).value<QMap<QString,PropertyInfo>>();
        QStringList fields,properties;
        int propertyCount = model.metaObject()->propertyCount();
        for(int i = 0 ; i < propertyCount; ++ i)
        {
            QString name = model.metaObject()->property(i).name();
            if(mapperInfo.contains(name))
            {
                PropertyInfo &info = mapperInfo[name];
                if(info.keyType == AUTOINCREAMENT)continue;
                fields<<info.name;
                switch (info.type)
                {
                case INT:
                    properties<<QString::number(model.property(name.toUtf8()).toInt());
                    break;
                default:
                    properties<<"'"+model.property(name.toUtf8()).toString()+"'";
                    break;
                }
            }
        }
        QString f = fields.join(',');
        QString p = properties.join(',');
        term = term.arg(f).arg(p);
        return term;
    }

private:
    T targetModel;
	  static QSqlDatabase *db;
};
template<class T>  QString QHera<T>::configPath = "./";
template<class T>  DbConfig QHera<T>::dbConfig = DbConfig();
template<class T>  QSqlDatabase* QHera<T>::db = NULL;
#endif // QHERA_H

