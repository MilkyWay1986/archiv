#include "model.h"

Model::Model(const QSharedPointer<DB::IDB> &pDB, QObject *parent)
    : QAbstractTableModel(parent), m_pDB(pDB) {
  QSqlTableModel::EditStrategy(OnManualSubmit);
}

Model::~Model() {}

int Model::rowCount(const QModelIndex &I) const {
  if (!I.isValid()) {
    return records.size();
  } else
    return 0;
}

int Model::columnCount(const QModelIndex &I) const {
  if (!I.isValid()) {
    return 9;
  } else
    return 0;
}

QVariant Model::data(const QModelIndex &I, int role) const {

  if (role != Qt::DisplayRole)
    return QVariant();
  Data *D = records[I.row()];
  switch (I.column()) {
  case 0:
    return D->rid_record_;
  case 1:
    return D->rid_owner_;
  case 2:
    return D->number_document;
  case 3:
    return D->data_document;
  case 4:
    return D->ogrn_document;
  case 5:
    return D->last_date_change_;
  case 6:
    return D->rid_who_last_date_change_;
  case 7:
    return D->path_;
  case 8:
    return D->is_activ_;
  default:
    return QVariant();
  }
  return QVariant();
}

QVariant Model::headerData(int section, Qt::Orientation orientation,
                           int role) const {

  if (orientation != Qt::Horizontal)
    return QAbstractTableModel::headerData(section, orientation, role);

  switch (role) {
  case Qt::DisplayRole:
    switch (section) {
    case 0:
      return tr("rid_record_");
    case 1:
      return tr("rid_owner_");
    case 2:
      return tr("number_document");
    case 3:
      return tr("data_document");
    case 4:
      return tr("ogrn_document");
    case 5:
      return tr("last_date_change_");
    case 6:
      return tr("rid_who_last_date_change_");
    case 7:
      return tr("path_");
    case 8:
      return tr("is_activ_");
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

bool Model::setData(const QModelIndex &I, const QVariant &V, int role) {

  if (role == Qt::EditRole) {
    QMessageBox::information(0, "Information", "setData!");
    beginResetModel();

    qDebug() << I.row() << " " << I.column();
    switch (I.column()) {
    case 0: {
      records.at(I.row())->rid_record_ = qvariant_cast<uint>(V);
      break;
    }

    case 1: {
      records.at(I.row())->rid_owner_ = qvariant_cast<uint>(V);
      break;
    }

    case 2: {
      records.at(I.row())->number_document = qvariant_cast<QString>(V);
      break;
    }

    case 3: {
      records.at(I.row())->data_document = qvariant_cast<QString>(V);
      break;
    }

    case 4: {
      records.at(I.row())->ogrn_document = qvariant_cast<QString>(V);
      break;
    }

    case 5: {
      records.at(I.row())->last_date_change_ = qvariant_cast<QVariant>(V);
      break;
    }

    case 6: {
      records.at(I.row())->rid_who_last_date_change_ = qvariant_cast<uint>(V);
      break;
    }

    case 7: {
      records.at(I.row())->path_ = qvariant_cast<QString>(V);
      break;
    }

    case 8: {
      records.at(I.row())->is_activ_ = qvariant_cast<bool>(V);
      break;
    }
    }

    QSqlQuery UPD;
    UPD.setForwardOnly(true);
    UPD.prepare("update document_ set"
                " number_document           = :NUMBER_DOC,"
                " data_document             = :DATA_DOC,"
                " ogrn_document             = :OGRN,"
                " last_date_change_         = :LDC,"
                " path_                     = :PATH,"
                " is_activ_                 = :ISACTIV"
                " where id_ = :IID;");

    UPD.bindValue(":NUMBER_DOC", records[I.row()]->number_document);
    UPD.bindValue(":DATA_DOC", records[I.row()]->data_document);
    UPD.bindValue(":OGRN", records[I.row()]->ogrn_document);
    UPD.bindValue(":LDC", records[I.row()]->last_date_change_);
    UPD.bindValue(":PATH", records[I.row()]->path_);
    UPD.bindValue(":ISACTIV", records[I.row()]->is_activ_);
    UPD.bindValue(":IID", records[I.row()]->id_);
    qDebug() << "-----------";
    for (auto &itm : records)
      qDebug() << itm->id_;
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
  }

  // 4.2

  endResetModel();

  return true;
}

Qt::ItemFlags Model::flags(const QModelIndex &I) const {
  if (!I.isValid())
    return Qt::NoItemFlags;
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void Model::AddToCat(Data *data) { records.append(data); }
