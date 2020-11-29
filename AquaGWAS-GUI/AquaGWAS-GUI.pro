QT       += core gui
QT       += concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    annovar.cpp \
    assoctool.cpp \
    emmax.cpp \
    emmaxparamwidget.cpp \
    filereader.cpp \
    funcannotator.cpp \
    gcta.cpp \
    gemma.cpp \
    gemmaparamwidget.cpp \
    graphviewer.cpp \
    main.cpp \
    mainwindow.cpp \
    mtoolbutton.cpp \
    phenoselector.cpp \
    plink.cpp \
    poplddecay.cpp \
    process.cpp \
    qualityctrlwidget.cpp \
    runningmsgwidget.cpp \
    tool.cpp \
    workdirectory.cpp

HEADERS += \
    annovar.h \
    assoctool.h \
    emmax.h \
    emmaxparamwidget.h \
    filereader.h \
    funcannotator.h \
    gcta.h \
    gemma.h \
    gemmaparamwidget.h \
    graphviewer.h \
    mainwindow.h \
    mtoolbutton.h \
    phenoselector.h \
    plink.h \
    poplddecay.h \
    process.h \
    qualityctrlwidget.h \
    runningmsgwidget.h \
    tool.h \
    workdirectory.h

FORMS += \
    emmaxparamwidget.ui \
    gemmaparamwidget.ui \
    graphviewer.ui \
    mainwindow.ui \
    qualityctrlwidget.ui \
    runningmsgwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

QMAKE_LFLAGS += -no-pie

