#include "users.h"
#include "ui_users.h"

Users::Users(const QSharedPointer<DB::IDB> &pDB, QWidget *parent)
    : QDialog(parent), m_pDB(pDB), m_model_user(new ModelUser(pDB, parent)),
      ui(new Ui::Users) {

  ui->setupUi(this);

  ui->CB_ROLE->addItem("operator_");
  ui->CB_ROLE->addItem("manager_");
  //ui->CB_ROLE->addItem("admin_");
}

Users::~Users() {
  delete m_model_user;
  delete ui;
}

void Users::on_PB_ADDUSER_clicked() {

  QString login = ui->LE_LOGIN->text().trimmed();
  QString password = ui->LE_PASSWORD->text().trimmed();
  QString role = ui->CB_ROLE->currentText();

  QSqlQuery query;
  QSqlRecord rec;
  int id_box = -1;
  if (m_pDB->isOpenConnect()) {
    query.prepare("select id_ from user_ where login_ = :LOGIN;");
    query.bindValue(":LOGIN", login);

    if (query.exec())
      rec = query.record();

    if (query.next())
      id_box = query.value(rec.indexOf("id_")).toInt();
  } else
    QMessageBox::information(0, "Information", "Нет связи с БД");

  if (id_box > 0) {
    QMessageBox::information(0, "Information", "Такой логин уже существует!");
  } else {
    if (m_pDB->isOpenConnect()) {
      QSqlQuery query;
      query.prepare("insert into user_ (login_, password_,role_,is_activ_) "
                    "values(:LOGIN, :PASSWORD, :ROLE, :IS_ACTIV);");

      query.bindValue(":LOGIN", login);
      query.bindValue(":PASSWORD", password);
      query.bindValue(":ROLE", role);
      query.bindValue(":IS_ACTIV", true);

      if (query.exec()) {
        QMessageBox::information(0, "Information", "Пользователь добавлен!");
      } else {
        QMessageBox::information(0, "Information", "Ошибка выполения запроса!");
        qCritical() << query.lastError().databaseText();
        qCritical() << query.lastError().driverText();
        qCritical() << query.lastError().nativeErrorCode();
        qCritical() << query.lastQuery();
      }

    } else {
      QMessageBox::information(0, "Information", "Нет связи с БД");
      qCritical() << query.lastError().databaseText();
      qCritical() << query.lastError().driverText();
      qCritical() << query.lastError().nativeErrorCode();
    }
  }
}

void Users::on_PB_SEARCH_clicked() {

  //Поиск по USERS
  QString txt = ui->LE_SEARCH->text();
  QMessageBox::information(0, "Information", "Поиск Пользователей");

  if (m_pDB->isOpenConnect()) {
    QSqlQuery qry;
    qry.setForwardOnly(true);
    qry.prepare("select"
                "  id_,"
                "  login_,"
                "  password_, "
                "  role_, "
                "  is_activ_"
                " from user_"
                "  where login_ like :TEMPLATE;");

    qry.bindValue(":TEMPLATE", QString("%%1%").arg(txt));

    if (qry.exec()) {
      while (qry.next()) {
        DataModelUser *D = new DataModelUser(qry, this);
        m_model_user->AddToCat(D);
      }

      ui->TV_RESULT->setModel(m_model_user);

    } else
      QMessageBox::information(0, "Information", "Ошибка выполения запроса!");
  } else
    QMessageBox::information(0, "Information", "Нет связи с БД");
  //Поиск по USERS
}
