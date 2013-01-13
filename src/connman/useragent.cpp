/*
 * Copyright © 2012, Jolla.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include <debug.h>
#include "useragent.h"

#define AGENT_PATH QLatin1String("/ConnectivityUserAgent")

QT_BEGIN_NAMESPACE_CONNMAN

UserAgent::UserAgent(QObject* parent) :
    QObject(parent),
    m_req_data(NULL),
    m_manager(NetworkManagerFactory::createInstance())
{
    new AgentAdaptor(this); // this object will be freed when UserAgent is freed
    QDBusConnection::systemBus().registerObject(AGENT_PATH, this);

    if (m_manager->isAvailable()) {
        m_manager->unregisterAgent(QString(AGENT_PATH));
        m_manager->registerAgent(QString(AGENT_PATH));
    }
    connect(m_manager, SIGNAL(availabilityChanged(bool)),
            this, SLOT(updateMgrAvailability(bool)));
}

UserAgent::~UserAgent()
{
    m_manager->unregisterAgent(QString(AGENT_PATH));
}

void UserAgent::requestUserInput(ServiceRequestData* data)
{
    m_req_data = data;
    QVariantList fields;

    foreach (const QString &key, data->fields.keys()) {
        QVariantMap field;

        field.insert(QLatin1String("name"), key);
        field.insert(QLatin1String("type"), data->fields.value(key).toMap().value(QLatin1String("Type")));
        fields.append(QVariant(field));
    }

    emit userInputRequested(data->objectPath, fields);
}

void UserAgent::cancelUserInput()
{
    delete m_req_data;
    m_req_data = NULL;
    emit userInputCanceled();
}

void UserAgent::reportError(const QString &error) {
    emit errorReported(error);
}

void UserAgent::sendUserReply(const QVariantMap &input) {
    if (m_req_data == NULL) {
        qWarning("Got reply for non-existing request");
        return;
    }

    if (!input.isEmpty()) {
        QDBusMessage &reply = m_req_data->reply;
        reply << input;
        QDBusConnection::systemBus().send(reply);
    } else {
        QDBusMessage error = m_req_data->msg.createErrorReply(
            QLatin1String("net.connman.Agent.Error.Canceled"),
            QLatin1String("canceled by user"));
        QDBusConnection::systemBus().send(error);
    }
    delete m_req_data;
    m_req_data = NULL;
}

void UserAgent::updateMgrAvailability(bool available)
{
    if (available) {
        m_manager->registerAgent(QString(AGENT_PATH));
    }
}

AgentAdaptor::AgentAdaptor(UserAgent* parent)
  : QDBusAbstractAdaptor(parent),
    m_userAgent(parent)
{
}

AgentAdaptor::~AgentAdaptor() {}

void AgentAdaptor::Release() {}

void AgentAdaptor::ReportError(const QDBusObjectPath &service_path, const QString &error)
{
    pr_dbg() << "From " << service_path.path() << " got this error:\n" << error;
    m_userAgent->reportError(error);
}

void AgentAdaptor::RequestBrowser(const QDBusObjectPath &service_path, const QString &url)
{
    pr_dbg() << "Service " << service_path.path() << " wants browser to open hotspot's url " << url;
}

void AgentAdaptor::RequestInput(const QDBusObjectPath &service_path,
                                       const QVariantMap &fields,
                                       const QDBusMessage &message)
{
    pr_dbg() << "Service " << service_path.path() << " wants user input";

    QVariantMap json;
    foreach (const QString &key, fields.keys()){
        QVariantMap payload = qdbus_cast<QVariantMap>(fields[key]);
        json.insert(key, payload);
    }

    message.setDelayedReply(true);

    ServiceRequestData *reqdata = new ServiceRequestData;
    reqdata->objectPath = service_path.path();
    reqdata->fields = json;
    reqdata->reply = message.createReply();
    reqdata->msg = message;

    m_userAgent->requestUserInput(reqdata);
}

void AgentAdaptor::Cancel()
{
    pr_dbg() << "WARNING: request to agent got canceled";
    m_userAgent->cancelUserInput();
}

QT_END_NAMESPACE_CONNMAN
