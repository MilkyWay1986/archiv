#include "model_archiv.h"

/*------------------------------------------------------*/
DataArchivBox::DataArchivBox(QObject *parent) : QObject(parent) {}

DataArchivBox::DataArchivBox(const QSqlQuery &qry, QObject *parent)
    : QObject(parent) {

  id_ = qry.value("box_.id_").toUInt();
  number_ = qry.value("box_.number_").toString();
  title = qry.value("project_.title").toString();
  login_ = qry.value("user_.login_").toString();
  is_activ_ = qry.value("box_.is_activ_").toBool();
}
//----------------------------------------
ModelArchivBox::ModelArchivBox(const QSharedPointer<DB::IDB> &pDB,
                               QObject *parent)
    : QAbstractTableModel(parent), m_pDB(pDB) {
  QSqlTableModel::EditStrategy(OnManualSubmit);
}

int ModelArchivBox::rowCount(const QModelIndex &I) const {
  if (!I.isValid()) {
    return records.size();
  } else
    return 0;
}
int ModelArchivBox::columnCount(const QModelIndex &I) const {
  if (!I.isValid()) {
    return 5;
  } else
    return 0;
}
QVariant ModelArchivBox::data(const QModelIndex &I, int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();
  DataArchivBox *D = records[I.row()];
  switch (I.column()) {
  case 0:
    return D->id_;
  case 1:
    return D->number_;
  case 2:
    return D->title;
  case 3:
    return D->login_;
  case 4:
    return D->is_activ_;
  default:
    return QVariant();
  }
  return QVariant();
}
QVariant ModelArchivBox::headerData(int section, Qt::Orientation orientation,
                                    int role) const {
  if (orientation != Qt::Horizontal)
    return QAbstractTableModel::headerData(section, orientation, role);

  switch (role) {
  case Qt::DisplayRole:
    switch (section) {
    case 0:
      return tr("id");
    case 1:
      return tr("Номер Короба(BOX)");
    case 2:
      return tr("Проект");
    case 3:
      return tr("Внес короб в систему");
    case 4:
      return tr("Активный короб(BOX)");
    default:
      return QVariant();
    }
  case Qt::TextAlignmentRole:
    return QVariant(Qt::AlignBaseline | Qt::AlignHCenter);

  default:
    return QVariant();
  }

  return QVariant();
}
bool ModelArchivBox::setData(const QModelIndex &I, const QVariant &V,
                             int role) {

  if (role == Qt::EditRole) {

    beginResetModel();

    switch (I.column()) {
    case 1: {
      break;
    }
    case 2: {
      QSqlQuery query;
      QSqlRecord rec;
      auto txt = qvariant_cast<QString>(V);
      int id_project = -1;
      query.prepare("select id_ from project_ where title = :TITLE_PROJECT;");
      query.bindValue(":TITLE_PROJECT", txt);

      if (query.exec())
        rec = query.record();

      if (query.next())
        id_project = query.value(rec.indexOf("id_")).toInt();

      if (id_project > 0) {

        records.at(I.row())->title = qvariant_cast<QString>(V);

        QSqlQuery UPD;
        UPD.setForwardOnly(true);
        UPD.prepare("update box_ set"
                    " rid_project_ = :PROJECT_ID"
                    " where id_ = :IID;");

        UPD.bindValue(":PROJECT_ID", id_project);
        UPD.bindValue(":IID", records[I.row()]->id_);

        if (UPD.exec()) {
          QMessageBox::information(0, "Information", "Обновлено!");
          return true;
        } else {
          QMessageBox::information(0, "Information", "Ошибка!");
          qCritical() << UPD.lastError().databaseText();
          qCritical() << UPD.lastError().driverText();
          qCritical() << UPD.lastError().nativeErrorCode();
          return false;
        }

        break;
      } else {
        QMessageBox::information(0, "Information",
                                 "Такого проекта нет в системе!");
        break;
      }
    }
    case 3: {
      break;
    }
    case 4: {
      records.at(I.row())->is_activ_ = qvariant_cast<bool>(V);

      QSqlQuery UPD;
      UPD.setForwardOnly(true);
      UPD.prepare("update box_ set"
                  " is_activ_ = :ISACTIV"
                  " where id_ = :IID;");

      UPD.bindValue(":ISACTIV", records[I.row()]->is_activ_);
      UPD.bindValue(":IID", records[I.row()]->id_);

      if (UPD.exec()) {
        QMessageBox::information(0, "Information", "Изменения внесены!");
        return true;
      } else {
        QMessageBox::information(0, "Information", "Ошибка!");
        qCritical() << UPD.lastError().databaseText();
        qCritical() << UPD.lastError().driverText();
        qCritical() << UPD.lastError().nativeErrorCode();
        return false;
      }

      break;
    }
    }

    endResetModel();
    return true;
  }

  return false;
}
Qt::ItemFlags ModelArchivBox::flags(const QModelIndex &I) const {
  if (!I.isValid())
    return Qt::NoItemFlags;
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void ModelArchivBox::AddToCat(DataArchivBox *data) { records.append(data); }
/*------------------------------------------------------*/
DataArchivFolder::DataArchivFolder(QObject *parent) : QObject(parent) {}

DataArchivFolder::DataArchivFolder(const QSqlQuery &qry, QObject *parent)
    : QObject(parent) {

  id_ = qry.value("folder_.id_").toUInt();
  bnumber_ = qry.value("box_.number_").toString();
  fnumber_ = qry.value("folder_.number_").toString();
  login_ = qry.value("user_.login_").toString();
  is_activ_ = qry.value("folder_.is_activ_").toBool();
}
//----------------------------------------
ModelArchivFolder::ModelArchivFolder(const QSharedPointer<DB::IDB> &pDB,
                                     QObject *parent)
    : QAbstractTableModel(parent), m_pDB(pDB) {
  QSqlTableModel::EditStrategy(OnManualSubmit);
}

int ModelArchivFolder::rowCount(const QModelIndex &I) const {
  if (!I.isValid()) {
    return records.size();
  } else
    return 0;
}
int ModelArchivFolder::columnCount(const QModelIndex &I) const {
  if (!I.isValid()) {
    return 5;
  } else
    return 0;
}
QVariant ModelArchivFolder::data(const QModelIndex &I, int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();
  DataArchivFolder *D = records[I.row()];
  switch (I.column()) {
  case 0:
    return D->id_;
  case 1:
    return D->bnumber_;
  case 2:
    return D->fnumber_;
  case 3:
    return D->login_;
  case 4:
    return D->is_activ_;
  default:
    return QVariant();
  }
  return QVariant();
}
QVariant ModelArchivFolder::headerData(int section, Qt::Orientation orientation,
                                       int role) const {
  if (orientation != Qt::Horizontal)
    return QAbstractTableModel::headerData(section, orientation, role);

  switch (role) {
  case Qt::DisplayRole:
    switch (section) {
    case 1:
      return tr("Номер короба(BOX)");
    case 2:
      return tr("Номер папки(FOLDER)");
    case 3:
      return tr("Внес папку в систему");
    case 4:
      return tr("Активный");
    default:
      return QVariant();
    }
  case Qt::TextAlignmentRole:
    return QVariant(Qt::AlignBaseline | Qt::AlignHCenter);

  default:
    return QVariant();
  }

  return QVariant();
}
bool ModelArchivFolder::setData(const QModelIndex &I, const QVariant &V,
                                int role) {
  if (role == Qt::EditRole) {

    beginResetModel();

    switch (I.column()) {
    case 1: {

      QSqlQuery query;
      QSqlRecord rec;
      auto txt = qvariant_cast<QString>(V);
      int id_box = -1;
      query.prepare("select id_ from box_ where number_ = :NUMBER_BOX;");
      query.bindValue(":NUMBER_BOX", txt);

      if (query.exec())
        rec = query.record();

      if (query.next())
        id_box = query.value(rec.indexOf("id_")).toInt();

      if (id_box > 0) {

        records.at(I.row())->bnumber_ = qvariant_cast<QString>(V);

        QSqlQuery UPD;
        UPD.setForwardOnly(true);
        UPD.prepare("update folder_ set"
                    " rid_box_ = :BOX_ID"
                    " where id_ = :IID;");

        UPD.bindValue(":BOX_ID", id_box);
        UPD.bindValue(":IID", records[I.row()]->id_);

        if (UPD.exec()) {
          QMessageBox::information(0, "Information", "Обновлено!");
          return true;
        } else {
          QMessageBox::information(0, "Information", "Ошибка!");
          qCritical() << UPD.lastError().databaseText();
          qCritical() << UPD.lastError().driverText();
          qCritical() << UPD.lastError().nativeErrorCode();
          return false;
        }

        break;
      } else {
        QMessageBox::information(0, "Information",
                                 "Такого короба нет в системе!");
        break;
      }
    }
    case 2: {
      break;
    }
    case 3: {
      break;
    }
    case 4: {
      records.at(I.row())->is_activ_ = qvariant_cast<bool>(V);

      QSqlQuery UPD;
      UPD.setForwardOnly(true);
      UPD.prepare("update folder_ set"
                  " is_activ_ = :ISACTIV"
                  " where id_ = :IID;");

      UPD.bindValue(":ISACTIV", records[I.row()]->is_activ_);
      UPD.bindValue(":IID", records[I.row()]->id_);

      if (UPD.exec()) {
        QMessageBox::information(0, "Information", "Изменения внесены!");
        return true;
      } else {
        QMessageBox::information(0, "Information", "Ошибка!");
        qCritical() << UPD.lastError().databaseText();
        qCritical() << UPD.lastError().driverText();
        qCritical() << UPD.lastError().nativeErrorCode();
        return false;
      }

      break;
    }
    }

    endResetModel();
    return true;
  }

  return false;
}
Qt::ItemFlags ModelArchivFolder::flags(const QModelIndex &I) const {
  if (!I.isValid())
    return Qt::NoItemFlags;
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void ModelArchivFolder::AddToCat(DataArchivFolder *data) {
  records.append(data);
}
/*------------------------------------------------------*/
DataArchivRecord::DataArchivRecord(QObject *parent) : QObject(parent) {}

DataArchivRecord::DataArchivRecord(const QSqlQuery &qry, QObject *parent)
    : QObject(parent) {

  id_ = qry.value("record_.id_").toUInt();
  fnumber_ = qry.value("folder_.number_").toString();
  rnumber_ = qry.value("record_.number_").toString();
  owner_login_ = qry.value("user_.login_").toString();
  last_date_change_ = qry.value("record_.last_date_change_");
  last_change_login_ = qry.value("user_.login_").toString();
  is_activ_ = qry.value("record_.is_activ_").toBool();
}
//----------------------------------------
ModelArchivRecord::ModelArchivRecord(const QSharedPointer<DB::IDB> &pDB,
                                     QObject *parent)
    : QAbstractTableModel(parent), m_pDB(pDB) {
  QSqlTableModel::EditStrategy(OnManualSubmit);
}

int ModelArchivRecord::rowCount(const QModelIndex &I) const {
  if (!I.isValid()) {
    return records.size();
  } else
    return 0;
}
int ModelArchivRecord::columnCount(const QModelIndex &I) const {
  if (!I.isValid()) {
    return 7;
  } else
    return 0;
}
QVariant ModelArchivRecord::data(const QModelIndex &I, int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();
  DataArchivRecord *D = records[I.row()];
  switch (I.column()) {
  case 0:
    return D->id_;
  case 1:
    return D->fnumber_;
  case 2:
    return D->rnumber_;
  case 3:
    return D->owner_login_;
  case 4:
    return D->last_date_change_;
  case 5:
    return D->last_change_login_;
  case 6:
    return D->is_activ_;
  default:
    return QVariant();
  }
  return QVariant();
}
QVariant ModelArchivRecord::headerData(int section, Qt::Orientation orientation,
                                       int role) const {
  if (orientation != Qt::Horizontal)
    return QAbstractTableModel::headerData(section, orientation, role);

  switch (role) {
  case Qt::DisplayRole:
    switch (section) {
    case 1:
      return tr("Номер папки(FOLDER)");
    case 2:
      return tr("Номер записи(RECORD)");
    case 3:
      return tr("Внес запись в систему");
    case 4:
      return tr("Дата последнего измнения");
    case 5:
      return tr("Кто последний изменял запись");
    case 6:
      return tr("Активный");
    default:
      return QVariant();
    }
  case Qt::TextAlignmentRole:
    return QVariant(Qt::AlignBaseline | Qt::AlignHCenter);

  default:
    return QVariant();
  }

  return QVariant();
}
bool ModelArchivRecord::setData(const QModelIndex &I, const QVariant &V,
                                int role) {
  if (role == Qt::EditRole) {

    beginResetModel();

    switch (I.column()) {
    case 1: {

      QSqlQuery query;
      QSqlRecord rec;
      auto txt = qvariant_cast<QString>(V);
      int id_folder = -1;
      query.prepare("select id_ from folder_ where number_ = :NUMBER_FOLDER;");
      query.bindValue(":NUMBER_FOLDER", txt);

      if (query.exec())
        rec = query.record();

      if (query.next())
        id_folder = query.value(rec.indexOf("id_")).toInt();

      if (id_folder > 0) {

        records.at(I.row())->fnumber_ = qvariant_cast<QString>(V);

        QSqlQuery UPD;
        UPD.setForwardOnly(true);
        UPD.prepare("update record_ set"
                    " rid_folder_ = :FOLDER_ID"
                    " where id_ = :IID;");

        UPD.bindValue(":FOLDER_ID", id_folder);
        UPD.bindValue(":IID", records[I.row()]->id_);

        if (UPD.exec()) {
          QMessageBox::information(0, "Information", "Обновлено!");
          return true;
        } else {
          QMessageBox::information(0, "Information", "Ошибка!");
          qCritical() << UPD.lastError().databaseText();
          qCritical() << UPD.lastError().driverText();
          qCritical() << UPD.lastError().nativeErrorCode();
          return false;
        }

        break;
      } else {
        QMessageBox::information(0, "Information",
                                 "Такой папки нет в системе!");
        break;
      }
      break;
    }
    case 2: {
      break;
    }
    case 3: {
      break;
    }
    case 4: {
      break;
    }
    case 5: {
      break;
    }
    case 6: {
      records.at(I.row())->is_activ_ = qvariant_cast<bool>(V);

      QSqlQuery UPD;
      UPD.setForwardOnly(true);
      UPD.prepare("update record_ set"
                  " is_activ_ = :ISACTIV"
                  " where id_ = :IID;");

      UPD.bindValue(":ISACTIV", records[I.row()]->is_activ_);
      UPD.bindValue(":IID", records[I.row()]->id_);

      if (UPD.exec()) {
        QMessageBox::information(0, "Information", "Изменения внесены!");
        return true;
      } else {
        QMessageBox::information(0, "Information", "Ошибка!");
        qCritical() << UPD.lastError().databaseText();
        qCritical() << UPD.lastError().driverText();
        qCritical() << UPD.lastError().nativeErrorCode();
        return false;
      }

      break;
    }
    }

    endResetModel();
    return true;
  }

  return false;
}
Qt::ItemFlags ModelArchivRecord::flags(const QModelIndex &I) const {
  if (!I.isValid())
    return Qt::NoItemFlags;
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void ModelArchivRecord::AddToCat(DataArchivRecord *data) {
  records.append(data);
}
/*------------------------------------------------------*/
DataArchivNumber::DataArchivNumber(QObject *parent) : QObject(parent) {}

DataArchivNumber::DataArchivNumber(const QSqlQuery &qry, QObject *parent)
    : QObject(parent) {

  id_ = qry.value("document_.id_").toUInt();
  rnumber_ = qry.value("record_.number_").toString();
  owner_login_ = qry.value("user_.login_").toString();
  number_document = qry.value("document_.number_document").toString();
  data_document = qry.value("document_.data_document").toString();
  ogrn_document = qry.value("document_.ogrn_document").toString();
  last_date_change_ = qry.value("document_.last_date_change_");
  last_change_login_ = qry.value("user_.login_").toString();
  path_ = qry.value("document_.path_").toString();
  is_activ_ = qry.value("document_.is_activ_").toBool();
}
//----------------------------------------
ModelArchivNumber::ModelArchivNumber(const QSharedPointer<DB::IDB> &pDB,
                                     QObject *parent)
    : QAbstractTableModel(parent), m_pDB(pDB) {
  QSqlTableModel::EditStrategy(OnManualSubmit);
}

int ModelArchivNumber::rowCount(const QModelIndex &I) const {
  if (!I.isValid()) {
    return records.size();
  } else
    return 0;
}
int ModelArchivNumber::columnCount(const QModelIndex &I) const {
  if (!I.isValid()) {
    return 10;
  } else
    return 0;
}
QVariant ModelArchivNumber::data(const QModelIndex &I, int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();
  DataArchivNumber *D = records[I.row()];
  switch (I.column()) {
  case 0:
    return D->id_;
  case 1:
    return D->rnumber_;
  case 2:
    return D->owner_login_;
  case 3:
    return D->number_document;
  case 4:
    return D->data_document;
  case 5:
    return D->ogrn_document;
  case 6:
    return D->last_date_change_;
  case 7:
    return D->last_change_login_;
  case 8:
    return D->path_;
  case 9:
    return D->is_activ_;
  default:
    return QVariant();
  }
  return QVariant();
}
QVariant ModelArchivNumber::headerData(int section, Qt::Orientation orientation,
                                       int role) const {
  if (orientation != Qt::Horizontal)
    return QAbstractTableModel::headerData(section, orientation, role);

  switch (role) {
  case Qt::DisplayRole:
    switch (section) {
    case 1:
      return tr("Номер записи(RECORD)");
    case 2:
      return tr("Внес запись в систему");
    case 3:
      return tr("Номер документа");
    case 4:
      return tr("Дата документа");
    case 5:
      return tr("ОГРН");
    case 6:
      return tr("Дата последнего изменения");
    case 7:
      return tr("Кто последний изменял запись");
    case 8:
      return tr("Путь к файлу");
    case 9:
      return tr("Активный");
    default:
      return QVariant();
    }
  case Qt::TextAlignmentRole:
    return QVariant(Qt::AlignBaseline | Qt::AlignHCenter);

  default:
    return QVariant();
  }

  return QVariant();
}
bool ModelArchivNumber::setData(const QModelIndex &I, const QVariant &V,
                                int role) {
  if (role == Qt::EditRole) {

    beginResetModel();

    switch (I.column()) {
    case 1: {
      break;
    }
    case 2: {
      break;
    }
    case 3: {

      records.at(I.row())->number_document = qvariant_cast<QString>(V);

      QSqlQuery UPD;
      UPD.setForwardOnly(true);
      UPD.prepare("update document_ set"
                  " number_document = :NUMBER_DOC"
                  " where id_ = :IID;");

      UPD.bindValue(":NUMBER_DOC", records[I.row()]->number_document);
      UPD.bindValue(":IID", records[I.row()]->id_);

      if (UPD.exec()) {
        QMessageBox::information(0, "Information", "Обновлено!");
        return true;
      } else {
        QMessageBox::information(0, "Information", "Ошибка!");
        qCritical() << UPD.lastError().databaseText();
        qCritical() << UPD.lastError().driverText();
        qCritical() << UPD.lastError().nativeErrorCode();
        return false;
      }

      break;
    }
    case 4: {
      records.at(I.row())->data_document = qvariant_cast<QString>(V);

      QSqlQuery UPD;
      UPD.setForwardOnly(true);
      UPD.prepare("update document_ set"
                  " data_document = :DATA_DOC"
                  " where id_ = :IID;");

      UPD.bindValue(":DATA_DOC", records[I.row()]->data_document);
      UPD.bindValue(":IID", records[I.row()]->id_);

      if (UPD.exec()) {
        QMessageBox::information(0, "Information", "Обновлено!");
        return true;
      } else {
        QMessageBox::information(0, "Information", "Ошибка!");
        qCritical() << UPD.lastError().databaseText();
        qCritical() << UPD.lastError().driverText();
        qCritical() << UPD.lastError().nativeErrorCode();
        return false;
      }
      break;
    }
    case 5: {
      records.at(I.row())->ogrn_document = qvariant_cast<QString>(V);

      QSqlQuery UPD;
      UPD.setForwardOnly(true);
      UPD.prepare("update document_ set"
                  " ogrn_document = :OGRN_DOC"
                  " where id_ = :IID;");

      UPD.bindValue(":OGRN_DOC", records[I.row()]->ogrn_document);
      UPD.bindValue(":IID", records[I.row()]->id_);

      if (UPD.exec()) {
        QMessageBox::information(0, "Information", "Обновлено!");
        return true;
      } else {
        QMessageBox::information(0, "Information", "Ошибка!");
        qCritical() << UPD.lastError().databaseText();
        qCritical() << UPD.lastError().driverText();
        qCritical() << UPD.lastError().nativeErrorCode();
        return false;
      }
      break;
    }
    case 6: {
      break;
    }
    case 7: {
      break;
    }
    case 8: {
      break;
    }
    case 9: {
      records.at(I.row())->is_activ_ = qvariant_cast<bool>(V);

      QSqlQuery UPD;
      UPD.setForwardOnly(true);
      UPD.prepare("update document_ set"
                  " is_activ_ = :ISACTIV"
                  " where id_ = :IID;");

      UPD.bindValue(":ISACTIV", records[I.row()]->is_activ_);
      UPD.bindValue(":IID", records[I.row()]->id_);

      if (UPD.exec()) {
        QMessageBox::information(0, "Information", "Изменения внесены!");
        return true;
      } else {
        QMessageBox::information(0, "Information", "Ошибка!");
        qCritical() << UPD.lastError().databaseText();
        qCritical() << UPD.lastError().driverText();
        qCritical() << UPD.lastError().nativeErrorCode();
        return false;
      }
      break;
    }
    }

    endResetModel();
    return true;
  }

  return false;
}
Qt::ItemFlags ModelArchivNumber::flags(const QModelIndex &I) const {
  if (!I.isValid())
    return Qt::NoItemFlags;
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void ModelArchivNumber::AddToCat(DataArchivNumber *data) {
  records.append(data);
}
/*------------------------------------------------------*/
DataArchivOgrn::DataArchivOgrn(QObject *parent) : QObject(parent) {}

DataArchivOgrn::DataArchivOgrn(const QSqlQuery &qry, QObject *parent)
    : QObject(parent) {

  id_ = qry.value("document_.id_").toUInt();
  rnumber_ = qry.value("record_.number_").toString();
  owner_login_ = qry.value("user_.login_").toString();
  number_document = qry.value("document_.number_document").toString();
  data_document = qry.value("document_.data_document").toString();
  ogrn_document = qry.value("document_.ogrn_document").toString();
  last_date_change_ = qry.value("document_.last_date_change_");
  last_change_login_ = qry.value("user_.login_").toString();
  path_ = qry.value("document_.path_").toString();
  is_activ_ = qry.value("document_.is_activ_").toBool();
}
//----------------------------------------
ModelArchivOgrn::ModelArchivOgrn(const QSharedPointer<DB::IDB> &pDB,
                                 QObject *parent)
    : QAbstractTableModel(parent), m_pDB(pDB) {
  QSqlTableModel::EditStrategy(OnManualSubmit);
}

int ModelArchivOgrn::rowCount(const QModelIndex &I) const {
  if (!I.isValid()) {
    return records.size();
  } else
    return 0;
}
int ModelArchivOgrn::columnCount(const QModelIndex &I) const {
  if (!I.isValid()) {
    return 10;
  } else
    return 0;
}
QVariant ModelArchivOgrn::data(const QModelIndex &I, int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();
  DataArchivOgrn *D = records[I.row()];
  switch (I.column()) {
  case 0:
    return D->id_;
  case 1:
    return D->rnumber_;
  case 2:
    return D->owner_login_;
  case 3:
    return D->number_document;
  case 4:
    return D->data_document;
  case 5:
    return D->ogrn_document;
  case 6:
    return D->last_date_change_;
  case 7:
    return D->last_change_login_;
  case 8:
    return D->path_;
  case 9:
    return D->is_activ_;
  default:
    return QVariant();
  }
  return QVariant();
}
QVariant ModelArchivOgrn::headerData(int section, Qt::Orientation orientation,
                                     int role) const {
  if (orientation != Qt::Horizontal)
    return QAbstractTableModel::headerData(section, orientation, role);

  switch (role) {
  case Qt::DisplayRole:
    switch (section) {
    case 1:
      return tr("Номер записи(RECORD)");
    case 2:
      return tr("Внес запись в систему");
    case 3:
      return tr("Номер документа");
    case 4:
      return tr("Дата документа");
    case 5:
      return tr("ОГРН");
    case 6:
      return tr("Дата последнего изменения");
    case 7:
      return tr("Кто последний изменял запись");
    case 8:
      return tr("Путь к файлу");
    case 9:
      return tr("Активный");
    default:
      return QVariant();
    }
  case Qt::TextAlignmentRole:
    return QVariant(Qt::AlignBaseline | Qt::AlignHCenter);

  default:
    return QVariant();
  }

  return QVariant();
}
bool ModelArchivOgrn::setData(const QModelIndex &I, const QVariant &V,
                              int role) {
  if (role == Qt::EditRole) {

    beginResetModel();

    switch (I.column()) {
    case 1: {
      break;
    }
    case 2: {
      break;
    }
    case 3: {

      records.at(I.row())->number_document = qvariant_cast<QString>(V);

      QSqlQuery UPD;
      UPD.setForwardOnly(true);
      UPD.prepare("update document_ set"
                  " number_document = :NUMBER_DOC"
                  " where id_ = :IID;");

      UPD.bindValue(":NUMBER_DOC", records[I.row()]->number_document);
      UPD.bindValue(":IID", records[I.row()]->id_);

      if (UPD.exec()) {
        QMessageBox::information(0, "Information", "Обновлено!");
        return true;
      } else {
        QMessageBox::information(0, "Information", "Ошибка!");
        qCritical() << UPD.lastError().databaseText();
        qCritical() << UPD.lastError().driverText();
        qCritical() << UPD.lastError().nativeErrorCode();
        return false;
      }

      break;
    }
    case 4: {
      records.at(I.row())->data_document = qvariant_cast<QString>(V);

      QSqlQuery UPD;
      UPD.setForwardOnly(true);
      UPD.prepare("update document_ set"
                  " data_document = :DATA_DOC"
                  " where id_ = :IID;");

      UPD.bindValue(":DATA_DOC", records[I.row()]->data_document);
      UPD.bindValue(":IID", records[I.row()]->id_);

      if (UPD.exec()) {
        QMessageBox::information(0, "Information", "Обновлено!");
        return true;
      } else {
        QMessageBox::information(0, "Information", "Ошибка!");
        qCritical() << UPD.lastError().databaseText();
        qCritical() << UPD.lastError().driverText();
        qCritical() << UPD.lastError().nativeErrorCode();
        return false;
      }
      break;
    }
    case 5: {
      records.at(I.row())->ogrn_document = qvariant_cast<QString>(V);

      QSqlQuery UPD;
      UPD.setForwardOnly(true);
      UPD.prepare("update document_ set"
                  " ogrn_document = :OGRN_DOC"
                  " where id_ = :IID;");

      UPD.bindValue(":OGRN_DOC", records[I.row()]->ogrn_document);
      UPD.bindValue(":IID", records[I.row()]->id_);

      if (UPD.exec()) {
        QMessageBox::information(0, "Information", "Обновлено!");
        return true;
      } else {
        QMessageBox::information(0, "Information", "Ошибка!");
        qCritical() << UPD.lastError().databaseText();
        qCritical() << UPD.lastError().driverText();
        qCritical() << UPD.lastError().nativeErrorCode();
        return false;
      }
      break;
    }
    case 6: {
      break;
    }
    case 7: {
      break;
    }
    case 8: {
      break;
    }
    case 9: {
      records.at(I.row())->is_activ_ = qvariant_cast<bool>(V);

      QSqlQuery UPD;
      UPD.setForwardOnly(true);
      UPD.prepare("update document_ set"
                  " is_activ_ = :ISACTIV"
                  " where id_ = :IID;");

      UPD.bindValue(":ISACTIV", records[I.row()]->is_activ_);
      UPD.bindValue(":IID", records[I.row()]->id_);

      if (UPD.exec()) {
        QMessageBox::information(0, "Information", "Изменения внесены!");
        return true;
      } else {
        QMessageBox::information(0, "Information", "Ошибка!");
        qCritical() << UPD.lastError().databaseText();
        qCritical() << UPD.lastError().driverText();
        qCritical() << UPD.lastError().nativeErrorCode();
        return false;
      }
      break;
    }
    }

    endResetModel();
    return true;
  }

  return false;
}
Qt::ItemFlags ModelArchivOgrn::flags(const QModelIndex &I) const {
  if (!I.isValid())
    return Qt::NoItemFlags;
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void ModelArchivOgrn::AddToCat(DataArchivOgrn *data) { records.append(data); }
/*------------------------------------------------------*/
