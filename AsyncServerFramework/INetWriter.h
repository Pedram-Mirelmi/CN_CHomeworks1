#pragma once
#include "./io/NetMessage.h"
#include "./Session.h"

template<typename MsgType>
class INetWriter : public std::enable_shared_from_this<INetWriter<MsgType>>
{
public:
    virtual void writeMessage(shared_ptr<NetMessage<MsgType>> msg, std::shared_ptr<Session<MsgType>> session) = 0;
};
