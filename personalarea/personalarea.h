#pragma once

#include "archiv/archiv.h"
#include "authorization/authorization.h"
#include "db/IDB.h"
#include "indexing/indexing.h"
#include "uploadbarcodes/uploadbarcodes.h"
#include "uploaddocuments/uploaddocuments.h"
#include "users/users.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QSharedPointer>

namespace Ui {
class PersonalArea;
}

class PersonalArea : public QMainWindow {
  Q_OBJECT

public:
  explicit PersonalArea(const QSharedPointer<DB::IDB> &p,
                        QWidget *parent = nullptr);
  ~PersonalArea();

public:
  void DisplayArchiv();
  void DisplayAuthorization();
  void DisplayIndexing();
  void DisplayUploadBarcodes();
  void DisplayUploadDocuments();
  void DisplayUsers();
  void DisplayPersonalArea();

  const QSharedPointer<DB::IDB> &GetpDB() const;

protected:
  const QSharedPointer<DB::IDB> &m_pDB;

private slots:
  void AuthorizationUser();

  void on_PB_USERS_clicked();

  void on_PB_ARCHIV_clicked();

  void on_PB_ADDBORCODS_clicked();

  void on_PB_ADDDOCUMENTS_clicked();

  void on_PB_INDEXING_clicked();

private:
  Ui::PersonalArea *ui;

  Authorization authorization;

  QSharedPointer<Users> users;
  QSharedPointer<Archiv> archiv;
  QSharedPointer<Indexing> indexing;
  QSharedPointer<UploadBarcodes> upload_barcodes;
  QSharedPointer<UploadDocuments> upload_documents;
};
