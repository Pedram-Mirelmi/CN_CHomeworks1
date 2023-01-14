#pragma once
#include <iostream>

#include "./AsyncNetFramework/AbstractNetIOManager.h"

#include "NetMessages/MessagesTypes.hpp"
#include "./NetMessages/AllNetMessages.hpp"
#include "./IResponseResolver.h"

using  namespace std;

class NetworkHandler : public AbstractNetIOManager<MessageTypes>
{
public:
    NetworkHandler(const std::string& ip, uint16_t port, std::string user_name)
        : AbstractNetIOManager<MessageTypes>(ip, port)
    {
        this->user_name = user_name;
        this->ip = ip;
        this->port = port;
    }
private:

    std::string user_name;
    std::string ip;
    uint16_t port;
    vector<uint16_t> user_ids;
    vector<std::string> user_names;
    bool server_acknowledged = false;


    shared_ptr<IResponseResolver> m_responseResolver;
protected:

    void onNewMessageReadCompletely() override {   

        shared_ptr<NetMessage<MessageTypes>> msg;
        switch (m_tempHeader.getMessageType())
        {
            case MessageTypes::CONNACK:
            {
                // std::cout << "CONNACK \n";
                this->server_acknowledged = true;
                break;
            }
            case MessageTypes::RECEIVEREPLY:
            {
                // std::cout << "RECEIVEREPLY \n";
                ReceiveReplyMessage message;
                message.deserialize(m_messageInBuff.data());

                if(message.getSenderId() != 0)
                    show_receive_message(message);
                break;
            }
            case MessageTypes::INFOREPLY:
            {
                // std::cout << "INFOREPLY \n";
                UserInfoReplyMessage message;
                message.deserialize(m_messageInBuff.data());
                this->user_names.push_back(message.get_user_name());
                std::cout << "   - " << message.get_user_name() << "\n";

                if(this->user_names.size() < this->user_ids.size()) {
                    uint16_t next_id = this->user_names.size() ;
                    get_info(this->user_ids[next_id]);
                }
                break;
            }
            case MessageTypes::SENDREPLY:
            {
                // std::cout << "SENDREPLY \n";
                break;
            }
            case MessageTypes::LISTREPLY:
            {
                // std::cout << "LISTREPLY \n";
                ListReplyMessage message;
                message.deserialize(m_messageInBuff.data());
                this->user_ids.clear();
                this->user_ids = message.getUsers();
                if (this->user_ids.size() != 0)
                    get_info(this->user_ids[0]);
                break;
            }
            default:
            {
                std::cout << "[ERROR] Server send a message that not supported\n";
            }
        }
    }


    void onConnected() {
        std::cout << "[INFO] Connected to " << this->ip << " on port " << this->port << "\n";

        send_username(this->user_name);
        std::cout << "[INFO] Waiting for server to ACCEPT you ...\n";

    }

    void onDisconnected() override
    {
        AbstractNetIOManager::onDisconnected();
        std::cout << "disconnected from server!" << std::endl;
    }

public:


    void wait_to_connect() {
        while(this->m_isConnected == false)
            std::this_thread::sleep_for(500ms);
    }

    void wait_to_acknowledge() {
        while(this->server_acknowledged == false)
            std::this_thread::sleep_for(500ms);
    }


    void receive_all_message() {
        ReceiveMessage* rec_mess = new ReceiveMessage();
        shared_ptr<_BNetMsg> rec_msg(static_cast<_BNetMsg*>(rec_mess));
        writeMessage(rec_msg);
    }

    void update_user_list() {
        ListMessage* list_msg = new ListMessage();
        shared_ptr<_BNetMsg> msg(static_cast<_BNetMsg*>(list_msg));

        this->user_names.clear();
        this->user_ids.clear();
        writeMessage(msg);

    }

    void get_info(uint16_t id) {
        UserInfoMessage* list_msg = new UserInfoMessage(id);
        shared_ptr<_BNetMsg> msg(static_cast<_BNetMsg*>(list_msg));
        writeMessage(msg);
    }

    void print_user_list() {
        for (auto name : this->user_names)
            std::cout << "  - " << name << "\n";
    }


    void send_message(std::string& receiver_name, std::string& message) {
        uint16_t id = get_id_by_name(receiver_name);
        if (id == 0) {
            std::cout << "[ERROR] User does not exists you can update the user list by 'list' command\n";
            return;
        }
        SendMessage* send_msg = new SendMessage(id, message.c_str());
        shared_ptr<_BNetMsg> msg(static_cast<_BNetMsg*>(send_msg));
        writeMessage(msg);
    }

private:

    void send_username(std::string& user_name) {
        ConnectMessage* usernameMsg = new ConnectMessage(user_name.c_str());
        shared_ptr<_BNetMsg> msg(static_cast<_BNetMsg*>(usernameMsg));
        writeMessage(msg);
    }

    void show_receive_message(ReceiveReplyMessage& message) {
        std::string sender_name = get_name_by_id(message.getSenderId());
        std::cout << "<< " << sender_name << " : " << message.getMessage() << "\n";
        if (sender_name == "[UnKnown]")
            std::cout << "[ERROR] You can fetch all names by 'list' command\n";
        
    }

    std::string get_name_by_id(int id) {
        for(int i=0; i < this->user_names.size(); i++)
            if (this->user_ids[i] == id)
                return this->user_names[i];

        return "[UnKnown]";   //not exist
    }

    uint16_t get_id_by_name(std::string& user_name) {
        for(int i=0; i < this->user_names.size(); i++)
            if (this->user_names[i] == user_name)
                return this->user_ids[i];

        return 0;   //not exist
    }
    

};
