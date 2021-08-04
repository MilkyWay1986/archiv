#include "data.h"

Data::Data(QObject *parent) : QObject(parent) {}

Data::Data(const QSqlQuery &qry, QObject *parent) : QObject(parent) {

  id_ = qry.value("id_").toUInt();
  rid_record_ = qry.value("rid_record_").toUInt();
  rid_owner_ = qry.value("rid_owner_").toUInt();
  number_document = qry.value("number_document").toString();
  data_document = qry.value("data_document").toString();
  ogrn_document = qry.value("ogrn_document").toString();
  last_date_change_ = qry.value("last_date_change_");
  rid_who_last_date_change_ = qry.value("rid_who_last_date_change_").toUInt();
  path_ = qry.value("path_").toString();
  is_activ_ = qry.value("is_activ_").toBool();
}


