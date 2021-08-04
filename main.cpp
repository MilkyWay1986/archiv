#include "archiv/archiv.h"
#include "db/postgresqlDB.h"
#include "personalarea/personalarea.h"
#include "users/users.h"

#include <QApplication>
#include <QDebug>
#include <QSharedPointer>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  static QSharedPointer<DB::IDB> DB =
      QSharedPointer<DB::PostgresqlDB>::create();

  PersonalArea pa(DB);
  pa.DisplayAuthorization();

  if (pa.GetpDB()->isOpenConnect())
    pa.DisplayPersonalArea();

  return a.exec();
}
