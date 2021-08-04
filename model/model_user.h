#pragma once

#include <QAbstractTableModel>
#include <QDebug>
#include <QList>
#include <QMessageBox>
#include <QObject>
#include <QSqlRecord>
#include <QVariant>
#include <QtSql>

#include "data.h"
#include "db/IDB.h"

class QAction;

class QSqlQuery;


/*------------------------------------------------------*/
struct DataModelUser : public QObject {
  Q_OBJECT

public:
  explicit DataModelUser(QObject *parent = nullptr);
  explicit DataModelUser(const QSqlQuery &, QObject *parent = nullptr);
  virtual ~DataModelUser() = default;

  uint id_;
  QString login_;
  QString password_;
  QString role_;
  bool is_activ_;
};
//----------------------
class ModelUser : public QAbstractTableModel {
public:
  explicit ModelUser(const QSharedPointer<DB::IDB> &, QObject *);
  virtual ~ModelUser() = default;

  int rowCount(const QModelIndex &) const override;
  int columnCount(const QModelIndex &) const override;
  QVariant data(const QModelIndex &, int) const override;
  QVariant headerData(int, Qt::Orientation, int) const override;
  bool setData(const QModelIndex &, const QVariant &, int) override;
  Qt::ItemFlags flags(const QModelIndex &I) const override;
  void AddToCat(DataModelUser *);

private:
  const QSharedPointer<DB::IDB> &m_pDB;
  QList<DataModelUser *> records;
};
/*------------------------------------------------------*/
