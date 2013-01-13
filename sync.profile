%modules = ( # path to module name map
    "QtAddOnConnman" => "$basedir/src/connman",
);
%moduleheaders = ( # restrict the module headers to those found in relative path
);
%mastercontent = (
    "connman" => "#include <QtAddOnConnman/QtAddOnConnman>\n",
);
%modulepris = (
);
%classnames = (
    "clockmodel.h" => "ClockModel",
    "clockproxy.h" => "ClockProxy",
    "commondbustypes.h" => "CommonDBusTypes",
    "counter.h" => "Counter",
    "debug.h" => "Debug",
    "manager.h" => "Manager",
    "networkmanager.h" => "NetworkManager",
    "networkservice.h" => "NetworkService",
    "networksession.h" => "NetworkSession",
    "networktechnology.h" => "NetworkTechnology",
    "sessionagent.h" => "SessionAgent",
    "session.h" => "Session",
    "useragent.h" => "UserAgent"
);
# Module dependencies.
# Every module that is required to build this module should have one entry.
# Each of the module version specifiers can take one of the following values:
#   - A specific Git revision.
#   - any git symbolic ref resolvable from the module's repository (e.g. "refs/heads/master" to track master branch)
#
%dependencies = (
    "qtbase" => "refs/heads/stable",
    "qtdeclarative" => "refs/heads/stable",
);
