#ifndef UTILITY_H
#define UTILITY_H
#include <QString>
#include <QRegExp>
#include <QVariant>
class Utility
{
public:
	Utility();
	~Utility();
	const static  QString tablePrefix;
	const static  QString wordSpliter;
	const static  QString memberPrefix;
	static QString tableNameConvert2ClassName(const QString &tableName);
	static QString classNameConvert2TableName(const QString &className);
	static QString fieldNameConvert2MemberName(const QString &fieldName, const QString &className);
	static QString memberNameConvert2FieldName(const QString &memberName, const QString &className);
	static QString variant2String(QVariant data);
private:
	
};

#endif // UTILITY_H
