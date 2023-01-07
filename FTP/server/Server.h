#pragma once

#include "AsyncServerFramework/IService.h"
#include "NetMessages/AllNetMessages.hpp"
#include "./INetMessageProcessor.h"
#include "./MessageBodyDeserializer.h"
#include "./NetIOManager.h"
#include <iostream>
#include <Config.h>

using std::string;

class Server : public IService,
                       public INetMessageProcessor<NetMessageType>,
                       public std::enable_shared_from_this<Server>
{
    Config config;

    NetIOManager m_netIoManager;
    std::vector<string> user_names;
    std::vector<string> passwords;
public:
    Server(const string& ip, uint16_t port, int netIOThreadsCount, Config& config)
        : INetMessageProcessor<NetMessageType>(),
          m_netIoManager(ip, port, netIOThreadsCount)
    {
        auto deserializer = shared_ptr<INetMessageBodyDeserializer<NetMessageType>>(static_cast<INetMessageBodyDeserializer<NetMessageType>*>(new MessageBodyDeserializer()));
        m_netIoManager.setNetMessageDeserializer(deserializer);

        auto processor = shared_ptr<INetMessageProcessor>(static_cast<INetMessageProcessor*>(this));
        m_netIoManager.setNetMessageProcessor(processor);
        this->config = config;
    }

    // IService interface
public:
    void start() override
    {
        m_netIoManager.start();
    }
    void stop() override
    {
        m_netIoManager.stop();
    }

    void read_json(const string file_name) {

    }

    // INetMessageProcessor interface
public:
    virtual void processNetMessage(shared_ptr<NetMessage<NetMessageType>> netMsg, shared_ptr<Session<NetMessageType>> session) override
    {

        std::cout << "processNetMessage: \n";
        switch (netMsg->getMessageType())
        {
            case NetMessageType::USERNAME:
            {
                auto newNetMsg = std::dynamic_pointer_cast<UsernameMessage>(netMsg);
                string username = newNetMsg->get_username();
                int i = get_user(config.users, username);
                if (i == -1){
                    // TODO: return error
                    std::cout << "user not exist\n";
                }
                else{
                    session->set_user(config.users[i]);
                    std::cout << "user found\n";
                    // TODO: return message
                }
                break;
            }
            case NetMessageType::PASSWORD:
            {
                auto newNetMsg = std::dynamic_pointer_cast<PasswordMessage>(netMsg);
                string password = newNetMsg->get_password();
                if (session->check_password(password));
                    // TODO: sond message
                else;
                    // TODO: send error
                break;
            }
            case NetMessageType::DOWNLOAD_FILE:
            {
                
            }
            default:
                break;
            }

        // std::cout << netMsg->
    }
};
