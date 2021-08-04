#include "db\postgresqlDB.h"



namespace DB {

//-------------------------------------------------------------------------------------
PostgresqlDB::PostgresqlDB()
    : pDB(QSharedPointer<QSqlDatabase>::create(
          QSqlDatabase::addDatabase("QPSQL"))) {}
//-------------------------------------------------------------------------------------
bool PostgresqlDB::ConnectDB(const QString &host_name,
                             const QString &database_name,
                             const QString &user_name,
                             const QString &password) {

  pDB->setDatabaseName(database_name);
  pDB->setUserName(user_name);
  pDB->setHostName(host_name);
  pDB->setPassword(password);


  if (pDB->open())
    return true;
  else{
      qDebug()<< pDB->lastError();
      return false;

  }

}
//-------------------------------------------------------------------------------------
void PostgresqlDB::DisconectDB() {
  if (pDB->isOpen())
    pDB->close();
}
//-------------------------------------------------------------------------------------
bool PostgresqlDB::CreateDB() {

  return false;
}
//-------------------------------------------------------------------------------------
bool PostgresqlDB::CreateUser(const QString &user_name, const QString &password,
                              enum role role) {
  return false;
}
//-------------------------------------------------------------------------------------
bool PostgresqlDB::CreateTable() { return false; }

bool PostgresqlDB::isOpenConnect() { return pDB->isOpen(); }
//-------------------------------------------------------------------------------------
bool DB::PostgresqlDB::QueryExecSelect(const QString &query) {

  if (pDB->isOpen()) {
    QSqlQuery sql_query;
    sql_query.exec(query);
    while (sql_query.next())
      return true;
  }
  return false;
}
//-------------------------------------------------------------------------------------
bool PostgresqlDB::QueryExecInsert(const QString &query) {

  if (pDB->isOpen()) {
    if (pDB->transaction()) {
      QSqlQuery sql_query;
      sql_query.exec(query);
      qDebug() << sql_query.lastError();
      if (!pDB->commit()) {
        pDB->rollback();
        return false;
      } else
        return true;
    }
  }
  return false;
}
//-------------------------------------------------------------------------------------

} // namespace DB
