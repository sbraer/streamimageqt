#QT -= gui
QT += widgets
QT += network

CONFIG += c++14 console
CONFIG -= app_bundle

# remove possible other optimization flags
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2

# add the desired -O2 if not present
QMAKE_CXXFLAGS_RELEASE *= -O2

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#https://decovar.dev/blog/2018/06/08/qmake-copy-files/

SOURCES += \
        imagecreator.cpp \
        main.cpp \
        mytimer.cpp \
        nltcpsocket.cpp \
        tcpserver.cpp \
        waitkeys.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    imagecreator.h \
    mytimer.h \
    nltcpsocket.h \
    tcpserver.h \
    waitkeys.h

RESOURCES +=

DISTFILES += \
    images/val_paghera.jpg

OTHER_FILES += \
    $$PWD/images

!isEmpty(target.path): INSTALLS += target
