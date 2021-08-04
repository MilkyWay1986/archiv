#pragma once

#include <QAbstractTableModel>
#include <QDebug>
#include <QList>
#include <QObject>
#include <QVariant>
#include <QtSql>
#include <QMessageBox>


#include "data.h"
#include "db/IDB.h"

class QAction;

class Model : public QAbstractTableModel {
public:
  explicit Model(const QSharedPointer<DB::IDB> &, QObject *);
  virtual ~Model();

  int rowCount(const QModelIndex &) const override;
  int columnCount(const QModelIndex &) const override;
  QVariant data(const QModelIndex &, int) const override;
  QVariant headerData(int, Qt::Orientation, int) const override;
  bool setData(const QModelIndex &, const QVariant &, int) override;
  Qt::ItemFlags flags(const QModelIndex &) const override;
  void AddToCat(Data *);

private:
  const QSharedPointer<DB::IDB> &m_pDB;
  QList<Data *> records;
};
