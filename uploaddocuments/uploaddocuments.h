#pragma once

#include <QDebug>
#include <QDialog>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegExp>
#include <QSet>
#include <QSharedPointer>
#include <QString>

#include "db/IDB.h"

namespace Ui {
class UploadDocuments;
}

class UploadDocuments : public QDialog {
  Q_OBJECT

public:
  explicit UploadDocuments(const QSharedPointer<DB::IDB> &,
                           QWidget *parent = nullptr);
  ~UploadDocuments();

protected:
  const QSharedPointer<DB::IDB> &m_pDB;

private slots:
  void on_PB_EXPLORER_clicked();

  void on_PB_ADD_clicked();

private:
  Ui::UploadDocuments *ui;
};
