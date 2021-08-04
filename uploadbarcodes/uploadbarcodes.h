#pragma once

#include <QDialog>
#include <QMessageBox>
#include <QSharedPointer>

#include <algorithm>

#include "db/IDB.h"

namespace Ui {
class UploadBarcodes;
}

class UploadBarcodes : public QDialog {
  Q_OBJECT

public:
  explicit UploadBarcodes(const QSharedPointer<DB::IDB> &,QWidget *parent = nullptr);
  ~UploadBarcodes();

protected:
  const QSharedPointer<DB::IDB> &m_pDB;

private slots:
    void on_PB_ADD_clicked();

private:
  Ui::UploadBarcodes *ui;
};
