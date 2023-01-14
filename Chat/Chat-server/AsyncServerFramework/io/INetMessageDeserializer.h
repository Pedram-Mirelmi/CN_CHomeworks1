#pragma once

#include "./BasicNetMessage.h"

template <typename MsgType>
class INetMessageDeserializer : public std::enable_shared_from_this<INetMessageDeserializer<MsgType>>
{
public:
    virtual shared_ptr<NetMessage<MsgType>> deserializeMessage(NetMessageHeader<MsgType> header, char* buffer) = 0;
};
