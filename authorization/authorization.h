#pragma once
#include "db/IDB.h"

#include <QDebug>
#include <QDialog>
#include <QSharedPointer>
#include <QString>

namespace Ui {
class Authorization;
}

class Authorization : public QDialog {
  Q_OBJECT

public:
  Authorization(QWidget *parent = nullptr);
  ~Authorization();

private slots:
  void on_PB_OK_clicked();

signals:
  void login_button_clicked();

private:
  Ui::Authorization *ui;

public:
  std::pair<QString, QString> GetValues();
};
