/*
 * Copyright © 2012, Jolla Ltd
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include "networksession.h"
#include "sessionagent.h"

NetworkSession::NetworkSession(QObject *parent) :
    QObject(parent),
    m_sessionAgent(0),
    m_path(QLatin1String("/ConnmanQmlSessionAgent"))
{
}

NetworkSession::~NetworkSession()
{
}

void NetworkSession::registerSession()
{
    if (m_path.isEmpty())
        return;
    m_sessionAgent = new SessionAgent(m_path ,this);
    connect(m_sessionAgent,SIGNAL(settingsUpdated(QVariantMap)),
            this,SLOT(sessionSettingsUpdated(QVariantMap)));

    m_sessionAgent->registerSession();
}

QString NetworkSession::state() const
{
    return settingsMap.value(QLatin1String("State")).toString();
}

QString NetworkSession::name() const
{
    return settingsMap.value(QLatin1String("Name")).toString();
}

QString NetworkSession::bearer() const
{
    return settingsMap.value(QLatin1String("Bearer")).toString();
}

QString NetworkSession::sessionInterface() const
{
    return settingsMap.value(QLatin1String("Interface")).toString();
}

QVariantMap NetworkSession::ipv4() const
{
    return settingsMap.value(QLatin1String("IPv4")).toMap();
}

QVariantMap NetworkSession::ipv6() const
{
    return settingsMap.value(QLatin1String("IPv6")).toMap();
}

QStringList NetworkSession::allowedBearers() const
{
    return settingsMap.value(QLatin1String("AllowedBearers")).toStringList();
}

QString NetworkSession::connectionType() const
{
    return settingsMap.value(QLatin1String("ConnectionType")).toString();
}

void NetworkSession::setAllowedBearers(const QStringList &bearers)
{
    settingsMap.insert(QLatin1String("AllowedBearers"),  qVariantFromValue(bearers));
    m_sessionAgent->setAllowedBearers(bearers);
}

void NetworkSession::setConnectionType(const QString &type)
{
    settingsMap.insert(QLatin1String("ConnectionType"),  qVariantFromValue(type));
    m_sessionAgent->setConnectionType(type);
}

void NetworkSession::requestDestroy()
{
    m_sessionAgent->requestDestroy();
}

void NetworkSession::requestConnect()
{
    m_sessionAgent->requestConnect();
}

void NetworkSession::requestDisconnect()
{
    m_sessionAgent->requestDisconnect();
}

void NetworkSession::sessionSettingsUpdated(const QVariantMap &settings)
{
    Q_FOREACH(const QString &name, settings.keys()) {
        settingsMap.insert(name,settings[name]);
    }
    Q_EMIT settingsChanged(settings);
}

QString NetworkSession::path() const
{
    return m_path;
}

void NetworkSession::setPath(const QString &path)
{
    m_path = path;
}
