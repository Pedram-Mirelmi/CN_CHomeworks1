#pragma once

#include "./ISerializable.h"


template <typename MsgType>
class NetMessageHeader : public ISerializable, public std::enable_shared_from_this<NetMessageHeader<MsgType>>
{
protected:
    MsgType messageType;
    size_t messageBodySize;
public:


    // ISerializable interface
    virtual void deserialize(char *buffer) override
    {
        this->messageType = *(reinterpret_cast<MsgType*>(buffer));
        buffer += sizeof (MsgType);

        this->messageBodySize = *(reinterpret_cast<size_t*>(buffer));
    }

    virtual void serialize(char *buffer) const override
    {
        *(reinterpret_cast<MsgType*>(buffer)) = this->messageType;
        buffer += sizeof (MsgType);

        *(reinterpret_cast<size_t*>(buffer)) = this->messageBodySize;
    }

    virtual size_t calculateNeededSizeForThis() const override
    {
        return sizeof(*this);
    }

    const size_t& getBodySize() const
    {
        return messageBodySize;
    }
    const MsgType &getMessageType() const
    {
        return messageType;
    }
};




template <typename MsgType>
class NetMessage : public ISerializable, public std::enable_shared_from_this<NetMessage<MsgType>>
{
protected:
    NetMessageHeader<MsgType> m_header;
public:
    virtual const MsgType& getMessageType() const = 0;
    const NetMessageHeader<MsgType> &getHeader() const
    {
        return m_header;
    }
};


