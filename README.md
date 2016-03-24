# QHera
a simple ORM lib depends on Qt 5.x or later

# Usage
1.db.xq/map.xq/name.xq these file should be make sure the QHera can find.default is source file.(notice: in this version,these files must be placed in the application running dirctory!)<br>
2.class map file should rename to class name and end with .xml extension,the default looking path is the app running path, you can change it by setting <code>static QString QHera::configPath;</code><br>
3.the rules to edit class map file or db config file,please look at the test project's xml fils<br>
4.db config file's name should be DbConnection.xml,and can changed by setting <code>static QString QHera::configPath;</code><br>
5.the model class should be  inherits from QObject and be set Q_OBJECT.</br>
6.API</br>
  <code>QList<T*> QHera::queryAll()</code></br>
  <code>QList<T*> QHera::queryByConditions(T& model,QStringList conditionProperties)</code></br>
  <code>QList<T*> QHera::query(QString sql)</code></br>
  <code>int QHera::insert(T& model)</code></br>
  <code>int QHera::update(T& model,QStringList changedProperties,QStringList conditions)</code></br>
  <code>bool QHera::connectDb()</code></br>
  <code>bool QHera::disconnectDb()</code></br>
  <code>bool QHera::exist(T& model,QStringList conditionProperties)</code></br>
  
