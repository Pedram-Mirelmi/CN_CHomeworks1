#pragma once

#include "AsyncServerFramework/IService.h"
#include "NetMessages/AllNetMessages.hpp"
#include "./INetMessageProcessor.h"
#include "./MessageBodyDeserializer.h"
#include "./NetIOManager.h"
#include <iostream>
#include <fstream>
#include <filesystem>
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

private:

    bool file_exist(const string& file_path) {
        std::filesystem::path f{file_path};
        if (std::filesystem::exists(f))
            return true;
        return false;
    }

    std::vector<char> read_whole_file(const string& path) {
        std::ifstream input_file(path);
        if (!input_file.is_open())
            exit(EXIT_FAILURE);
        string s = string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
        std::vector<char> v(s.begin(), s.end());
        input_file.close();
        return v;
    }

    void save_to_file(string file_name, std::vector<char> content) {
        std::ofstream file (file_name);
        for (char c : content)
            file << c;
        
        file.close();
    }


public:
    void start() override
    {
        m_netIoManager.start();
    }
    void stop() override
    {
        m_netIoManager.stop();
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

        std::cout << "[INFO] User \"" << session->get_client_port() << "\" send a message:\n";
        if (session->is_autherized() == false){
            switch (msg_type)
            {
                case NetMessageType::USERNAME:
                {
                    std::cout << "  [MESSAGE TYPE] USERNAME: ";
                    auto newNetMsg = std::dynamic_pointer_cast<UsernameMessage>(netMsg);
                    string username = newNetMsg->get_username();
                    int id = get_user(config.users, username);
                    std::cout << username << '\n';
                    if (id == -1){
                        write_short_response(430, session);
                        std::cout << "  [ERROR] User not exist\n";
                    }
                    else{
                        session->set_user(config.users[id]);
                        write_short_response(331, session);
                        std::cout << "  [INFO] User found\n";
                    }
                    break;
                }
                case NetMessageType::PASSWORD:
                {
                    std::cout << "  [MESSAGE TYPE] PSSWORD: ";
                    auto newNetMsg = std::dynamic_pointer_cast<PasswordMessage>(netMsg);
                    string password = newNetMsg->get_password();
                    std::cout << password << '\n';
                    if (session->check_password(password)) {
                        write_short_response(230, session);
                        std::cout << "  [INFO] User loged in\n";
                    }
                    else{
                        write_short_response(430, session);
                        std::cout << "  [ERROR] wrong password\n";
                    }
                    break;
                }
                default:
            {
                cout << "[ERROR] Message type not support\n";
                write_short_response(332, session);
                return;
                break;
            }
            }
        }
        else {

            switch (msg_type)
            {
                
                case NetMessageType::DOWNLOAD_FILE:
                {
                    std::cout << "  [MESSAGE TYPE] DOWNLOAD: ";
                    auto newNetMsg = std::dynamic_pointer_cast<DownloadFileMessage>(netMsg);
                    string file_name = newNetMsg->get_file_name();
                    string file_path = "./Files/" + file_name;
                    std::cout << file_name << endl;

                    if (file_exist(file_path) == false){
                        std::cout << "  [ERROR] File not exists\n";
                        write_short_response(500, session);
                        return;
                    }
                    
                    if (is_admin_file(config.admin_files, file_name) and !session->is_admin()) {
                        std::cout << "  [ERROR] Only admin can download this file\n";
                        write_short_response(550, session);
                        return;
                    }


                    // send file
                    std::vector<char> whole_file = read_whole_file(file_path);
                    std::cout << "  [INFO] File size is : " << whole_file.size() << " KB\n";
                    if (!session->download(whole_file.size())){
                        std::cout << "  [ERROR] Don't have enough data to download!\n";
                        write_short_response(425, session);
                        return;
                    }
                    std::cout << "  [INFO] Sending file ... \n";
                    FileContentMessage* file_content = new FileContentMessage(whole_file);
                    shared_ptr<_BNetMsg> msg(static_cast<_BNetMsg*>(file_content));
                    this->m_netIoManager.writeMessage(msg ,session);
                    write_short_response(226, session);
                    break;
                }
                case NetMessageType::UPLOAD_FILE:
                {
                    std::cout << "  [MESSAGE TYPE] UPLOAD_FILE: ";
                    auto newNetMsg = std::dynamic_pointer_cast<UploadFileMessage>(netMsg);
                    string file_name = newNetMsg->get_file_name();
                    std::cout << file_name << endl;
                    if(session->is_admin()) {
                        write_short_response(226, session);
                        session->set_next_upload();
                    }
                    else {
                        write_short_response(500, session); // simple error
                    }
                    break;
                }
                case NetMessageType::FILE_CONTENT:
                {
                    std::cout << "  [MESSAGE TYPE] FILE_CONTENT\n";
                    auto newNetMsg = std::dynamic_pointer_cast<FileContentMessage>(netMsg);
                    std::vector<char> file_content = newNetMsg->get_file_content();
                    if (session->can_upload()) {
                        string file_name = session->get_file_name();
                        save_to_file(file_name, file_content);
                        write_short_response(226, session);
                    }
                    else {
                        write_short_response(500, session);
                    }


                }
                case NetMessageType::HELP_CONTENT:
                {
                    std::cout << "  [MESSGAE TYPE] HELP_CONTENT\n";
                    auto newNetMsg = std::dynamic_pointer_cast<HelpContentMessage>(netMsg);
                    HelpContentMessage* help = new HelpContentMessage();
                    shared_ptr<_BNetMsg> msg(static_cast<_BNetMsg*>(help));
                    this->m_netIoManager.writeMessage(msg ,session);
                    break;
                }
                case NetMessageType::QUIT:
                {
                    std::cout << "  [MESSAGE TYPE] QUIT\n";
                    write_short_response(221, session);
                    break;
                }
                
                default:
                {
                    cout << "   [ERROR] Message type not support or user want to login again\n";
                    write_short_response(500, session);
                    break;
                }
            }
        }
    }
};
