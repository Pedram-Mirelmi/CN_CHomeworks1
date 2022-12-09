#pragma once

#include <string>
#include <cstring>
#include "./NetMessageTypes.h"
#include "../io/BasicNetMessage.h"


class SimpleTextMessage : public NetMessage<DemoNetMessageType>
{
public:
    std::string m_text;
    SimpleTextMessage(const std::string& requestStr)
        : NetMessage<DemoNetMessageType>(DemoNetMessageType::TEXT_MESSAGE, requestStr.size()), m_text(requestStr)
    {

    }
    void deserialize(char *buffer) override
    {
        this->m_header.deserialize(buffer);

        buffer += this->m_header.calculateNeededSizeForThis();
        m_text = std::string(buffer, this->m_header.getBodySize());

    }
    void serialize(char *buffer) const override
    {
        this->m_header.serialize(buffer);
        buffer += this->m_header.calculateNeededSizeForThis();

        std::memcpy(buffer, m_text.data(), m_text.size());
    }
    uint32_t calculateNeededSizeForThis() const override
    {
        return this->m_header.calculateNeededSizeForThis() + m_text.size();
    }

    // NetMessage interface
public:
    const DemoNetMessageType &getMessageType() const override
    {
        return DemoNetMessageType::TEXT_MESSAGE;
    }
};
