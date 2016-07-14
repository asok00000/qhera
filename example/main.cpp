
#include <QtCore/QCoreApplication>
#include "qhera.h"
#include "user.h"
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	DataBase::DatabaseConfig config;
	config.driverType = "QSQLITE";
	config.name = "./data.db";
	DataBase::getInstance()->initDb(config);
	QHera hera;
	//QList<User*> users = hera.queryAll<User>();
	User u;
	u.setId(2);
	u.setAge(12);
	u.setCreateTime("2016-7-12 18:39:20");
	u.setEmail("assads@qq.com");
	u.setName("HanMeiMei");
	u.setNickName("MeiMei");
	int affectedRows = hera.update<User>(&u, "[m_age]={m_age},[m_email]={m_email},[m_name]={m_name} where [m_id] = {m_id}");
	affectedRows = hera.deleteRecord<User>(&u, " where [m_id] = {m_id}");
	int id = hera.insert<User>(&u);
	QList<User*> users = hera.query<User>(&u, " where [m_age] = {m_age}");
	return a.exec();
}
