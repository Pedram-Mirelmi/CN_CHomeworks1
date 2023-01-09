#pragma once
#include <exception>
#include "AsyncServerFramework/io/INetMessageBodyDeserializer.h"
#include "NetMessages/AllNetMessages.hpp"
#include "typedefs.hpp"
using std::make_shared;
using std::shared_ptr;

class MessageBodyDeserializer : public INetMessageBodyDeserializer<NetMessageType>
{
public:     

    // INetMessageBodyDeserializer interface
public:
    virtual shared_ptr<NetMessage<NetMessageType>> deserializeBody(NetMessageHeader<NetMessageType> header, char *bodyBuffer) override
    {
        // std::cout << "message type is: ";
        switch (header.getMessageType())
        {
            case NetMessageType::USERNAME:
            {
                auto message = shared_ptr<UsernameMessage>(new UsernameMessage());
                message->deserialize(bodyBuffer);
                return shared_ptr<_BNetMsg>(message);
                break;
            }
            case NetMessageType::PASSWORD:
            {
                auto message = shared_ptr<PasswordMessage>(new PasswordMessage());
                message->deserialize(bodyBuffer);
                return shared_ptr<_BNetMsg>(message);   
                break;
            }
            case NetMessageType::DOWNLOAD_FILE:
            {
                auto message = shared_ptr<DownloadFileMessage>(new DownloadFileMessage());
                message->deserialize(bodyBuffer);
                return shared_ptr<_BNetMsg>(message);
                break;
            }
            case NetMessageType::UPLOAD_FILE:
            {
                auto message = shared_ptr<UploadFileMessage>(new UploadFileMessage());
                message->deserialize(bodyBuffer);
                return shared_ptr<_BNetMsg>(message);
                break;
            }
            case NetMessageType::HELP_CONTENT:
            {
                auto message = shared_ptr<HelpContentMessage>(new HelpContentMessage());
                message->deserialize(bodyBuffer);
                return shared_ptr<_BNetMsg>(message);
                break;
            }
            case NetMessageType::FILE_CONTENT:
            {
                auto message = shared_ptr<FileContentMessage>(new FileContentMessage());
                message->deserialize(bodyBuffer);
                return shared_ptr<_BNetMsg>(message);
                break;
            }
            case NetMessageType::QUIT:
            {
                auto message = shared_ptr<QuitMessage>(new QuitMessage());
                message->deserialize(bodyBuffer);
                return shared_ptr<_BNetMsg>(message);
                break;
            }
            default:
                throw std::runtime_error("Unknown type of net message");
        }
    }
};
