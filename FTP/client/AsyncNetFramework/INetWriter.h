#pragma once
#include <memory>
#include "./io/BasicNetMessage.h"


template <typename MsgType>
class INetWriter : public std::enable_shared_from_this<INetWriter<MsgType>>
{
public:
    virtual void writeMessage(std::shared_ptr<BasicNetMessage<MsgType>> msg) = 0;
};
