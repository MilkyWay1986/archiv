#include "authorization.h"
#include "ui_authorization.h"

Authorization::Authorization(QWidget *parent)
    : QDialog(parent), ui(new Ui::Authorization) {
  ui->setupUi(this);
  ui->LE_LOGIN->setText("admindb");
  ui->LE_PASSWORD->setText("admindb");
}

Authorization::~Authorization() { delete ui; }

void Authorization::on_PB_OK_clicked() { emit login_button_clicked(); }

std::pair<QString, QString> Authorization::GetValues() {
  return std::make_pair(QVariant(ui->LE_LOGIN->text()).toString(),
                        QVariant(ui->LE_PASSWORD->text()).toString());
}
