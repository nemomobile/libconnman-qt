cmake_minimum_required(VERSION 2.8.8)

set(CONNMAN_QT_VERSION_MAJOR 0)
set(CONNMAN_QT_VERSION_MINOR 0)
set(CONNMAN_QT_VERSION_PATCH 0)
set(CONNMAN_QT_VERSION       "${CONNMAN_QT_VERSION_MAJOR}.${CONNMAN_QT_VERSION_MINOR}.${CONNMAN_QT_VERSION_PATCH}")

# Find additional CMake modules
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# The project currently requires QtCore, QtDBus
set(QT_USE_QTCORE true)
set(QT_USE_QTGDBUS true)

# Headers
set(SOURCES
   networkmanager.cpp
   networktechnology.cpp
   networkservice.cpp
   clockproxy.cpp
   clockmodel.cpp
   commondbustypes.cpp
    )

# Source    
set(HEADERS
    networkmanager.h
    networktechnology.h
    networkservice.h
    commondbustypes.h
    clockproxy.h
    clockmodel.h
    )

# Create DBUS interfaces
set_source_files_properties(connman-manager.xml
    PROPERTIES CLASSNAME Manager
               INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/commondbustypes.h)
set_source_files_properties(connman-service.xml
    PROPERTIES CLASSNAME Service)
set_source_files_properties(connman-technology.xml
    PROPERTIES CLASSNAME Technology)
QT4_ADD_DBUS_INTERFACE(SOURCES connman-manager.xml manager)
QT4_ADD_DBUS_INTERFACE(SOURCES connman-service.xml service)
QT4_ADD_DBUS_INTERFACE(SOURCES connman-technology.xml technology)

# Apply moc and create the resources

QT4_WRAP_CPP(HEADERS_MOC ${HEADERS})

# Compile the executable
add_library(connman-qt SHARED
    ${SOURCES} 
    ${HEADERS_MOC}  
    ${RESOURCES_RCC}
    )
    


# Link the executable to the different libraries. The variables are defined by find_package().
target_link_libraries(connman-qt
    ${QT_LIBRARIES}
    ${QT_QTCORE_LIBRARIES}
    ${QT_QTDBUS_LIBRARIES}
    )

# Find the required headers. The variables are defined by find_package().
include_directories(${CMAKE_CURRENT_BINARY_DIR}
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTDBUS_INCLUDE_DIR}
    )
    
install(TARGETS connman-qt DESTINATION usr/lib)
install(FILES ${HEADERS} DESTINATION usr/include/connman-qt)


