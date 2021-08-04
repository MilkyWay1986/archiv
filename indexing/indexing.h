#pragma once

#include <QDialog>
#include <QMessageBox>
#include <QSharedPointer>
#include <QString>

#include "db/IDB.h"
#include <db/configure.h>

namespace Ui {
class Indexing;
}

class Indexing : public QDialog {
  Q_OBJECT

public:
  explicit Indexing(const QSharedPointer<DB::IDB> &,QWidget *parent = nullptr);
  ~Indexing();

protected:
  const QSharedPointer<DB::IDB> &m_pDB;

private slots:
  void on_PB_ADD_clicked();

private:
  Ui::Indexing *ui;
};
