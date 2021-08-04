#pragma once

#include <QPair>
#include <QSqlDatabase>
#include <QString>
#include <QVariantList>
#include <QtSql>
#include <any>

#include "db/configure.h"

enum role { manager_, operator_, admin_ };

namespace DB {

class IDB {
public:
  IDB() = default;
  virtual ~IDB() = default;

  virtual bool ConnectDB(const QString &, const QString &, const QString &,
                         const QString &) = 0;

  virtual void DisconectDB() = 0;

  virtual bool CreateDB() = 0;

  virtual bool CreateUser(const QString &, const QString &, enum role) = 0;

  virtual bool CreateTable() = 0;

  virtual bool isOpenConnect() = 0;

  virtual bool QueryExecSelect(const QString &) = 0;

  virtual bool QueryExecInsert(const QString &) = 0;

  // virtual QPair<bool, QVariantList> QueryExec1(const QString &)= 0;
};

} // namespace DB
