/*
 * Copyright 2011 Intel Corporation.
 * Copyright © 2012, Jolla.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QtQml/QQmlEngine>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlContext>

#include <QtAddOnConnman/networkservice.h>
#include <QtAddOnConnman/clockmodel.h>

#include "components.h"
#include "networkingmodel.h"
#include "technologymodel.h"
#include "useragent.h"
#include "networksession.h"

QT_USE_NAMESPACE_CONNMAN

void Components::registerTypes(const char *uri)
{
    qmlRegisterUncreatableType<NetworkService>(uri, 0, 2, "NetworkService",
        "Please don't create \"NetworkService\" objects manually.");
    qmlRegisterType<NetworkingModel>(uri, 0, 2, "NetworkingModel");
    qmlRegisterType<TechnologyModel>(uri, 0, 2, "TechnologyModel");
    qmlRegisterType<UserAgent>(uri, 0, 2, "UserAgent");
    qmlRegisterType<ClockModel>(uri, 0, 2, "ClockModel");
    qmlRegisterType<NetworkSession>(uri, 0, 2, "NetworkSession");
}

void Components::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri);
    Q_UNUSED(engine);
}
