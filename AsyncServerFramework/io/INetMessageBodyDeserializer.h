#pragma once

#include "./NetMessage.h"

template <typename MsgType>
class INetMessageBodyDeserializer : public std::enable_shared_from_this<INetMessageBodyDeserializer<MsgType>>
{
public:
    virtual shared_ptr<NetMessage<MsgType>> deserializeBody(MsgType msgType, const char* bodyBuffer) = 0;
};
