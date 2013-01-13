TARGET = QtAddOnConnman
VERSION = 0.3.0
TEMPLATE = lib
QT = core dbus

load(qt_module)

DEFINES += QT_CONNMAN_LIB
DBUS_INTERFACES = ../../data/interfaces/service.xml ../../data/interfaces/technology.xml

HEADERS += manager.h \
    networkmanager.h \
    networktechnology.h \
    networkservice.h \
    commondbustypes.h \
    clockproxy.h \
    clockmodel.h \
    debug.h \
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
    debug.cpp \
    useragent.cpp \
    session.cpp \
    sessionagent.cpp \
    networksession.cpp \
    counter.cpp
