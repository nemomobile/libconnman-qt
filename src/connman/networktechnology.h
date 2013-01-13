/*
 * Copyright © 2010, Intel Corporation.
 * Copyright © 2012, Jolla.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef NETWORKTECHNOLOGY_H
#define NETWORKTECHNOLOGY_H

#include <QtDBus>

class NetConnmanTechnologyInterface;

class NetworkTechnology : public QObject
{
    Q_OBJECT;

    Q_PROPERTY(QString name READ name);
    Q_PROPERTY(QString type READ type);
    Q_PROPERTY(bool powered READ powered WRITE setPowered NOTIFY poweredChanged);
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged);

public:
    NetworkTechnology(const QString &path, const QVariantMap &properties, QObject* parent);
    virtual ~NetworkTechnology();

    const QString name() const;
    const QString type() const;
    bool powered() const;
    bool connected() const;
    const QString objPath() const;

public slots:
    void setPowered(const bool &powered);
    void scan();

signals:
    void poweredChanged(const bool &powered);
    void connectedChanged(const bool &connected);
    void scanFinished();

private:
    NetConnmanTechnologyInterface *m_technology;
    QVariantMap m_propertiesCache;
    QDBusPendingCallWatcher *m_scanWatcher;

    static const QLatin1String Name;
    static const QLatin1String Type;
    static const QLatin1String Powered;
    static const QLatin1String Connected;

private slots:
    void propertyChanged(const QString &name, const QDBusVariant &value);
    void scanReply(QDBusPendingCallWatcher *call);

private:
    Q_DISABLE_COPY(NetworkTechnology);
};

#endif //NETWORKTECHNOLOGY_H
