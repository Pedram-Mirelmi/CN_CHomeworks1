#pragma once
#include "./AsyncServerFramework/io/HTTP-Request.hpp"
#include "AsyncServerFramework/Session.h"

class INetMessageProcessor : public std::enable_shared_from_this<INetMessageProcessor>
{
public:
    virtual void processNetMessage(shared_ptr<HTTPRequest> netMsg, shared_ptr<Session> session) = 0;
};
