#pragma once
#include "./io/BasicHTTPMessage.h"
#include "./Session.h"

class INetWriter : public std::enable_shared_from_this<INetWriter>
{
public:
    virtual void writeMessage(shared_ptr<HTTPMessage> msg, std::shared_ptr<Session> session) = 0;
};
