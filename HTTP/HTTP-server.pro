TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

SUBDIRS += \
    AsyncServerFramework/AsyncServerFramework.pro



HEADERS += \
    AsyncServerFramework/AbstractNetIOManager.h \
    AsyncServerFramework/INetWriter.h \
    AsyncServerFramework/IService.h \
    AsyncServerFramework/Session.h \
    AsyncServerFramework/io/BasicHTTPMessage.h \
    AsyncServerFramework/io/ISerializable.h \
    HTTPServer.h \
    INetMessageProcessor.hpp \
    NetIOManager.hpp \
    NetMessages/HTTP-Request.hpp \
    NetMessages/HTTP-Response.hpp
