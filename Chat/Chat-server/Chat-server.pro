TEMPLATE = app
CONFIG += console c++20
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
    AsyncServerFramework/io/BasicNetMessage.h \
    AsyncServerFramework/io/INetMessageDeserializer.h \
    AsyncServerFramework/io/ISerializable.h \
    Chat-Server.hpp \
    ChatServer-typedefs.hpp \
    IMainManager.hpp \
    MessageBodyDeserializer.hpp \
    NetIOManager.hpp \
    NetMessages/AllNetMessages.hpp \
    NetMessages/ChatServerMessagesTypes.hpp \
    NetMessages/ConnAck.hpp \
    NetMessages/ConnectMessage.hpp \
    NetMessages/InfoMessage.hpp \
    NetMessages/InfoReplyMessage.hpp \
    NetMessages/ListMessage.h \
    NetMessages/ListReplyMessage.hpp \
    NetMessages/ReceiveMessage.h \
    NetMessages/ReceiveReplyMessage.hpp \
    NetMessages/SendMessage.hpp \
    NetMessages/SendReplyMessage.hpp
