#ifndef QHERA_H
#define QHERA_H
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QSqlField>
#include <QString>
#include <QMetaProperty>
#include <QSqlQuery>
#include <QMap>
#include "utility.h"
class DataBase
{
public:
	struct DatabaseConfig
	{
		QString name;
		QString host;
		QString userName;
		QString password;
		QString connectOpt;
		QString driverType;
		int port;
	};
	DataBase(){}
	~DataBase(){}
	static DataBase *getInstance();
	void initDb(DatabaseConfig dbConfig);
	QSqlDatabase getDatabase() const { return database; }
private:
	QSqlDatabase database;
	QMap<QString, QSqlField> autoIncreateFields;
};
class QHera
{
public:
	QHera();
	~QHera();
	// query all record from table
	template <typename T>
	QList<T*> queryAll();
	//insert record into table ,return last insert id(if db support)
	template <typename T>
	int insert(T *model);
	//update record,return number rows affected
	template <typename T>
	int update(T*model, QString hql);
	//delete record
	template <typename T>
	int deleteRecord(T*model, QString hql);
	//query record
	template <typename T>
	QList<T*> query(T*model, QString hql);
private:
	QMap<QString, QSqlField> autoIncreateFields;
	template <typename T>
	QString compressHeraSql(T*model, QString hql);
};

template <typename T>
QList<T*> QHera::query(T*model, QString hql)
{
	const QMetaObject o = T::staticMetaObject;
	QString queryString = "SELECT * ";
	QStringList propertyList;
	for (int i = 0; i < o.propertyCount(); ++i)
	{
		QMetaProperty property = o.property(i);
		QString propertyName = property.name();
		if (propertyName == "objectName") {
			continue;
		}
		propertyList.append(propertyName);
	}
	queryString += " FROM ";
	queryString += Utility::classNameConvert2TableName(QString(o.className()));
	QSqlQuery q(DataBase::getInstance()->getDatabase());
	QString sql = compressHeraSql(model, hql);
	queryString += sql;
	QList<T*> objectList;
	if (!q.exec(queryString))
		qDebug() << q.lastError().text();
	while (q.next())
	{
		QSqlRecord record = q.record();
		T* object = new T;
		Q_FOREACH(const QString& prop, propertyList)
		{
			object->setProperty(prop.toUtf8(), record.value(Utility::memberNameConvert2FieldName(prop, o.className())));
		}
		objectList.append(object);
	}
	return objectList;
}

template <typename T>
int QHera::deleteRecord(T*model, QString hql)
{
	const QMetaObject o = T::staticMetaObject;
	QString queryString = "DELETE FROM %1 ";
	QString tableName = Utility::classNameConvert2TableName(QString(o.className()));
	queryString = queryString.arg(tableName);
	QString sql = compressHeraSql(model, hql);
	queryString += sql;
	QSqlQuery q(DataBase::getInstance()->getDatabase());
	if (!q.exec(queryString))
		qDebug() << q.lastError().text();
	return q.numRowsAffected();
}

template <typename T>
QString QHera::compressHeraSql(T*model, QString hql)
{
	QString sql = "";
	QStringList propertyList;
	QMap<QString, QString> memberFields;
	const QMetaObject o = T::staticMetaObject;
	for (int i = 0; i < o.propertyCount(); ++i)
	{
		QMetaProperty property = o.property(i);
		QString propertyName = property.name();
		QString fieldName = Utility::memberNameConvert2FieldName(propertyName, o.className());
		memberFields.insert(propertyName, fieldName);
		if (propertyName == "objectName")
		{
			continue;
		}
		propertyList.append(propertyName);
	}
	foreach(QString proper, propertyList)
	{
		QString rpField="[%1]", rpMember="{%1}";
		//convert to field name and replace
		rpField = rpField.arg(proper);
		rpMember = rpMember.arg(proper);
		QString fieldName = memberFields[proper];
		hql.replace(rpField, fieldName);
		//convert to member value and replace
		QVariant data = model->property(proper.toUtf8());
		hql.replace(rpMember, "'" + Utility::variant2String(data) + "'");
	}
	sql = hql;
	return sql;
}

template <typename T>
int QHera::update(T*model, QString hql)
{
	const QMetaObject o = T::staticMetaObject;
	QString queryString = "UPDATE  %1 SET ";
	QString tableName = Utility::classNameConvert2TableName(QString(o.className()));
	queryString = queryString.arg(tableName);
	QString sql = compressHeraSql(model, hql);
	queryString += sql;
	QSqlQuery q(DataBase::getInstance()->getDatabase());
	if (!q.exec(queryString))
		qDebug() << q.lastError().text();
	return q.numRowsAffected();
}

template <typename T>
int QHera::insert(T *model)
{
	const QMetaObject o = T::staticMetaObject;
	QString queryString = "INSERT INTO %1 ( ";
	QString tableName = Utility::classNameConvert2TableName(QString(o.className()));
	queryString = queryString.arg(tableName);
	QStringList propertyList;
	for (int i = 0; i < o.propertyCount(); ++i)
	{
		QMetaProperty property = o.property(i);
		QString propertyName = property.name();
		QString fieldName = Utility::memberNameConvert2FieldName(propertyName, o.className());
		if (propertyName == "objectName") 
		{
			continue;
		}
		if (autoIncreateFields.contains(tableName) && autoIncreateFields[tableName].name() == fieldName)
		{
			continue;
		}	
		propertyList.append(propertyName);
		queryString += fieldName;
		queryString += ",";
	}
	queryString.chop(1);
	queryString += ")VALUES(";
	Q_FOREACH(const QString& prop, propertyList)
	{
		queryString += "'%1',";
		QVariant data =  model->property(prop.toUtf8());
		queryString = queryString.arg(Utility::variant2String(data));
	}
	queryString.chop(1);
	queryString += ")";
	QSqlQuery q(DataBase::getInstance()->getDatabase());
	if (!q.exec(queryString))
		qDebug() << q.lastError().text();
	return q.lastInsertId().toInt();
}

template <typename T>
QList<T*> QHera::queryAll()
{
	const QMetaObject o = T::staticMetaObject;
	QString queryString = "SELECT * ";
	QStringList propertyList;
	for (int i = 0; i < o.propertyCount(); ++i) 
	{
		QMetaProperty property = o.property(i);
		QString propertyName = property.name();
		if (propertyName == "objectName") {
			continue;
		}
		propertyList.append(propertyName);
	}
	queryString += " FROM ";
	queryString += Utility::classNameConvert2TableName(QString(o.className()));
	QSqlQuery q(DataBase::getInstance()->getDatabase());

	QList<T*> objectList;
	if(!q.exec(queryString))
		qDebug()<<q.lastError().text();
	while (q.next())
	{
		QSqlRecord record = q.record();
		T* object = new T;
		Q_FOREACH(const QString& prop, propertyList)
		{
			object->setProperty(prop.toUtf8(),record.value(Utility::memberNameConvert2FieldName(prop,o.className())));
		}
		objectList.append(object);
	}
	return objectList;
}


#endif // QHERA_H
