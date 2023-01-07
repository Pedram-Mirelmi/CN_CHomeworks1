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
    void write_short_response(u_int16_t responseNumber, shared_ptr<Session<NetMessageType>> session) {
        ShortResponseMessage* usernameMsg = new ShortResponseMessage(responseNumber);
        shared_ptr<_BNetMsg> msg(static_cast<_BNetMsg*>(usernameMsg));
        this->m_netIoManager.writeMessage(msg ,session);  
    }

    void processNetMessage(shared_ptr<NetMessage<NetMessageType>> netMsg, NetMessageType msg_type,
                                    shared_ptr<Session<NetMessageType>> session) override
    {

        std::cout << "processNetMessage: \n";
        switch (msg_type)
        {
            case NetMessageType::USERNAME:
            {
                std::cout << "USERNAME\n";
                auto newNetMsg = std::dynamic_pointer_cast<UsernameMessage>(netMsg);
                string username = newNetMsg->get_username();
                int id = get_user(config.users, username);
                std::cout << username << '\n';
                if (id == -1){
                    write_short_response(430, session);
                    std::cout << "User not exist\n";
                }
                else{
                    session->set_user(config.users[id]);
                    write_short_response(331, session);
                    std::cout << "User found\n";
                }
                break;
            }
            case NetMessageType::PASSWORD:
            {
                std::cout << "PSSWORD\n";
                auto newNetMsg = std::dynamic_pointer_cast<PasswordMessage>(netMsg);
                string password = newNetMsg->get_password();
                if (session->check_password(password)) {
                    write_short_response(230, session);
                    std::cout << "user loged in\n";
                }
                else{
                    write_short_response(430, session);
                    std::cout << "wrong password\n";
                }
                break;
            }
            case NetMessageType::DOWNLOAD_FILE:
            {
                std::cout << "DOWNLOAD\n";
                auto newNetMsg = std::dynamic_pointer_cast<DownloadFileMessage>(netMsg);
                string file_name = newNetMsg->get_file_name();
                // TODO: save file
                break;
            }
            case NetMessageType::UPLOAD_FILE:
            {
                std::cout << "UPLOAD_FILE\n";
                auto newNetMsg = std::dynamic_pointer_cast<UploadFileMessage>(netMsg);
                string file_name = newNetMsg->get_file_name();
                // TODO: load file and send
                break;
            }
            case NetMessageType::HELP_CONTENT:
            {
                std::cout << "HELP_CONTENT\n";
                auto newNetMsg = std::dynamic_pointer_cast<HelpContentMessage>(netMsg);
                HelpContentMessage* help = new HelpContentMessage();
                shared_ptr<_BNetMsg> msg(static_cast<_BNetMsg*>(help));
                this->m_netIoManager.writeMessage(msg ,session);
                break;
            }
            case NetMessageType::QUIT:
            {
                std::cout << "QUIT\n";
                write_short_response(221, session);
                break;
            }
            
            default:
            {
                cout << "not any of the messages\n";
                break;
            }
        }
    }
};
