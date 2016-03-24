#include "birdmodel.h"

BirdModel::BirdModel()
{
    name = metaObject()->className();
}

QString BirdModel::getName() const
{
    return name;
}

void BirdModel::setName(const QString &value)
{
    name = value;
}

QString BirdModel::getAddress() const
{
    return address;
}

void BirdModel::setAddress(const QString &value)
{
    address = value;
}

int BirdModel::getSex() const
{
    return sex;
}

void BirdModel::setSex(int value)
{
    sex = value;
}

int BirdModel::getAge() const
{
    return age;
}

void BirdModel::setAge(int value)
{
    age = value;
}

int BirdModel::getId() const
{
    return id;
}

void BirdModel::setId(int value)
{
    id = value;
}

