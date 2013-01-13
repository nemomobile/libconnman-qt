/*
 * Copyright © 2012, Jolla.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef SESSIONAGENT_H
#define SESSIONAGENT_H

#include <QtAddOnConnman/connman_global.h>
#include <QtAddOnConnman/manager.h>

QT_BEGIN_NAMESPACE_CONNMAN

class Session;

class Q_CONNMAN_EXPORT SessionAgent : public QObject
{
    Q_OBJECT

public:
    explicit SessionAgent(const QString &path,QObject* parent = 0);
    virtual ~SessionAgent();

    void setAllowedBearers(const QStringList &bearers);
    void setConnectionType(const QString &type);
    void registerSession();
    void requestConnect();
    void requestDisconnect();
    void requestDestroy();

public slots:
    void release();
    void update(const QVariantMap &settings);

Q_SIGNALS:
    void settingsUpdated(const QVariantMap &settings);
    void released();

private:
    QString agentPath;
    QVariantMap sessionSettings;
    Manager* m_manager;
    Session *m_session;

    friend class SessionNotificationAdaptor;
};

class Q_CONNMAN_EXPORT SessionNotificationAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "net.connman.Notification")

public:
    explicit SessionNotificationAdaptor(SessionAgent* parent);
    virtual ~SessionNotificationAdaptor();

public slots:
    void Release();
    void Update(const QVariantMap &settings);
private:
    SessionAgent* m_sessionAgent;
};

QT_END_NAMESPACE_CONNMAN

#endif // USERAGENT_H
