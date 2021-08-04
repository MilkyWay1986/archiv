#include "uploaddocuments.h"
#include "ui_uploaddocuments.h"

UploadDocuments::UploadDocuments(const QSharedPointer<DB::IDB> &pDB,
                                 QWidget *parent)
    : QDialog(parent), m_pDB(pDB), ui(new Ui::UploadDocuments) {
  ui->setupUi(this);
  ui->PB_ADD->hide();
}

static QString path_upload_files = "";

UploadDocuments::~UploadDocuments() { delete ui; }

void UploadDocuments::on_PB_EXPLORER_clicked() {
  ui->TB_LIST->clear();
  QString files = QFileDialog::getExistingDirectory();
  if (!files.isEmpty()) {
    QString list_files;
    QDir dir(files);
    QFileInfoList list = dir.entryInfoList();
    path_upload_files = dir.absolutePath();

    for (const auto &itm : list) {
      QFileInfo fileinfo = itm;
      if (fileinfo.suffix() == "pdf") {
        list_files.append(fileinfo.fileName());
        list_files.append(";\n");
      }
    }
    if (!list_files.isEmpty()) {
      ui->PB_ADD->show();
      ui->TB_LIST->setText(list_files);
    } else {
      QMessageBox::information(0, "Information",
                               "Скан-образы в формате pdf не найдены!");
    }
  }
}

void UploadDocuments::on_PB_ADD_clicked() {
  ui->PB_ADD->hide();
  QString txt = ui->TB_LIST->toPlainText();
  QStringList SL, SL2;
  SL = txt.split(";\n");

  for (const auto &itm : SL)
    SL2.append(itm.section('.', 0, 0));

  SL2.removeAll(QString(""));

  std::vector<QString> v;
  for (const auto &itm : SL2)
    v.emplace_back("'" + itm + "'");

  QSqlQuery query;
  query.setForwardOnly(true);

  if (m_pDB->isOpenConnect()) {

    QStringList ids_string;
    for (const auto &val : v)
      ids_string << val;

    QString txt = ids_string.join(",");

    query.prepare(
        QString("select id_, number_ from record_ where number_ in (%1)")
            .arg(txt));

    QStringList res;
    if (query.exec()) {
      while (query.next()) {
        res << query.value(1).toString();
      }

      QStringList res2;
      for (const auto &itm : res)
        res2 << "'" + itm + "'";

      QSet<QString> after_query = ids_string.toSet().subtract(res2.toSet());

      path_upload_files.replace("/", "\\");
      QString file_ectension = ".pdf";

      QSqlQuery query;
      query.setForwardOnly(true);

      for (const auto &itm : res) {

        QString to = path_storage + itm + file_ectension;
        QString from = path_upload_files + "\\" + itm + file_ectension;

        query.prepare("update document_ "
                      "set "
                      " path_= :PATH "
                      "where rid_record_="
                      "(select id_ from record_ where number_= :REC_NUMBER);");

        query.bindValue(":PATH", to);
        query.bindValue(":REC_NUMBER", itm);

        if (query.exec()) {
          if (QFile::exists(to)) {
            QFile::remove(to);
          } else {
            QFile::copy(from, to);
            QFile::remove(from);
          }
        } else {
          QMessageBox::information(0, "Information", "Ошибка загрузки!");
          qCritical() << query.lastError().databaseText();
          qCritical() << query.lastError().driverText();
          qCritical() << query.lastError().nativeErrorCode();
          qCritical() << query.lastQuery();
        }
      }

      {
        QStringList res;
        for (const auto &itm : after_query)
          res << itm;
        QString err = "Записи не найдены в системе:\n\n" + res.join("\n ");
        ui->TB_LIST->setText(err);
        ui->PB_ADD->hide();
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
}
