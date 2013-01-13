TARGET = QtAddOnConnman
VERSION = 0.3.0
TEMPLATE = lib
QT = core dbus

load(qt_module)

DBUS_INTERFACES = ../../data/interfaces/service.xml ../../data/interfaces/technology.xml

HEADERS += \
    connman_global.h \
    manager.h \
    networkmanager.h \
    networktechnology.h \
    networkservice.h \
    commondbustypes.h \
    clockproxy.h \
    clockmodel.h \
    useragent.h \
    session.h \
    sessionagent.h \
    networksession.h \
    counter.h

SOURCES += \
    networkmanager.cpp \
    networktechnology.cpp \
    networkservice.cpp \
    manager.cpp \
    clockproxy.cpp \
    clockmodel.cpp \
    commondbustypes.cpp \
    useragent.cpp \
    session.cpp \
    sessionagent.cpp \
    networksession.cpp \
    counter.cpp
