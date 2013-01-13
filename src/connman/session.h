/*
 * Copyright © 2012, Jolla.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */
#ifndef SESSION_H_1356566832
#define SESSION_H_1356566832

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

#include <QtAddOnConnman/connman_global.h>

QT_BEGIN_NAMESPACE_CONNMAN

/*
 * Proxy class for interface net.connman.Session
 */
class Q_CONNMAN_EXPORT Session: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticConnmanService()
    { return "net.connman"; }
    static inline const char *staticInterfaceName()
    { return "net.connman.Session"; }

public:
    Session(const QString &path, QObject *parent = 0);

    ~Session();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> Change(const QString &name, const QDBusVariant &value)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(name) << QVariant::fromValue(value);
        return asyncCallWithArgumentList(QLatin1String("Change"), argumentList);
    }

    inline QDBusPendingReply<> Connect()
    {
         return asyncCall(QLatin1String("Connect"));
    }

    inline QDBusPendingReply<> Destroy()
    {
        return asyncCall(QLatin1String("Destroy"));
    }

    inline QDBusPendingReply<> Disconnect()
    {
        return asyncCall(QLatin1String("Disconnect"));
    }
};

QT_END_NAMESPACE_CONNMAN

#endif
