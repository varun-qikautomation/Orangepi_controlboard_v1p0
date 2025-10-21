QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addproductiondialog.cpp \
    cardframe.cpp \
    flowlayout.cpp \
    home.cpp \
    main.cpp \
    login.cpp \
    partsin.cpp \
    pickup.cpp \
    productionline.cpp \
    qrcodetemplate.cpp \
    reelin.cpp \
    settings.cpp \
    storesin.cpp \
    storesout.cpp \
    warning.cpp

HEADERS += \
    DatabaseManager.h \
    DatabaseSchema.h \
    addproductiondialog.h \
    cardframe.h \
    flowlayout.h \
    home.h \
    login.h \
    partsin.h \
    pickup.h \
    productionline.h \
    qrcodetemplate.h \
    reelin.h \
    settings.h \
    storesin.h \
    storesout.h \
    warning.h

FORMS += \
    addproductiondialog.ui \
    home.ui \
    login.ui \
    partsin.ui \
    pickup.ui \
    productionline.ui \
    qrcodetemplate.ui \
    reelin.ui \
    settings.ui \
    storesin.ui \
    storesout.ui \
    warning.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc \
    resources.qrc
