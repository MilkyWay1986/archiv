QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    archiv/archiv.cpp \
    authorization/authorization.cpp \
    db/configure.cpp \
    db/postgresqlDB.cpp \
    indexing/indexing.cpp \
    main.cpp \
    model/data.cpp \
    model/model.cpp \
    model/model_archiv.cpp \
    model/model_user.cpp \
    personalarea/personalarea.cpp \
    uploadbarcodes/uploadbarcodes.cpp \
    uploaddocuments/uploaddocuments.cpp \
    users/users.cpp

HEADERS += \
    archiv/archiv.h \
    authorization/authorization.h \
    db/IDB.h \
    db/configure.h \
    db/postgresqlDB.h \
    indexing/indexing.h \
    model/data.h \
    model/model.h \
    model/model_archiv.h \
    model/model_user.h \
    personalarea/personalarea.h \
    uploadbarcodes/uploadbarcodes.h \
    uploaddocuments/uploaddocuments.h \
    users/users.h

FORMS += \
    archiv/archiv.ui \
    authorization/authorization.ui \
    indexing/indexing.ui \
    personalarea/personalarea.ui \
    uploadbarcodes/uploadbarcodes.ui \
    uploaddocuments/uploaddocuments.ui \
    users/users.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
