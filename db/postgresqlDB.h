#pragma once

#include "db/IDB.h"
#include <QSharedPointer>
#include <QSqlDatabase>
#include <tuple>


namespace DB {
class PostgresqlDB : public IDB {
public:
  PostgresqlDB();
  ~PostgresqlDB() = default;

  bool ConnectDB(const QString &, const QString &, const QString &,
                 const QString &) override;

  void DisconectDB() override;

  bool CreateDB() override;

  bool CreateUser(const QString &, const QString &, enum role) override;

  bool CreateTable() override;

  bool isOpenConnect() override;

  bool QueryExecSelect(const QString &) override;

  bool QueryExecInsert(const QString &) override;

  // QPair<bool, QVariantList> QueryExec1(const QString &) override;

protected:
  QSharedPointer<QSqlDatabase> pDB;
};
} // namespace DB
