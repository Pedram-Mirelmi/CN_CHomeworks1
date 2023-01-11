TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

SUBDIRS += \
    AsyncServerFramework/AsyncServerFramework.pro

HEADERS += \
    AsyncNetFramework/AbstractNetIOManager.h \
    AsyncNetFramework/INetWriter.h \
    AsyncNetFramework/IService.h \
    AsyncNetFramework/io/BasicNetMessage.h \
    AsyncNetFramework/io/ISerializable.h \
    IResponseResolver.h \
    NetMessages/AllNetMessages.hpp \
    NetMessages/DownloadFileMessage.h \
    NetMessages/FTP-Message-types.h \
    NetMessages/FileContentMessage.h \
    NetMessages/GetHelpMessage.h \
    NetMessages/HelpContentMessage.h \
    NetMessages/PasswordMessage.h \
    NetMessages/QuitMessage.h \
    NetMessages/ShortResponseMessage.h \
    NetMessages/UploadFileMessage.h \
    NetMessages/UsernameMessage.h \
    NetworkHandler.hpp \
    ResponseHandler.hpp \
    typedefs.hpp

