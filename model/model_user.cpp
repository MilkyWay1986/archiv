#include "model_user.h"

/*------------------------------------------------------*/
DataModelUser::DataModelUser(QObject *parent) : QObject(parent) {}

DataModelUser::DataModelUser(const QSqlQuery &qry, QObject *parent)
    : QObject(parent) {

  id_ = qry.value("id_").toUInt();
  login_ = qry.value("login_").toString();
  password_ = qry.value("password_").toString();
  role_ = qry.value("role_").toString();
  is_activ_ = qry.value("is_activ_").toBool();
}
//----------------------------------------

ModelUser::ModelUser(const QSharedPointer<DB::IDB> &pDB, QObject *parent)
    : QAbstractTableModel(parent), m_pDB(pDB) {
  QSqlTableModel::EditStrategy(OnManualSubmit);
}

int ModelUser::rowCount(const QModelIndex &I) const {
  if (!I.isValid()) {
    return records.size();
  } else
    return 0;
}
int ModelUser::columnCount(const QModelIndex &I) const {
  if (!I.isValid()) {
    return 5;
  } else
    return 0;
}
QVariant ModelUser::data(const QModelIndex &I, int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();
  DataModelUser *D = records[I.row()];
  switch (I.column()) {
  case 0:
    return D->id_;
  case 1:
    return D->login_;
  case 2:
    return D->password_;
  case 3:
    return D->role_;
  case 4:
    return D->is_activ_;
  default:
    return QVariant();
  }
  return QVariant();
}
QVariant ModelUser::headerData(int section, Qt::Orientation orientation,
                               int role) const {
  if (orientation != Qt::Horizontal)
    return QAbstractTableModel::headerData(section, orientation, role);

  switch (role) {
  case Qt::DisplayRole:
    switch (section) {
    case 0:
      return tr("id");
    case 1:
      return tr("Логин");
    case 2:
      return tr("Пароль");
    case 3:
      return tr("Роль");
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
bool ModelUser::setData(const QModelIndex &I, const QVariant &V, int role) {

  if (role == Qt::EditRole) {

    beginResetModel();

    switch (I.column()) {
    case 1: {
      break;
    }
    case 2: {

      records.at(I.row())->password_ = qvariant_cast<QString>(V);
      if (!records[I.row()]->password_.trimmed().isEmpty()) {
        QSqlQuery UPD;
        UPD.setForwardOnly(true);
        UPD.prepare("update user_ set"
                    " password_ = :PASSWORD"
                    " where id_ = :IID;");

        UPD.bindValue(":PASSWORD", records[I.row()]->password_);
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
      } else {
        QMessageBox::information(0, "Information",
                                 "Пароль не может быть пустым!");
      }
      break;
    }
    case 3: {
      records.at(I.row())->role_ = qvariant_cast<QString>(V);
      QString txt = records[I.row()]->role_;
      if (txt == QString("manager_") || txt == QString("operator_")) {

        QSqlQuery UPD;
        UPD.setForwardOnly(true);
        UPD.prepare("update user_ set"
                    " role_ = :ROLE"
                    " where id_ = :IID;");

        UPD.bindValue(":ROLE", txt);
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
      } else {
        qDebug() << txt;
        QMessageBox::information(0, "Information",
                                 "Роли: manager_  и operator_");
        return false;
        break;
      }
      break;
    }
    case 4: {
      records.at(I.row())->is_activ_ = qvariant_cast<bool>(V);

      QSqlQuery UPD;
      UPD.setForwardOnly(true);
      UPD.prepare("update user_ set"
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
    return false;
  }

  return false;
}
Qt::ItemFlags ModelUser::flags(const QModelIndex &I) const {
  if (!I.isValid())
    return Qt::NoItemFlags;
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void ModelUser::AddToCat(DataModelUser *data) { records.append(data); }
/*------------------------------------------------------*/
