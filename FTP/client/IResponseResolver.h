#pragma once

#include <memory>
#include "./AsyncNetFramework/io/BasicNetMessage.h"
#include "./NetMessages/FTP-Message-types.h"
class IResponseResolver
{
public:
    virtual void resolveResponse(shared_ptr<BasicNetMessage<FTPMessageType>> response) = 0;
};
