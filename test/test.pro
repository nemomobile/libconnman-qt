#-------------------------------------------------
#
# Project created by QtCreator 2010-10-12T13:49:46
#
#-------------------------------------------------

QT += quick

TARGET = testconnman-qt
CONFIG   += console
CONFIG   -= app_bundle

OTHER_FILES += *.qml

TEMPLATE = app


SOURCES += main.cpp

HEADERS +=

target.path = $$INSTALL_ROOT/usr/lib/libconnman-qt5/test

qml.files = $$OTHER_FILES
qml.path = $$target.path


INSTALLS += target qml
