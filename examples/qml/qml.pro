SOURCES = main.cpp

OTHER_FILES += test.qml

TARGET = qml

QT = quick

CONFIG += console

# install
target.path = $$[QT_INSTALL_EXAMPLES]/addonconnman/qml
INSTALLS += target
