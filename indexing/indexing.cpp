#include "indexing.h"
#include "db/configure.h"
#include "ui_indexing.h"

Indexing::Indexing(const QSharedPointer<DB::IDB> &pDB, QWidget *parent)
    : QDialog(parent), m_pDB(pDB), ui(new Ui::Indexing) {
  ui->setupUi(this);

  ui->LE_BOX->setPlaceholderText("BOX00000");
  ui->LE_FF->setPlaceholderText("FF00000");
  ui->LE_REC->setPlaceholderText("REC00000000");
  ui->LE_DATA->setInputMask("00.00.0000");
  ui->LE_OGRN->setInputMask("0000000000000");

  ui->LE_BOX->setText("BOX000002");
  ui->LE_FF->setText("FF000003");
  ui->LE_REC->setText("REC0000000014");
  ui->LE_NUMBER->setText("1");
  ui->LE_DATA->setText("12.01.2020");
  ui->LE_OGRN->setText("123456789");
}

Indexing::~Indexing() { delete ui; }

void Indexing::on_PB_ADD_clicked() {
  QString box = ui->LE_BOX->text();
  QString ff = ui->LE_FF->text();
  QString rec = ui->LE_REC->text();
  QString number = ui->LE_NUMBER->text();
  QString data = ui->LE_DATA->text();
  QString ogrn = ui->LE_OGRN->text();

  if (!box.isEmpty() && !ff.isEmpty() && !rec.isEmpty() && !number.isEmpty() &&
      !data.isEmpty() && !ogrn.isEmpty()) {

    QSqlQuery query;
    query.setForwardOnly(true);

    std::vector<int> res(3, 0);
    if (m_pDB->isOpenConnect()) {

      // BOX
      query.prepare(
          QString("select id_ from box_ where number_ = :NUMBER_BOX;"));
      query.bindValue(":NUMBER_BOX", box);

      if (query.exec()) {
        while (query.next()) {
          if (query.value(0).toInt() > 0)
            res[0] = (query.value(0).toInt());
        }

        if (res[0] == 0) {
          QMessageBox::information(0, "Information",
                                   "Ошибка! BOX с таки номером нет в системе");
          ui->LE_BOX->clear();
        } else {

          // FF
          query.prepare(
              QString("select id_ from folder_ where number_ = :NUMBER_FF;"));
          query.bindValue(":NUMBER_FF", ff);

          if (query.exec()) {
            while (query.next()) {
              if (query.value(0).toInt() > 0)
                res[1] = (query.value(0).toInt());
            }

            if (res[1] == 0) {
              QMessageBox::information(
                  0, "Information", "Ошибка! FF с таки номером нет в системе!");
              ui->LE_FF->clear();
            } else {

              // REC
              query.prepare(QString(
                  "select id_ from record_ where number_ = :NUMBER_REC;"));
              query.bindValue(":NUMBER_REC", rec);

              if (query.exec()) {
                while (query.next()) {
                  if (query.value(0).toInt() > 0)
                    res[2] = (query.value(0).toInt());
                }

                if (res[2] == 0) {
                  QMessageBox::information(
                      0, "Information",
                      "Ошибка! REC с таки номером нет в системе!");
                  ui->LE_REC->clear();
                } else {

                  if (res.size() == 3 && res[0] != 0 && res[1] != 0 &&
                      res[2] != 0) {

                    QString rid_record = QString::number(res[2]);

                    QString rid_owner;
                    query.prepare(QString("select id_ from user_ where "
                                          "login_ = :LOGIN;"));

                    query.bindValue(":LOGIN", Login);

                    if (query.exec()) {
                      while (query.next()) {
                        if (query.value(0).toInt() > 0)
                          rid_owner = query.value(0).toString();
                        else
                          QMessageBox::information(
                              0, "Information",
                              "Ошибка выполения запроса LOGIN!");
                      }
                    }

                    QDate d = QDate::currentDate();
                    QTime t = QTime::currentTime();
                    QString dt = d.toString("yyyy-MM-dd") + " " +
                                 t.toString(Qt::SystemLocaleLongDate);

                    QStringList ids_string;

                    ids_string
                        << "('" + rid_record + "'" // rid_record_
                        << "'" + rid_owner + "'"   // rid_owner_
                        << "'" + number + "'"      // number_document
                        << "'" + data + "'"        // data_document
                        << "'" + ogrn + "'"        // ogrn_document
                        << "'" + dt + "'"          // last_date_change_
                        << "'" + rid_owner + "'"   // rid_who_last_date_change_
                        << "' '"                   // path_
                        << "true)";                // is_activ_

                    QString txt = ids_string.join(",");

                    query.prepare(QString("insert into document_"
                                          "(   rid_record_,"
                                          "    rid_owner_, "
                                          "    number_document, "
                                          "    data_document,"
                                          "    ogrn_document, "
                                          "    last_date_change_,"
                                          "    rid_who_last_date_change_,"
                                          "    path_, "
                                          "   is_activ_"
                                          ") values %1")
                                      .arg(txt));

                    // TODO Нужен запрос-проверка а нет ли такой записи уже в
                    // таблие document_
                    query.prepare(
                        QString(
                            "select id_ from document_ where rid_record_= %1")
                            .arg(rid_record));
                    if (query.exec()) {
                      while (query.next()) {
                        if (query.value(0).toInt() > 0) {
                          QMessageBox::information(
                              0, "Information",
                              "Документ с таким номером REC уже есть в базе!");
                          ui->LE_REC->clear();
                        } else {
                          if (query.exec()) {
                            QMessageBox::information(0, "Information",
                                                     "Запись добавлена!");
                            ui->LE_REC->clear();
                            ui->LE_NUMBER->clear();
                            ui->LE_DATA->clear();
                            ui->LE_OGRN->clear();

                          } else {
                            QMessageBox::information(
                                0, "Information",
                                "Ошибка добавления записи в БД!");
                            qCritical() << query.lastError().databaseText();
                            qCritical() << query.lastError().driverText();
                            qCritical() << query.lastError().nativeErrorCode();
                            qCritical() << query.lastQuery();
                            qCritical() << query.executedQuery();

                            ui->LE_BOX->clear();
                            ui->LE_FF->clear();
                            ui->LE_REC->clear();
                            ui->LE_NUMBER->clear();
                            ui->LE_DATA->clear();
                            ui->LE_OGRN->clear();
                          }
                        }
                      }
                    }
                    // TODO Нужен запрос-проверка а нет ли такой записи уже в
                    // таблие document_

                  } else {
                    qDebug() << "res.size(): " << res.size() << "\n";
                    for (const auto &itm : res)
                      qDebug() << itm << "\n";
                    qCritical()
                        << "Что-то пошло нетак вектор не на 3 элемента или "
                           "содержит 0!\n";

                    ui->LE_BOX->clear();
                    ui->LE_FF->clear();
                    ui->LE_REC->clear();
                    ui->LE_NUMBER->clear();
                    ui->LE_DATA->clear();
                    ui->LE_OGRN->clear();
                  }
                }
              } else {
                QMessageBox::information(0, "Information",
                                         "Ошибка выполения запроса REC!");
                qCritical() << query.lastError().databaseText();
                qCritical() << query.lastError().driverText();
                qCritical() << query.lastError().nativeErrorCode();
                qCritical() << query.lastQuery();
                qCritical() << query.executedQuery();
              }
              // REC
            }
          } else {
            QMessageBox::information(0, "Information",
                                     "Ошибка выполения запроса FF!");
            qCritical() << query.lastError().databaseText();
            qCritical() << query.lastError().driverText();
            qCritical() << query.lastError().nativeErrorCode();
            qCritical() << query.lastQuery();
            qCritical() << query.executedQuery();
          }
          // FF
        }
      } else {
        QMessageBox::information(0, "Information",
                                 "Ошибка выполения запроса BOX!");
        qCritical() << query.lastError().databaseText();
        qCritical() << query.lastError().driverText();
        qCritical() << query.lastError().nativeErrorCode();
        qCritical() << query.lastQuery();
        qCritical() << query.executedQuery();
      }
      // BOX

    } else
      QMessageBox::information(0, "Information", "Нет связи с БД");

  } else
    QMessageBox::information(0, "Information", "Не все поля заполнены!");
}
