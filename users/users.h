#pragma once

#include <QDialog>
#include <QMessageBox>

#include "db/IDB.h"
#include "model/model_user.h"

namespace Ui {
class Users;
}

class Users : public QDialog {
  Q_OBJECT

public:
  explicit Users(QWidget *parent = nullptr);

  Users(const QSharedPointer<DB::IDB> &pDB, QWidget *parent = nullptr);
  ~Users();

protected:
  const QSharedPointer<DB::IDB> &m_pDB;
  ModelUser *m_model_user;

private slots:
  void on_PB_ADDUSER_clicked();

  void on_PB_SEARCH_clicked();

private:
  Ui::Users *ui;
};
