#pragma once
#include <iostream>

#include "./AsyncNetFramework/AbstractNetIOManager.h"

#include "NetMessages/MessagesTypes.hpp"
#include "./NetMessages/AllNetMessages.hpp"
#include "./IResponseResolver.h"

class NetworkHandler : public AbstractNetIOManager<MessageTypes>
{
public:
    NetworkHandler(const std::string& ip, uint16_t port)
        : AbstractNetIOManager<MessageTypes>(ip, port)
    {

    }
private:
    bool conn_ack = false; 

    shared_ptr<IResponseResolver> m_responseResolver;
protected:
    void onNewMessageReadCompletely() override
    {
        shared_ptr<NetMessage<MessageTypes>> msg;
        switch (m_tempHeader.getMessageType())
        {
            case MessageTypes::CONNACK:
            {
                msg.reset(static_cast<NetMessage<MessageTypes>*>(new ConnAckMessage()));
                conn_ack = true;
                break;
            }
        //     case MessageTypes::SHORT_RESPONSE:
        //     {
        //         msg.reset(static_cast<NetMessage<MessageTypes>*>(new ShortResponseMessage()));
        //         break;
        //     }
        //     case MessageTypes::FILE_CONTENT:
        //     {
        //         msg.reset(static_cast<NetMessage<MessageTypes>*>(new FileContentMessage()));
        //         break;
        //     }
        //     case MessageTypes::HELP_CONTENT:
        //     {
        //         msg.reset(static_cast<NetMessage<MessageTypes>*>(new HelpContentMessage()));
        //         break;
        //     }
        }
        msg->deserialize(m_messageInBuff.data());
        m_responseResolver->resolveResponse(msg);
    }

    void onDisconnected() override
    {
        AbstractNetIOManager::onDisconnected();
        std::cout << "disconnected from server!" << std::endl;
    }


public:
    //

    bool does_server_accept() {
        return this->conn_ack;
    }

    void send_username(std::string& user_name) {
        ConnectMessage* usernameMsg = new ConnectMessage(user_name.c_str());
        shared_ptr<_BNetMsg> msg(static_cast<_BNetMsg*>(usernameMsg));
        writeMessage(msg);
    }

};
