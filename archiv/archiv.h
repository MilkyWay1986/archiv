#pragma once

#include <QDialog>
#include <QMessageBox>
#include <QString>
#include <QTableView>

#include "db/IDB.h"
#include "model/model_archiv.h"

namespace Ui {
class Archiv;
}

class Archiv : public QDialog {
  Q_OBJECT

public:
  explicit Archiv(const QSharedPointer<DB::IDB> &, QWidget *parent = nullptr);
  ~Archiv();

protected:
  const QSharedPointer<DB::IDB> &m_pDB;
  // Model *m_model;
  ModelArchivBox *m_ab_model;
  ModelArchivFolder *m_af_model;
  ModelArchivRecord *m_ar_model;
  ModelArchivNumber *m_an_model;
  ModelArchivOgrn *m_ao_model;

private slots:
  void on_PB_SEARCH_clicked();

private:
  Ui::Archiv *ui;
};
