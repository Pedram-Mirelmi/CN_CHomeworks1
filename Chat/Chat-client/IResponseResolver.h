#pragma once

#include <memory>
#include "./AsyncNetFramework/io/BasicNetMessage.h"
#include "./NetMessages/MessagesTypes.hpp"
class IResponseResolver
{
public:
    virtual void resolveResponse(shared_ptr<NetMessage<MessageTypes>> response) = 0;
    virtual void addPendingdDownloadFile(std::string&& filename) = 0;
    virtual void addPendingUploadFile(std::string&& filename) = 0;
};
