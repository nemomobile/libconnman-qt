/*
 * Copyright © 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef COMMONDBUSTYPES_H
#define COMMONDBUSTYPES_H

#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QtDBus/QtDBus>
#include <QtDBus/QDBusObjectPath>

#include <QtAddOnConnman/connman_global.h>

QT_BEGIN_NAMESPACE_CONNMAN

typedef QMap<QString, QString> StringMap;

// TODO: re-implement with better interface i.e. "const QString path() const" instead of objpath
struct ConnmanObject {
    QDBusObjectPath objpath;
    QVariantMap properties;
};

typedef QList<ConnmanObject> ConnmanObjectList;

QDBusArgument &operator<<(QDBusArgument &argument, const ConnmanObject &obj);
const QDBusArgument &operator>>(const QDBusArgument &argument, ConnmanObject &obj);

QT_END_NAMESPACE_CONNMAN

Q_DECLARE_METATYPE(QtAddOn::Connman::StringMap)
Q_DECLARE_METATYPE(QtAddOn::Connman::ConnmanObject)
Q_DECLARE_METATYPE(QtAddOn::Connman::ConnmanObjectList)

inline void registerCommonDataTypes() {
  qDBusRegisterMetaType<QtAddOn::Connman::StringMap>();
  qDBusRegisterMetaType<QtAddOn::Connman::ConnmanObject>();
  qDBusRegisterMetaType<QtAddOn::Connman::ConnmanObjectList>();
  qRegisterMetaType<QtAddOn::Connman::ConnmanObjectList>("ConnmanObjectList");
}

#endif //COMMONDBUSTYPES_H
