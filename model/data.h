#pragma once

#include <QObject>
#include <QtSql>


class QSqlQuery;

class Data : public QObject {
  Q_OBJECT
public:
  explicit Data(QObject *parent = nullptr);
  explicit Data(const QSqlQuery &, QObject *parent = nullptr);



public:
  uint id_;
  uint rid_record_;
  uint rid_owner_;
  QString number_document;
  QString data_document;
  QString ogrn_document;
  QVariant last_date_change_;
  uint rid_who_last_date_change_;
  QString path_;
  bool is_activ_;
};
