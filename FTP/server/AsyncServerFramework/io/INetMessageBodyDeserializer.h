#pragma once

#include "./BasicNetMessage.h"

template <typename MsgType>
class INetMessageBodyDeserializer : public std::enable_shared_from_this<INetMessageBodyDeserializer<MsgType>>
{
public:
    virtual shared_ptr<NetMessage<MsgType>> deserializeBody(NetMessageHeader<MsgType> header, const char* bodyBuffer) = 0;
};
