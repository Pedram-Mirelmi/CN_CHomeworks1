#pragma once
#include <iostream>

#include "./AsyncNetFramework/AbstractNetIOManager.h"
#include "NetMessages/FTP-Message-types.h"
#include "./NetMessages/AllNetMessages.hpp"
#include "./IResponseResolver.h"

class NetworkHandler : public AbstractNetIOManager<FTPMessageType>
{
public:
    NetworkHandler(const std::string& ip, uint16_t port)
        : AbstractNetIOManager<FTPMessageType>(ip, port)
    {

    }
private:
    shared_ptr<IResponseResolver> m_responseResolver;
protected:
    void onNewMessageReadCompletely() override
    {
        shared_ptr<BasicNetMessage<FTPMessageType>> msg;
        switch (m_tempHeader.getMessageType())
        {
            case FTPMessageType::USERNAME:
            {
                msg.reset(static_cast<BasicNetMessage<FTPMessageType>*>(new UsernameMessage()));
                break;
            }
            case FTPMessageType::PASSWORD:
            {
                msg.reset(static_cast<BasicNetMessage<FTPMessageType>*>(new PasswordMessage()));
                break;
            }

            case FTPMessageType::DOWNLOAD_FILE:
            {
                msg.reset(static_cast<BasicNetMessage<FTPMessageType>*>(new DownloadFileMessage()));
                break;
            }
            case FTPMessageType::UPLOAD_FILE:
            {
                msg.reset(static_cast<BasicNetMessage<FTPMessageType>*>(new UploadFileMessage()));
                break;
            }
            case FTPMessageType::GET_HELP:
            {
                msg.reset(static_cast<BasicNetMessage<FTPMessageType>*>(new GetHelpMessage()));
                break;
            }
            case FTPMessageType::QUIT:
            {
                msg.reset(static_cast<BasicNetMessage<FTPMessageType>*>(new QuitMessage()));
                break;
            }
            case FTPMessageType::SHORT_RESPONSE:
            {
                msg.reset(static_cast<BasicNetMessage<FTPMessageType>*>(new ShortResponseMessage()));
                break;
            }
            case FTPMessageType::FILE_CONTENT:
            {
                msg.reset(static_cast<BasicNetMessage<FTPMessageType>*>(new FileContentMessage()));
                break;
            }
            case FTPMessageType::HELP_CONTENT:
            {
                msg.reset(static_cast<BasicNetMessage<FTPMessageType>*>(new HelpContentMessage()));
                break;
            }
        }
        msg->deserialize(m_messageInBuff.data());
        m_responseResolver->resolveResponse(msg);
    }

    void onDisconnected() override
    {
        AbstractNetIOManager::onDisconnected();
        std::cout << "disconnected from server!" << std::endl;
        m_isConnected = false;
    }


public:
    void addPendingDownloadFile(const std::string& filename)
    {
        m_responseResolver->addPendingdDownloadFile(filename);
    }
    void addPendingUploadFile(const std::string& filename)
    {
        m_responseResolver->addPendingUploadFile(filename);
    }
    void closeConnection()
    {
        m_socket.close();
    }
    void sendUsername(const std::string& username)
    {
        UsernameMessage* usernameMsg = new UsernameMessage(username);
        shared_ptr<_BNetMsg> msg(static_cast<_BNetMsg*>(usernameMsg));
        writeMessage(msg);
    }

    void sendPassword(const std::string& password)
    {
        PasswordMessage* passwordMsg = new PasswordMessage(password);
        shared_ptr<_BNetMsg> msg(static_cast<_BNetMsg*>(passwordMsg));
        writeMessage(msg);
    }

    void sendRetr(const std::string& filename)
    {
        DownloadFileMessage* filenameMsg = new DownloadFileMessage(filename);
        shared_ptr<_BNetMsg> msg(static_cast<_BNetMsg*>(filenameMsg));
        addPendingDownloadFile(std::move(filename));
        writeMessage(msg);
    }

    void sendUpload(const std::string& filename)
    {
        UploadFileMessage* uploadMsg = new UploadFileMessage(filename);
        addPendingUploadFile(filename);
        shared_ptr<_BNetMsg> msg(static_cast<_BNetMsg*>(uploadMsg));
        writeMessage(msg);
    }

    void sendGetHelp()
    {
        GetHelpMessage* getHelpMsg = new GetHelpMessage();
        shared_ptr<_BNetMsg> msg(static_cast<_BNetMsg*>(getHelpMsg));
        writeMessage(msg);
    }

    void sendQuit()
    {
        QuitMessage* quitMsg = new QuitMessage();
        shared_ptr<_BNetMsg> msg(static_cast<_BNetMsg*>(quitMsg));
        writeMessage(msg);
    }


    void setResponseResolver(const shared_ptr<IResponseResolver> &newResponseResolver)
    {
        m_responseResolver = newResponseResolver;
    }
};


