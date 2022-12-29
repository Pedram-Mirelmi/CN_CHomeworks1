#pragma once

#include "./AsyncNetFramework/AbstractNetIOManager.h"
#include "NetMessages/FTP-Message-types.h"
#include "./NetMessages/AllNetMessages.hpp"
#include "./IResponseResolver.h"

class NetworkHandler : AbstractNetIOManager<FTPMessageType>
{
private:
    shared_ptr<IResponseResolver> m_responseResolver;
protected:
    void onNewMessageReadCompletely()
    {
        switch (m_tempHeader.getMessageType()) {
        case FTPMessageType::USERNAME:
        {
            PasswordMessage* msg = new PasswordMessage();
            msg->deserialize(m_messageInBuff.data());
            auto response = shared_ptr<BasicNetMessage<FTPMessageType>>(static_cast<BasicNetMessage<FTPMessageType>*>(msg));
            m_responseResolver->resolveResponse(std::move(response));
            break;
        }
        case FTPMessageType::PASSWORD:
            break;
        case FTPMessageType::DOWNLOAD_FILE:
            break;
        case FTPMessageType::UPLOAD_FILE:
            break;
        case FTPMessageType::GET_HELP:
            break;
        case FTPMessageType::QUIT:
            break;
        case FTPMessageType::SHORT_RESPONSE:
            break;
        case FTPMessageType::FILE_CONTENT:
            break;
        case FTPMessageType::HELP_CONTENT:
            break;

        }
    }


};
