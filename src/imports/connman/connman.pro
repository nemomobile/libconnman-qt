QT += qml dbus connman-private

SOURCES = \
    components.cpp \
    networkingmodel.cpp \
    technologymodel.cpp 

HEADERS = \
    components.h \
    networkingmodel.h \
    technologymodel.h 

LIBS += -L$$QT.connman.libs

load(qml_plugin)
