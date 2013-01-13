TARGET = declarative_connman
TEMPLATE = lib
QT += qml dbus
CONFIG += plugin
SOURCES = components.cpp networkingmodel.cpp technologymodel.cpp 
HEADERS = components.h networkingmodel.h technologymodel.h 

INCLUDEPATH += ../libconnman-qt
LIBS += -L../libconnman-qt -lconnman-qt5

target.path = $$[QT_INSTALL_QML]/MeeGo/Connman
qmldir.files += qmldir
qmldir.path = $$[QT_INSTALL_QML]/MeeGo/Connman

INSTALLS += target qmldir
