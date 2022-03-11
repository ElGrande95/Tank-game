QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    barrier.cpp \
    bullet.cpp \
    explosion.cpp \
    main.cpp \
    myhero.cpp \
    myscene.cpp \
    startpage.cpp \
    target.cpp \
    targetdestroy.cpp \
    widget.cpp

HEADERS += \
    barrier.h \
    bullet.h \
    explosion.h \
    myhero.h \
    myscene.h \
    startpage.h \
    target.h \
    targetdestroy.h \
    widget.h

FORMS += \
    startpage.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Images.qrc
