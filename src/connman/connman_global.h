/*
 * Copyright © 2012, Jolla.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef CONNMAN_GLOBAL_H
#define CONNMAN_GLOBAL_H

#include <qglobal.h>

#if defined(QT_BUILD_ADDON_CONNMAN_LIB)
#  define Q_CONNMAN_EXPORT Q_DECL_EXPORT
#else
#  define Q_CONNMAN_EXPORT Q_DECL_IMPORT
#endif

#if defined(QT_NAMESPACE)
#  define QT_BEGIN_NAMESPACE_CONNMAN namespace QT_NAMESPACE { namespace QtAddOn { namespace Connman {
#  define QT_END_NAMESPACE_CONNMAN } } }
#  define QT_USE_NAMESPACE_CONNMAN using namespace QT_NAMESPACE::QtAddOn::Connman;
#  define QT_PREPEND_NAMESPACE_CONNMAN(name) ::QT_NAMESPACE::QtAddOn::Connman::name
#else
#  define QT_BEGIN_NAMESPACE_CONNMAN namespace QtAddOn { namespace Connman {
#  define QT_END_NAMESPACE_CONNMAN } }
#  define QT_USE_NAMESPACE_CONNMAN using namespace QtAddOn::Connman;
#  define QT_PREPEND_NAMESPACE_CONNMAN(name) ::QtAddOn::Connman::name
#endif

// A workaround for moc - if there is a header file that doesn't use the namespace,
// we still force moc to do "using namespace" but the namespace have to
// be defined, so let's define an empty namespace here
QT_BEGIN_NAMESPACE_CONNMAN
QT_END_NAMESPACE_CONNMAN

#endif // CONNMAN_GLOBAL_H
