#include "archiv.h"
#include "ui_archiv.h"

Archiv::Archiv(const QSharedPointer<DB::IDB> &pDB, QWidget *parent)
    : QDialog(parent), m_pDB(pDB), m_ab_model(new ModelArchivBox(pDB, parent)),
      m_af_model(new ModelArchivFolder(pDB, parent)),
      m_ar_model(new ModelArchivRecord(pDB, parent)),
      m_an_model(new ModelArchivNumber(pDB, parent)),
      m_ao_model(new ModelArchivOgrn(pDB, parent)), ui(new Ui::Archiv) {
  ui->setupUi(this);
}

Archiv::~Archiv() {
  delete m_ab_model;
  delete m_af_model;
  delete m_ar_model;
  delete m_an_model;
  delete m_ao_model;
  delete ui;
}

void Archiv::on_PB_SEARCH_clicked() {
  ui->TV_RESULT->clearSpans();

  QString txt;

  int pole = 0;

  if (!(ui->LE_BOX->text().isEmpty())) {
    txt = ui->LE_BOX->text();
    pole = 1;
  }

  else if (!(ui->LE_FOLDER->text().isEmpty())) {
    txt = ui->LE_FOLDER->text();
    pole = 2;
  }

  else if (!(ui->LE_RECORD->text().isEmpty())) {
    txt = ui->LE_RECORD->text();
    pole = 3;
  }

  else if (!(ui->LE_NUMBER->text().isEmpty())) {
    txt = ui->LE_NUMBER->text();
    pole = 4;
  }

  else if (!(ui->LE_OGRN->text().isEmpty())) {
    txt = ui->LE_OGRN->text();
    pole = 5;
  }

  switch (pole) {

  case 1: {
    //Поиск по BOX
    QMessageBox::information(0, "Information", "Поиск по BOX");

    if (m_pDB->isOpenConnect()) {
      QSqlQuery qry;
      qry.setForwardOnly(true);
      qry.prepare("select "
                  "   box_.id_, "
                  "   box_.number_, "
                  "   project_.title, "
                  "   user_.login_, "
                  "   box_.is_activ_"
                  " from box_ "
                  "  join project_ on box_.rid_project_ = project_.id_"
                  "  join user_ on box_.rid_owner_ = user_.id_"
                  " where box_.number_ like :TEMPLATE ;");

      qry.bindValue(":TEMPLATE", QString("%%1%").arg(txt));

      if (qry.exec()) {
        while (qry.next()) {
          DataArchivBox *D = new DataArchivBox(qry, this);
          m_ab_model->AddToCat(D);
        }

        ui->TV_RESULT->setModel(m_ab_model);

      } else
        QMessageBox::information(0, "Information", "Ошибка выполения запроса!");
    } else
      QMessageBox::information(0, "Information", "Нет связи с БД");
    //Поиск по BOX
    break;
  }

  case 2: {
    //Поиск по FOLDER
    QMessageBox::information(0, "Information", "Поиск по FOLDER");

    if (m_pDB->isOpenConnect()) {
      QSqlQuery qry;
      qry.setForwardOnly(true);
      qry.prepare("select"
                  "  folder_.id_,"
                  "  box_.number_,"
                  "  folder_.number_,"
                  "  user_.login_,"
                  "  folder_.is_activ_"
                  " from folder_"
                  "  join box_ on folder_.rid_box_ = box_.id_"
                  "  join user_ on  	folder_.rid_owner_ = user_.id_"
                  " where folder_.number_ like :TEMPLATE;");

      qry.bindValue(":TEMPLATE", QString("%%1%").arg(txt));

      if (qry.exec()) {
        while (qry.next()) {
          DataArchivFolder *D = new DataArchivFolder(qry, this);
          m_af_model->AddToCat(D);
        }

        ui->TV_RESULT->setModel(m_af_model);

      } else
        QMessageBox::information(0, "Information", "Ошибка выполения запроса!");
    } else
      QMessageBox::information(0, "Information", "Нет связи с БД");
    //Поиск по FOLDER
    break;
  }
  case 3: {
    //Поиск по RECORD
    QMessageBox::information(0, "Information", "Поиск по RECORD");

    if (m_pDB->isOpenConnect()) {
      QSqlQuery qry;
      qry.setForwardOnly(true);
      qry.prepare("select "
                  "  record_.id_,"
                  "  folder_.number_, "
                  "  record_.number_,"
                  "  user_.login_,"
                  "  record_.last_date_change_,"
                  "  user_.login_,"
                  "  record_.is_activ_"
                  " from record_"
                  "  join folder_ on record_.rid_folder_ = folder_.id_"
                  "  join user_ on record_.rid_owner_ = user_.id_"
                  " where record_.number_ like :TEMPLATE;");

      qry.bindValue(":TEMPLATE", QString("%%1%").arg(txt));

      if (qry.exec()) {
        while (qry.next()) {
          DataArchivRecord *D = new DataArchivRecord(qry, this);
          m_ar_model->AddToCat(D);
        }

        ui->TV_RESULT->setModel(m_ar_model);

      } else
        QMessageBox::information(0, "Information", "Ошибка выполения запроса!");
    } else
      QMessageBox::information(0, "Information", "Нет связи с БД");
    //Поиск по RECORD
    break;
  }
  case 4: {
    //Поиск по NUMBER
    QMessageBox::information(0, "Information", "Поиск по NUMBER");

    if (m_pDB->isOpenConnect()) {
      QSqlQuery qry;
      qry.setForwardOnly(true);
      qry.prepare("select  "
                  "  document_.id_,"
                  "  record_.number_,"
                  "  user_.login_,"
                  "  document_.number_document,"
                  "  document_.data_document,	"
                  "  document_.ogrn_document,"
                  "  document_.last_date_change_,"
                  "  user_.login_,"
                  "  document_.path_,"
                  "  document_.is_activ_ "
                  " from document_ "
                  "  join record_ on document_.rid_record_ = record_.id_ "
                  "  join user_ on document_.rid_owner_ =user_.id_ "
                  " where document_.number_document like :TEMPLATE;");

      qry.bindValue(":TEMPLATE", QString("%%1%").arg(txt));

      if (qry.exec()) {
        while (qry.next()) {
          DataArchivNumber *D = new DataArchivNumber(qry, this);
          m_an_model->AddToCat(D);
        }
        ui->TV_RESULT->setModel(m_an_model);

      } else
        QMessageBox::information(0, "Information", "Ошибка выполения запроса!");
    } else
      QMessageBox::information(0, "Information", "Нет связи с БД");
    //Поиск по NUMBER
    break;
  }
  case 5: {
    //Поиск по OGRN
    QMessageBox::information(0, "Information", "Поиск по OGRN");

    if (m_pDB->isOpenConnect()) {
      QSqlQuery qry;
      qry.setForwardOnly(true);
      qry.prepare("select  "
                  "  document_.id_,"
                  "  record_.number_,"
                  "  user_.login_,"
                  "  document_.number_document,"
                  "  document_.data_document,"
                  "  document_.ogrn_document,"
                  "  document_.last_date_change_,"
                  "  user_.login_,"
                  "  document_.path_,"
                  "  document_.is_activ_"
                  " from document_"
                  "  join record_  on document_.rid_record_ = record_.id_"
                  "  join user_ 	  on document_.rid_owner_=user_.id_"
                  " where document_.ogrn_document like :TEMPLATE;");

       qry.bindValue(":TEMPLATE", QString("%%1%").arg(txt));

      if (qry.exec()) {
        while (qry.next()) {
          DataArchivOgrn *D = new DataArchivOgrn(qry, this);
          m_ao_model->AddToCat(D);
        }

        ui->TV_RESULT->setModel(m_ao_model);

      } else
        QMessageBox::information(0, "Information", "Ошибка выполения запроса!");
    } else
      QMessageBox::information(0, "Information", "Нет связи с БД");
    //Поиск по OGRN
  }
  }
}
