TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

SUBDIRS += \
    AsyncServerFramework/AsyncServerFramework.pro

LIBS += -lfmt


HEADERS += \
    AsyncServerFramework/AbstractNetIOManager.h \
    AsyncServerFramework/INetWriter.h \
    AsyncServerFramework/IService.h \
    AsyncServerFramework/Session.h \
    AsyncServerFramework/io/BasicHTTPMessage.h \
    AsyncServerFramework/io/ISerializable.h \
    AsyncServerFramework/io/HTTP-Request.hpp \
    AsyncServerFramework/io/HTTP-Response.hpp \
    HTTPServer.h \
    INetMessageProcessor.hpp \
    NetIOManager.hpp

DISTFILES += \
    404.html \
    README.md

