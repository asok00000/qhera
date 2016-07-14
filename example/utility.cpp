#include "utility.h"
 const QString Utility::tablePrefix = "t_";
 const QString Utility::memberPrefix = "m_";
 const QString Utility::wordSpliter = "_";
Utility::Utility()
{

}

Utility::~Utility()
{

}

QString Utility::tableNameConvert2ClassName(const QString &tableName)
{
	QString className = tableName.toLower();
	int i = className.indexOf(tablePrefix);
	if (i == 0)
	{
		className.remove(0, tablePrefix.length());
	}
	i = className.indexOf(wordSpliter);
	while (i >= 0)
	{
		if (className.length() > i)
		{
			className[i + 1] = className[i + 1].toUpper();
		}
		i = className.indexOf(wordSpliter,i+1);
	}
	className.remove(wordSpliter);
	if (!className.isEmpty()&&className[0].isLower())
	{
		className[0] = className[0].toUpper();
	}
	return className;
}

QString Utility::fieldNameConvert2MemberName(const QString &fieldName, const QString &className)
{
	if (fieldName.isEmpty() || className.isEmpty())
	{
		return "";
	}
	QString memberName = fieldName.toLower();
	QString filedPrefix = className[0].toLower();
	if (memberName[0] == filedPrefix[0])
	{
		memberName.remove(0, 1);
	}
	memberName = tableNameConvert2ClassName(memberName);
	return memberName;
}

QString Utility::classNameConvert2TableName(const QString &className)
{
	QString tableName = className;
	tableName[0] = tableName[0].toLower();
	QRegExp reg("[A-Z]");
	int upperLetterIndex = tableName.indexOf(reg);
	while (upperLetterIndex > 0)
	{
		tableName[upperLetterIndex] = tableName[upperLetterIndex].toLower();
		tableName.insert(upperLetterIndex,wordSpliter);
		upperLetterIndex = tableName.indexOf(reg, upperLetterIndex);
	}
	return tablePrefix+tableName;
}

QString Utility::memberNameConvert2FieldName(const QString &memberName, const QString &className)
{
	QString fieldName = memberName;
	int index = fieldName.indexOf(memberPrefix);
	if (index == 0)
	{
		fieldName.remove(index, memberPrefix.count());
	}
	fieldName[0] = fieldName[0].toLower();
	fieldName = className[0].toLower() + fieldName;
	fieldName = classNameConvert2TableName(fieldName);
	index = fieldName.indexOf(tablePrefix);
	if (index == 0)
	{
		fieldName.remove(index, tablePrefix.count());
	}
	return fieldName;
}

QString Utility::variant2String(QVariant data)
{
	if (data.isValid())
	{
		switch (data.type())
		{
		case QVariant::Int:
			return QString::number(data.toInt());
		case QVariant::String:
			return data.toString();
		default:
			return "";
		}
	}
	else
	{
		return "";
	}
}
