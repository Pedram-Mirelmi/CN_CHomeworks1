#pragma once
#include "./AsyncServerFramework/io/BasicHTTPMessage.h"
#include "AsyncServerFramework/Session.h"

class INetMessageProcessor : public std::enable_shared_from_this<INetMessageProcessor>
{
public:
    virtual void processNetMessage(shared_ptr<HTTPMessage> netMsg, shared_ptr<Session> session) = 0;
};
