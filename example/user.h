#ifndef USER_H
#define USER_H
#include <QObject>
class User : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int m_id READ getId WRITE setId)
    Q_PROPERTY(QString m_name READ getName WRITE setName)
    Q_PROPERTY(QString m_nickName READ getNickName WRITE setNickName)
    Q_PROPERTY(int m_age READ getAge WRITE setAge)
    Q_PROPERTY(QString m_email READ getEmail WRITE setEmail)
    Q_PROPERTY(QString m_createTime READ getCreateTime WRITE setCreateTime)
public:
    int getId() const
    {
        return m_id;
    }
    void setId(int val)
    {
        m_id = val;
    }
    QString getName() const
    {
        return m_name;
    }
    void setName(QString val)
    {
        m_name = val;
    }
    QString getNickName() const
    {
        return m_nickName;
    }
    void setNickName(QString val)
    {
        m_nickName = val;
    }
    int getAge() const
    {
        return m_age;
    }
    void setAge(int val)
    {
        m_age = val;
    }
    QString getEmail() const
    {
        return m_email;
    }
    void setEmail(QString val)
    {
        m_email = val;
    }
    QString getCreateTime() const
    {
        return m_createTime;
    }
    void setCreateTime(QString val)
    {
        m_createTime = val;
    }
private:
    int m_id;
    QString m_name;
    QString m_nickName;
    int m_age;
    QString m_email;
    QString m_createTime;

};
#endif