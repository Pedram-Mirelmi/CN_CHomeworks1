#pragma once
#include "AsyncServerFramework/io/BasicNetMessage.h"
#include "AsyncServerFramework/Session.h"

template<typename MsgType>
class INetMessageProcessor : public std::enable_shared_from_this<INetMessageProcessor<MsgType>>
{
public:
    virtual void processNetMessage(shared_ptr<NetMessage<MsgType>> netMsg, shared_ptr<Session<MsgType>> session) = 0;
};
