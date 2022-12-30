#pragma once

#include "./ISerializable.h"


template <typename MsgType>
class BasicNetMessage;

template <typename MsgType>
class NetMessageHeader : public ISerializable,
                         public std::enable_shared_from_this<NetMessageHeader<MsgType>>
{
protected:
    MsgType m_messageType;
    uint32_t m_messageBodySize;
    friend BasicNetMessage<MsgType>;
public:
    NetMessageHeader() = default;
    NetMessageHeader(MsgType msgType, const uint32_t& msgBodySize)
        :ISerializable(),
         std::enable_shared_from_this<NetMessageHeader<MsgType>>(),
         m_messageType(msgType),
         m_messageBodySize(msgBodySize)
    {}

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
class BasicNetMessage : public ISerializable, public std::enable_shared_from_this<BasicNetMessage<MsgType>>
{
protected:
    NetMessageHeader<MsgType> m_header;
public:
    BasicNetMessage() = default;

    BasicNetMessage(MsgType msgType, uint32_t bodySize)
        :m_header(msgType, bodySize)
    {}

    virtual const MsgType& getMessageType() const = 0;

    const NetMessageHeader<MsgType> &getHeader() const
    {
        return m_header;
    }
};


