#include "uploadbarcodes.h"
#include "ui_uploadbarcodes.h"

UploadBarcodes::UploadBarcodes(const QSharedPointer<DB::IDB> &pDB,
                               QWidget *parent)
    : QDialog(parent), m_pDB(pDB), ui(new Ui::UploadBarcodes) {
  ui->setupUi(this);

  ui->CB_TYPE->addItem("BOX");
  ui->CB_TYPE->addItem("FF");
  ui->CB_TYPE->addItem("REC");

  ui->LE_FIRST->setText("0000010");
  ui->LE_SECOND->setText("00000256");
}

UploadBarcodes::~UploadBarcodes() { delete ui; }

void UploadBarcodes::on_PB_ADD_clicked() {
  QString begin = ui->LE_FIRST->text();
  QString end = ui->LE_SECOND->text();
  QString type = ui->CB_TYPE->currentText();

  if (!begin.isEmpty() && !end.isEmpty() && !type.isEmpty()) {
    int lhs = begin.toInt();
    int rhs = end.toInt();
    if (lhs <= rhs) {

      int s = begin.toInt();
      int f = end.toInt();

      QSqlQuery query;
      query.setForwardOnly(true);
      QStringList res;

      std::vector<QString> v(f - s + 1);
      std::generate(v.begin(), v.end(), [&s, &type]() {
        QString res;
        if (type == "BOX") {
          res += "'BOX00000";
        }
        if (type == "FF") {
          res += "'FF00000";
        }
        if (type == "REC") {
          res += "'REC00000000";
        }
        QString num = "";
        num = QString::number(s);
        res += num;
        res += "'";
        s++;
        return res;
      });

      if (m_pDB->isOpenConnect()) {

        QStringList ids_string;
        for (const auto &val : v)
          ids_string << val;

        QString txt = ids_string.join(",");

        if (type == "BOX") {
          query.prepare(
              QString("select id_, number_ from box_ where number_ in(%1)")
                  .arg(txt));
        }
        if (type == "FF") {
          query.prepare(
              QString("select id_, number_ from folder_ where number_ in(%1)")
                  .arg(txt));
        }
        if (type == "REC") {
          query.prepare(
              QString("select id_, number_ from record_ where number_ in(%1)")
                  .arg(txt));
        }

        if (query.exec()) {
          while (query.next()) {
            res << query.value(1).toString();
          }

          if (res.size() == 0) {
            QStringList ids_string;
            for (const auto &val : v)
              ids_string << "(" + val << "'1'"
                         << "'1'"
                         << "true)";

            QString txt = ids_string.join(",");

            if (type == "BOX") {
              query.prepare(QString("insert into box_( number_, rid_project_, "
                                    "rid_owner_, is_activ_)"
                                    "values %1")
                                .arg(txt));
            }
            if (type == "FF") {
              query.prepare(QString("insert into folder_(number_, rid_box_, "
                                    "rid_owner_, is_activ_)"
                                    "values %1")
                                .arg(txt));
            }
            if (type == "REC") {
              QStringList ids_string;
              for (const auto &val : v)
                ids_string << "(" + val << "'1'"
                           << "'1'"
                           << "'2021-07-25 12:00:00'"
                           << "'1'"
                           << "true)";
              QString txt = ids_string.join(",");

              query.prepare(
                  QString(
                      "insert into record_( number_, rid_folder_, rid_owner_, "
                      "last_date_change_, rid_who_last_date_change_, is_activ_)"
                      "values %1")
                      .arg(txt));
            }

            if (query.exec()) {
              QMessageBox::information(0, "Information",
                                       "Штрих-коды добавлены!");
              ui->LE_FIRST->clear();
              ui->LE_SECOND->clear();
            } else {
              QMessageBox::information(0, "Information", "Ошибка!");
              qCritical() << query.lastError().databaseText();
              qCritical() << query.lastError().driverText();
              qCritical() << query.lastError().nativeErrorCode();
              qCritical() << query.lastQuery();
              qCritical() << query.executedQuery();
            }

          } else {
            QString err = res.join(", ");
            QMessageBox::information(
                0, "Information",
                "Указаные номера уже есть в системе, удалите их из списка : " +
                    err);
            ui->LE_FIRST->clear();
            ui->LE_SECOND->clear();
          }

        } else {
          QMessageBox::information(0, "Information", "Ошибка!");
          qCritical() << query.lastError().databaseText();
          qCritical() << query.lastError().driverText();
          qCritical() << query.lastError().nativeErrorCode();
          qCritical() << query.lastQuery();
          qCritical() << query.executedQuery();
        }

      } else
        QMessageBox::information(0, "Information", "Нет связи с БД");

    } else
      QMessageBox::information(0, "Information", "Введен неверный диапазон!");
  } else
    QMessageBox::information(0, "Information", "Поля не заполнены!");
} // void UploadBarcodes::on_PB_ADD_clicked()
