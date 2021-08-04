#pragma once

#include <QAbstractTableModel>
#include <QDebug>
#include <QList>
#include <QMessageBox>
#include <QObject>
#include <QVariant>
#include <QtSql>
#include <QSqlRecord>

#include "data.h"
#include "db/IDB.h"

class QAction;

class QSqlQuery;

/*------------------------------------------------------*/
struct DataArchivBox : public QObject {
  Q_OBJECT

public:
  explicit DataArchivBox(QObject *parent = nullptr);
  explicit DataArchivBox(const QSqlQuery &, QObject *parent = nullptr);
  virtual ~DataArchivBox() = default;

  uint id_;
  QString number_;
  QString title;
  QString login_;
  bool is_activ_;
};
//----------------------
class ModelArchivBox : public QAbstractTableModel {
public:
  explicit ModelArchivBox(const QSharedPointer<DB::IDB> &, QObject *);
  virtual ~ModelArchivBox() = default;

  int rowCount(const QModelIndex &) const override;
  int columnCount(const QModelIndex &) const override;
  QVariant data(const QModelIndex &, int) const override;
  QVariant headerData(int, Qt::Orientation, int) const override;
  bool setData(const QModelIndex &, const QVariant &, int) override;
  Qt::ItemFlags flags(const QModelIndex &I) const override;
  void AddToCat(DataArchivBox *);

private:
  const QSharedPointer<DB::IDB> &m_pDB;
  QList<DataArchivBox *> records;
};
/*------------------------------------------------------*/
struct DataArchivFolder : public QObject {
  Q_OBJECT

public:
  explicit DataArchivFolder(QObject *parent = nullptr);
  DataArchivFolder(const QSqlQuery &, QObject *parent = nullptr);
  virtual ~DataArchivFolder() = default;

  uint id_;
  QString bnumber_;
  QString fnumber_;
  QString login_;
  bool is_activ_;
};
//----------------------
class ModelArchivFolder : public QAbstractTableModel {
public:
  explicit ModelArchivFolder(const QSharedPointer<DB::IDB> &, QObject *);
  virtual ~ModelArchivFolder() = default;

  int rowCount(const QModelIndex &) const override;
  int columnCount(const QModelIndex &) const override;
  QVariant data(const QModelIndex &, int) const override;
  QVariant headerData(int, Qt::Orientation, int) const override;
  bool setData(const QModelIndex &, const QVariant &, int) override;
  Qt::ItemFlags flags(const QModelIndex &I) const override;
  void AddToCat(DataArchivFolder *);

private:
  const QSharedPointer<DB::IDB> &m_pDB;
  QList<DataArchivFolder *> records;
};
/*------------------------------------------------------*/
struct DataArchivRecord : public QObject {
  Q_OBJECT

public:
  explicit DataArchivRecord(QObject *parent = nullptr);
  DataArchivRecord(const QSqlQuery &, QObject *parent = nullptr);
  virtual ~DataArchivRecord() = default;

  uint id_;
  QString fnumber_;
  QString rnumber_;
  QString owner_login_;
  QVariant last_date_change_;
  QString last_change_login_;
  bool is_activ_;
};
//----------------------
class ModelArchivRecord : public QAbstractTableModel {
public:
  explicit ModelArchivRecord(const QSharedPointer<DB::IDB> &, QObject *);
  virtual ~ModelArchivRecord() = default;

  int rowCount(const QModelIndex &) const override;
  int columnCount(const QModelIndex &) const override;
  QVariant data(const QModelIndex &, int) const override;
  QVariant headerData(int, Qt::Orientation, int) const override;
  bool setData(const QModelIndex &, const QVariant &, int) override;
  Qt::ItemFlags flags(const QModelIndex &I) const override;
  void AddToCat(DataArchivRecord *);

private:
  const QSharedPointer<DB::IDB> &m_pDB;
  QList<DataArchivRecord *> records;
};
/*------------------------------------------------------*/
struct DataArchivNumber : public QObject {
  Q_OBJECT

public:
  explicit DataArchivNumber(QObject *parent = nullptr);
  DataArchivNumber(const QSqlQuery &, QObject *parent = nullptr);
  virtual ~DataArchivNumber() = default;

  uint id_;
  QString rnumber_;
  QString owner_login_;
  QString number_document;
  QString data_document;
  QString ogrn_document;
  QVariant last_date_change_;
  QString last_change_login_;
  QString path_;
  bool is_activ_;
};
//----------------------
class ModelArchivNumber : public QAbstractTableModel {
public:
  explicit ModelArchivNumber(const QSharedPointer<DB::IDB> &, QObject *);
  virtual ~ModelArchivNumber() = default;

  int rowCount(const QModelIndex &) const override;
  int columnCount(const QModelIndex &) const override;
  QVariant data(const QModelIndex &, int) const override;
  QVariant headerData(int, Qt::Orientation, int) const override;
  bool setData(const QModelIndex &, const QVariant &, int) override;
  Qt::ItemFlags flags(const QModelIndex &I) const override;
  void AddToCat(DataArchivNumber *);

private:
  const QSharedPointer<DB::IDB> &m_pDB;
  QList<DataArchivNumber *> records;
};
/*------------------------------------------------------*/
struct DataArchivOgrn : public QObject {
  Q_OBJECT

public:
  explicit DataArchivOgrn(QObject *parent = nullptr);
  DataArchivOgrn(const QSqlQuery &, QObject *parent = nullptr);
  virtual ~DataArchivOgrn() = default;

  uint id_;
  QString rnumber_;
  QString owner_login_;
  QString number_document;
  QString data_document;
  QString ogrn_document;
  QVariant last_date_change_;
  QString last_change_login_;
  QString path_;
  bool is_activ_;
};
//----------------------
class ModelArchivOgrn : public QAbstractTableModel {
public:
  explicit ModelArchivOgrn(const QSharedPointer<DB::IDB> &, QObject *);
  virtual ~ModelArchivOgrn() = default;

  int rowCount(const QModelIndex &) const override;
  int columnCount(const QModelIndex &) const override;
  QVariant data(const QModelIndex &, int) const override;
  QVariant headerData(int, Qt::Orientation, int) const override;
  bool setData(const QModelIndex &, const QVariant &, int) override;
  Qt::ItemFlags flags(const QModelIndex &I) const override;
  void AddToCat(DataArchivOgrn *);

private:
  const QSharedPointer<DB::IDB> &m_pDB;
  QList<DataArchivOgrn *> records;
};
/*------------------------------------------------------*/
