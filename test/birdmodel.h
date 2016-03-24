#ifndef BIRDMODEL_H
#define BIRDMODEL_H
#include "../qhera.h"
class BirdModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(QString address READ getAddress WRITE setAddress)
    Q_PROPERTY(int sex READ getSex WRITE setSex)
    Q_PROPERTY(int age READ getAge WRITE setAge)
    Q_PROPERTY(int id READ getId WRITE setId)
public:
    BirdModel();
    QString getName() const;
    void setName(const QString &value);
    QString getAddress() const;
    void setAddress(const QString &value);

    int getSex() const;
    void setSex(int value);

    int getAge() const;
    void setAge(int value);

    int getId() const;
    void setId(int value);
    QString toString()
    {
       return "Bird "+QString::number(id)+":"+name+"|"+address+"|"+QString::number(sex)+"|"+QString::number(age);
    }

private:
    QString name;
    QString address;
    int sex;
    int age;
    int id;
};

#endif // BIRDMODEL_H
