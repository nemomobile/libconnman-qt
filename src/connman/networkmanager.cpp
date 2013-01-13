/*
 * Copyright © 2010, Intel Corporation.
 * Copyright © 2012, Jolla.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include <QDebug>

#include "manager.h"
#include "networkmanager.h"

QT_BEGIN_NAMESPACE_CONNMAN

static NetworkManager* staticInstance = NULL;

NetworkManager* NetworkManagerFactory::createInstance()
{
    if(!staticInstance)
        staticInstance = new NetworkManager;

    return staticInstance;
}

// NetworkManager implementation

const QLatin1String NetworkManager::State("State");
const QLatin1String NetworkManager::OfflineMode("OfflineMode");
const QLatin1String NetworkManager::SessionMode("SessionMode");

NetworkManager::NetworkManager(QObject* parent)
  : QObject(parent),
    m_manager(NULL),
    m_getPropertiesWatcher(NULL),
    m_getTechnologiesWatcher(NULL),
    m_getServicesWatcher(NULL),
    m_defaultRoute(NULL),
    watcher(NULL),
    m_available(false)
{
    registerCommonDataTypes();

    watcher = new QDBusServiceWatcher(QLatin1String("net.connman"), QDBusConnection::systemBus(),
            QDBusServiceWatcher::WatchForRegistration |
            QDBusServiceWatcher::WatchForUnregistration, this);
    connect(watcher, SIGNAL(serviceRegistered(QString)),
            this, SLOT(connectToConnman()));
    connect(watcher, SIGNAL(serviceUnregistered(QString)),
            this, SLOT(connmanUnregistered()));

    m_available = QDBusConnection::systemBus().interface()->isServiceRegistered(QLatin1String("net.connman"));

    if (m_available)
        connectToConnman();
}

NetworkManager::~NetworkManager() {}

void NetworkManager::connectToConnman()
{
    disconnectFromConnman();
    m_manager = new Manager(QLatin1String("net.connman"), QLatin1String("/"),
            QDBusConnection::systemBus(), this);

    if (!m_manager->isValid()) {

        qDebug() << "D-Bus net.connman.Manager object is invalid. Connman may not be running or is invalid.";

        delete m_manager;
        m_manager = NULL;

        // shouldn't happen but in this case service isn't available
        if(m_available)
            emit availabilityChanged(m_available = false);
    } else {
        QDBusPendingReply<QVariantMap> props_reply = m_manager->GetProperties();
        m_getPropertiesWatcher = new QDBusPendingCallWatcher(props_reply, m_manager);
        connect(m_getPropertiesWatcher,
                SIGNAL(finished(QDBusPendingCallWatcher*)),
                this,
                SLOT(getPropertiesReply(QDBusPendingCallWatcher*)));

        QDBusPendingReply<ConnmanObjectList> techs_reply = m_manager->GetTechnologies();
        m_getTechnologiesWatcher = new QDBusPendingCallWatcher(techs_reply, m_manager);
        connect(m_getTechnologiesWatcher,
                SIGNAL(finished(QDBusPendingCallWatcher*)),
                this,
                SLOT(getTechnologiesReply(QDBusPendingCallWatcher*)));

        QDBusPendingReply<ConnmanObjectList> services_reply = m_manager->GetServices();
        m_getServicesWatcher = new QDBusPendingCallWatcher(services_reply, m_manager);
        connect(m_getServicesWatcher,
                SIGNAL(finished(QDBusPendingCallWatcher*)),
                this,
                SLOT(getServicesReply(QDBusPendingCallWatcher*)));

        if(!m_available)
            emit availabilityChanged(m_available = true);

        qDebug() << "Connected";
    }
}

void NetworkManager::disconnectFromConnman()
{
    if (m_manager) {
        delete m_manager;
        m_manager = NULL;
    }
    // FIXME: should we delete technologies and services?
}


void NetworkManager::connmanUnregistered()
{
    disconnectFromConnman();

    if(m_available)
        emit availabilityChanged(m_available = false);
}


// These functions is a part of setup procedure

void NetworkManager::getPropertiesReply(QDBusPendingCallWatcher *call)
{
    Q_ASSERT(call);

    qDebug() << "Got reply with manager's properties";

    QDBusPendingReply<QVariantMap> reply = *call;
    if (reply.isError()) {

        qDebug() << "Error getPropertiesReply: " << reply.error().message();

        disconnectFromConnman();

        // TODO: set up timer to reconnect in a bit
        QTimer::singleShot(10000,this,SLOT(connectToConnman()));
    } else {

        m_propertiesCache = reply.value();

        qDebug() << "Initial Manager's properties";
        qDebug() << "\tState: " << m_propertiesCache[State].toString();
        qDebug() << "\tOfflineMode: " << m_propertiesCache[OfflineMode].toString();

        emit stateChanged(m_propertiesCache[State].toString());

        connect(m_manager,
                SIGNAL(PropertyChanged(const QString&, const QDBusVariant&)),
                this,
                SLOT(propertyChanged(const QString&, const QDBusVariant&)));
    }
}

void NetworkManager::getTechnologiesReply(QDBusPendingCallWatcher *call)
{
    Q_ASSERT(call);

    qDebug() << "Got reply with technolgies";

    QDBusPendingReply<ConnmanObjectList> reply = *call;
    if (reply.isError()) {

        qDebug() << "Error getTechnologiesReply:" << reply.error().message();

        disconnectFromConnman();

        // TODO: set up timer to reconnect in a bit
        //QTimer::singleShot(10000,this,SLOT(connectToConnman()));
    } else {

        ConnmanObjectList lst = reply.value();
        ConnmanObject obj;
        foreach (obj, lst) { // TODO: consider optimizations

            NetworkTechnology *tech = new NetworkTechnology(obj.objpath.path(),
                    obj.properties, this);

            m_technologiesCache.insert(tech->type(), tech);

            qDebug() << "Technology: " << tech->type();
            qDebug() << "\tConnected:" << tech->connected();
            qDebug() << "\tPowered:" << tech->powered();
        }

        connect(m_manager,
                SIGNAL(TechnologyAdded(const QDBusObjectPath &, const QVariantMap &)),
                this,
                SLOT(technologyAdded(const QDBusObjectPath &, const QVariantMap &)));

        connect(m_manager,
                SIGNAL(TechnologyRemoved(const QDBusObjectPath &)),
                this,
                SLOT(technologyRemoved(const QDBusObjectPath &)));

        emit technologiesChanged();
    }
}

void NetworkManager::getServicesReply(QDBusPendingCallWatcher *call)
{
    Q_ASSERT(call);

    qDebug() << "Got reply with services";

    QDBusPendingReply<ConnmanObjectList> reply = *call;
    if (reply.isError()) {

        qDebug() << "Error getServicesReply:" << reply.error().message();

        disconnectFromConnman();

        // TODO: set up timer to reconnect in a bit
        //QTimer::singleShot(10000,this,SLOT(connectToConnman()));
    } else {

        ConnmanObjectList lst = reply.value();
        ConnmanObject obj;
        int order = -1;
        NetworkService *service = NULL;

        // make sure we don't leak memory
        m_servicesOrder.clear();

        foreach (obj, lst) { // TODO: consider optimizations
            order++;

            service = new NetworkService(obj.objpath.path(),
                    obj.properties, this);

            m_servicesCache.insert(obj.objpath.path(), service);
            m_servicesOrder.push_back(service);

            qDebug() << "From Service: " << obj.objpath.path();

            // by connman's documentation, first service is always
            // the default route's one
            if (order == 0)
                updateDefaultRoute(service);
        }

        // if no service was replied
        if (order == -1)
            updateDefaultRoute(NULL);

        emit servicesChanged();

        connect(m_manager,
                SIGNAL(ServicesChanged(ConnmanObjectList, QList<QDBusObjectPath>)),
                this,
                SLOT(updateServices(ConnmanObjectList, QList<QDBusObjectPath>)));

    }
}

void NetworkManager::updateServices(const ConnmanObjectList &changed, const QList<QDBusObjectPath> &removed)
{
    qDebug() << "Number of services that changed: " << changed.size();

    foreach (QDBusObjectPath obj, removed) {
        qDebug() << "Removing " << obj.path();
        m_servicesCache.value(obj.path())->deleteLater();
        m_servicesCache.remove(obj.path());
    }

    ConnmanObject connmanobj;
    int order = -1;
    NetworkService *service = NULL;

    // make sure we don't leak memory
    m_servicesOrder.clear();

    foreach (connmanobj, changed) {
        order++;

        if (!m_servicesCache.contains(connmanobj.objpath.path())) {
            service = new NetworkService(connmanobj.objpath.path(),
                    connmanobj.properties, this);
            m_servicesCache.insert(connmanobj.objpath.path(), service);
            qDebug() << "Added service " << connmanobj.objpath.path();
        } else {
            service = m_servicesCache.value(connmanobj.objpath.path());
        }

        m_servicesOrder.push_back(service);

        if (order == 0)
            updateDefaultRoute(service);
    }

    if (order == -1)
        updateDefaultRoute(NULL);

    emit servicesChanged();
}

void NetworkManager::updateDefaultRoute(NetworkService* defaultRoute)
{
    if (m_defaultRoute != defaultRoute) {
        m_defaultRoute = defaultRoute;
        emit defaultRouteChanged(m_defaultRoute);
    }
}

void NetworkManager::propertyChanged(const QString &name,
        const QDBusVariant &value)
{
    QVariant tmp = value.variant();

    qDebug() << "Manager's property" << name << "changed from"
             << m_propertiesCache[name].toString() << "to" << tmp.toString();

    m_propertiesCache[name] = tmp;
    if (name == State) {
        emit stateChanged(tmp.toString());
    } else if (name == OfflineMode) {
        emit offlineModeChanged(tmp.toBool());
    } else if (name == SessionMode) {
       emit sessionModeChanged(tmp.toBool());
   }
}

void NetworkManager::technologyAdded(const QDBusObjectPath &technology,
                                     const QVariantMap &properties)
{
    NetworkTechnology *tech = new NetworkTechnology(technology.path(),
                                                    properties, this);

    m_technologiesCache.insert(tech->type(), tech);

    qDebug() << "Technology: " << tech->type();
    qDebug() << "\tConnected:" << tech->connected();
    qDebug() << "\tPowered:" << tech->powered();

    emit technologiesChanged();
}

void NetworkManager::technologyRemoved(const QDBusObjectPath &technology)
{
    NetworkTechnology *net;
    // if we wasn't storing by type() this loop would be unecessary
    // but since this function will be triggered rarely that's fine
    foreach (net, m_technologiesCache) {
        if (net->objPath() == technology.path()) {

            qDebug() << "Removing " << net->objPath();
            m_technologiesCache.remove(net->type());
            net->deleteLater();

            break;
        }
    }

    emit technologiesChanged();
}


// Public API /////////////

// Getters


bool NetworkManager::isAvailable() const
{
    return m_available;
}


const QString NetworkManager::state() const
{
    return m_propertiesCache[State].toString();
}

bool NetworkManager::offlineMode() const
{
    return m_propertiesCache[OfflineMode].toBool();
}

NetworkService* NetworkManager::defaultRoute() const
{
    return m_defaultRoute;
}

NetworkTechnology* NetworkManager::getTechnology(const QString &type) const
{
    if (m_technologiesCache.contains(type))
        return m_technologiesCache.value(type);
    else {
        qDebug() << "Technology " << type << " doesn't exist";
        return NULL;
    }
}

const QVector<NetworkTechnology *> NetworkManager::getTechnologies() const
{
    QVector<NetworkTechnology *> techs(m_technologiesCache.size(), NULL);

    foreach (NetworkTechnology *tech, m_technologiesCache) {
        techs.push_back(tech);
    }

    return techs;
}

const QVector<NetworkService*> NetworkManager::getServices(const QString &tech) const
{
    QVector<NetworkService *> services;

    // this foreach is based on the m_servicesOrder to keep connman's sort
    // of services.
    foreach (NetworkService *service, m_servicesOrder) {
        if (tech.isEmpty() || service->type() == tech)
            services.push_back(service);
    }

    return services;
}

// Setters

void NetworkManager::setOfflineMode(const bool &offlineMode)
{
    if(!m_manager) return;

    QDBusPendingReply<void> reply =
        m_manager->SetProperty(OfflineMode,
                               QDBusVariant(QVariant(offlineMode)));
}

  // these shouldn't crash even if connman isn't available
void NetworkManager::registerAgent(const QString &path)
{
    if(m_manager)
        m_manager->RegisterAgent(QDBusObjectPath(path));
}

void NetworkManager::unregisterAgent(const QString &path)
{
    if(m_manager)
        m_manager->UnregisterAgent(QDBusObjectPath(path));
}

void NetworkManager::registerCounter(const QString &path, quint32 accuracy,quint32 period)
{
    if(m_manager)
        m_manager->RegisterCounter(QDBusObjectPath(path),accuracy, period);
}

void NetworkManager::unregisterCounter(const QString &path)
{
    if(m_manager)
        m_manager->UnregisterCounter(QDBusObjectPath(path));
}

QDBusObjectPath NetworkManager::createSession(const QVariantMap &settings, const QString &sessionNotifierPath)
{
    qDebug() << Q_FUNC_INFO << sessionNotifierPath << m_manager;

    QDBusPendingReply<QDBusObjectPath> reply;
    if(m_manager)
       reply = m_manager->CreateSession(settings,QDBusObjectPath(sessionNotifierPath));
    return reply;
}

void NetworkManager::destroySession(const QString &sessionAgentPath)
{
    if(m_manager)
        m_manager->DestroySession(QDBusObjectPath(sessionAgentPath));
}

void NetworkManager::setSessionMode(const bool &sessionMode)
{
    if(m_manager)
        m_manager->SetProperty(SessionMode, QDBusVariant(QVariant(sessionMode)));
}

bool NetworkManager::sessionMode() const
{
    return m_propertiesCache[SessionMode].toBool();
}

QT_END_NAMESPACE_CONNMAN
