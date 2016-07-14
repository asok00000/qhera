#include "qhera.h"

QHera::QHera()
{
	QStringList tables = DataBase::getInstance()->getDatabase().tables();
	foreach(QString table, tables)
	{
		QSqlRecord record = DataBase::getInstance()->getDatabase().record(table);
		for (int i = 0; i < record.count(); ++i)
		{
			if (record.field(i).isAutoValue())
			{
				autoIncreateFields.insert(table, record.field(i));
				qDebug() << record.field(i).name();
				break;
			}
		}
	}
}

QHera::~QHera()
{

}

DataBase * DataBase::getInstance()
{
	static DataBase *db = new DataBase;
	return db;
}

void DataBase::initDb(DatabaseConfig config)
{
	database = QSqlDatabase::addDatabase(config.driverType);
	database.setDatabaseName(config.name);
	database.setConnectOptions(config.connectOpt);
	database.setHostName(config.host);
	database.setPassword(config.password);
	database.setPort(config.port);
	database.setUserName(config.userName);
	if (!database.open())
	{
		qDebug() << database.lastError();
		//return result;
	}
	QStringList tables = database.tables();
	foreach(QString table, tables)
	{
		QSqlRecord record = database.record(table);
		for (int i = 0; i < record.count(); ++i)
		{
			if (record.field(i).isAutoValue())
			{
				autoIncreateFields.insert(table, record.field(i));
				qDebug() << record.field(i).name();
				break;
			}
		}
	}
}
