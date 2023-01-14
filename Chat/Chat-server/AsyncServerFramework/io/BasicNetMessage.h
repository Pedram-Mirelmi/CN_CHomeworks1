#pragma once

#include "./ISerializable.h"


template <typename MsgType>
class NetMessage;

template <typename MsgType>
class NetMessageHeader : public ISerializable, public std::enable_shared_from_this<NetMessageHeader<MsgType>>
{
protected:
    MsgType m_messageType;
    uint32_t m_messageBodySize;
    friend NetMessage<MsgType>;
public:
    NetMessageHeader() = default;
    NetMessageHeader(MsgType msgType, const uint32_t& msgBodySize)
        :ISerializable(),
         std::enable_shared_from_this<NetMessageHeader<MsgType>>(),
         m_messageType(msgType),
         m_messageBodySize(msgBodySize)
    {

    }

    // ISerializable interface
    virtual void deserialize(char *buffer) override
    {
        ISerializable::deserializePrimitiveType<MsgType>(buffer, m_messageType);
        ISerializable::deserializePrimitiveType<uint32_t>(buffer, m_messageBodySize, false);
    }

    virtual void serialize(char *buffer) const override
    {
        ISerializable::serializePrimitiiveType<MsgType>(buffer, m_messageType);
        ISerializable::serializePrimitiiveType<uint32_t>(buffer, m_messageBodySize, false);
    }

    virtual uint32_t calculateNeededSizeForThis() const override
    {
        return NetMessageHeader<MsgType>::getHeaderSize();
    }

    static uint32_t getHeaderSize()
    {
        return sizeof(MsgType) + sizeof (uint32_t);
    }

    const uint32_t& getBodySize() const
    {
        return m_messageBodySize;
    }
    const MsgType &getMessageType() const
    {
        return m_messageType;
    }
};




template <typename MsgType>
class NetMessage : public ISerializable, public std::enable_shared_from_this<NetMessage<MsgType>>
{
protected:
    NetMessageHeader<MsgType> m_header;
public:
    virtual MsgType getMessageType() const = 0;
    NetMessage() = default;
    NetMessage(MsgType msgType, uint32_t bodySize)
        :m_header(msgType, bodySize)
    {}
    const NetMessageHeader<MsgType> &getHeader() const
    {
        return m_header;
    }
};


