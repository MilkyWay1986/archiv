#include "personalarea.h"
#include "db/configure.h"
#include "ui_personalarea.h"

PersonalArea::PersonalArea(const QSharedPointer<DB::IDB> &pDB, QWidget *parent)
    : QMainWindow(parent), m_pDB(pDB), ui(new Ui::PersonalArea),
      users(new Users(pDB)), archiv(new Archiv(pDB)),
      indexing(new Indexing(pDB)), upload_barcodes(new UploadBarcodes(pDB)),
      upload_documents(new UploadDocuments(pDB)) {

  connect(&authorization, SIGNAL(login_button_clicked()), this,
          SLOT(AuthorizationUser()));
  ui->setupUi(this);

}

PersonalArea::~PersonalArea() { delete ui; }

void PersonalArea::DisplayArchiv() { archiv->show(); }

void PersonalArea::DisplayAuthorization() { authorization.show(); }

void PersonalArea::DisplayUploadBarcodes() { upload_barcodes->show(); }

void PersonalArea::DisplayUploadDocuments() { upload_documents->show(); }

void PersonalArea::DisplayUsers() { users->show(); }

void PersonalArea::DisplayPersonalArea() { this->show(); }

const QSharedPointer<DB::IDB> &PersonalArea::GetpDB() const { return m_pDB; }

void PersonalArea::AuthorizationUser() {

  auto [l, p] = authorization.GetValues();

  if (m_pDB->ConnectDB(HostName, DataBase, l, p)) {
    authorization.hide();
    this->show();
    Login = l;

    if (m_pDB->isOpenConnect()) {
      QSqlQuery query;
      query.setForwardOnly(true);
      query.prepare(QString("select role_ from user_ where login_= :LOGIN"));
      query.bindValue(":LOGIN", Login);

      if (query.exec()) {
        while (query.next()) {
          Role = query.value(0).toString();
        }
      } else {
        qCritical() << query.lastError().databaseText();
        qCritical() << query.lastError().driverText();
        qCritical() << query.lastError().nativeErrorCode();
        qCritical() << query.lastQuery();
      }
    } else {
      qCritical() << QString("Нет связи с БД!");
    }

    if (Role == QString("operator_")) {
      ui->PB_ADDBORCODS->hide();
      ui->PB_ARCHIV->hide();
      ui->PB_USERS->hide();
    }
  }
}

void PersonalArea::on_PB_USERS_clicked() {
  if (m_pDB->isOpenConnect())
    users->show();
  else {
    this->hide();
    authorization.show();
  }
}

void PersonalArea::on_PB_ARCHIV_clicked() {
  if (m_pDB->isOpenConnect())
    archiv->show();
  else {
    this->hide();
    authorization.show();
  }
}

void PersonalArea::on_PB_ADDBORCODS_clicked() {
  if (m_pDB->isOpenConnect())
    upload_barcodes->show();
  else {
    this->hide();
    authorization.show();
  }
}

void PersonalArea::on_PB_ADDDOCUMENTS_clicked() {
  if (m_pDB->isOpenConnect())
    upload_documents->show();
  else {
    this->hide();
    authorization.show();
  }
}

void PersonalArea::on_PB_INDEXING_clicked() {
  if (m_pDB->isOpenConnect())
    indexing->show();
  else {
    this->hide();
    authorization.show();
  }
}
