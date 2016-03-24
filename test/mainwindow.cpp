#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "birdmodel.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QHera<BirdModel> a;
    //query all records
    QList<BirdModel*> lstBirds= a.queryAll();
    for(int i = 0 ; i< lstBirds.count(); ++ i)
    {
        qDebug()<<lstBirds[i]->toString();
        lstBirds[i]->setName(lstBirds[i]->getName()+QString::number(i));
        QStringList changedProperty,conditionProperty;
        changedProperty<<"name";
        conditionProperty<<"id";
        //update by strict conditions
        qDebug()<<"Affected rows:"<<a.update(*lstBirds[i],changedProperty,conditionProperty);
    }
    lstBirds= a.queryAll();
    for(int i = 0 ; i< lstBirds.count(); ++ i)
    {
        qDebug()<<lstBirds[i]->toString();
    }
    //query by strict conditions
    BirdModel bird;
    bird.setAge(13);
    QStringList conditions;
    conditions<<"age";
    lstBirds= a.queryByConditions(bird,conditions);
    for(int i = 0 ; i< lstBirds.count(); ++ i)
    {
        qDebug()<<lstBirds[i]->toString();
    }
    //query by custom sql
    lstBirds= a.query("select * from Account");
    for(int i = 0 ; i< lstBirds.count(); ++ i)
    {
        qDebug()<<lstBirds[i]->toString();
    }
    //insert
    bird.setName("HuMouMou");
    bird.setAddress("SC");
    bird.setSex(0);
    qDebug()<<"Affected rows:"<<a.insert(bird);
    lstBirds= a.queryAll();
    for(int i = 0 ; i< lstBirds.count(); ++ i)
    {
        qDebug()<<lstBirds[i]->toString();
    }
    //exist
    qDebug()<<"exist:"<<a.exist(bird,conditions);
}

MainWindow::~MainWindow()
{
    delete ui;
}
