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

    bool all_ready = false;
    bool wait = false;
    bool last_message_read = false;
    MessageTypes target_message;    

    shared_ptr<IResponseResolver> m_responseResolver;
protected:

    void onNewMessageReadCompletely() override {   
         shared_ptr<NetMessage<MessageTypes>> msg;
        switch (m_tempHeader.getMessageType())
        {
            case MessageTypes::CONNACK:
            {
                release_wait(MessageTypes::CONNACK);
                break;
            }
            case MessageTypes::RECEIVEREPLY:
            {
                ReceiveReplyMessage message;
                message.deserialize(m_messageInBuff.data());
                if(message.getSenderId() == 0)
                    this->last_message_read = true;
                else
                    show_receive_message(message);

                release_wait(MessageTypes::RECEIVEREPLY);
                break;
            }
            case MessageTypes::INFOREPLY:
            {
                UserInfoReplyMessage message;
                message.deserialize(m_messageInBuff.data());
                this->user_names.push_back(message.get_user_name());
                release_wait(MessageTypes::INFOREPLY);
                break;
            }
            case MessageTypes::SENDREPLY:
            {
                release_wait(MessageTypes::SENDREPLY);
                break;
            }
            case MessageTypes::LISTREPLY:
            {
                ListReplyMessage message;
                message.deserialize(m_messageInBuff.data());
                this->user_ids.clear();
                this->user_ids = message.getUsers();
                release_wait(MessageTypes::LISTREPLY);
                break;
            }
        }

        m_responseResolver->resolveResponse(msg);
    }


    void onConnected() {
        std::cout << "[INFO] Connected to " << this->ip << " on port " << this->port << "\n";

        // fist seding user name
        send_username(this->user_name);
        std::cout << "[INFO] Waiting for server to ACCEPT you ...\n";
        this->wait_for(MessageTypes::CONNACK);
        wait_to_receive();
        cout << "------------------------------\n\n";
        cout << "Hello \"" << this->user_name << "\"\n";
        std::cout << "[INFO] Recieving messeges ...\n";
        receive_all_message();
        this->all_ready = true;
    }

    void onDisconnected() override
    {
        AbstractNetIOManager::onDisconnected();
        std::cout << "disconnected from server!" << std::endl;
    }

private:
    bool is_waiting() {
        return this->wait;
    }

    void wait_for(MessageTypes type) {
        this->wait = true;
        this->target_message = type;
    }

    void release_wait(MessageTypes type) {
        if(this->target_message == type)
            this->wait = false;
    }

    void wait_to_receive() {
        for(;;)
            if(this->is_waiting())
                std::this_thread::sleep_for(500ms);
            else
                break;
    }
    
    void send_username(std::string& user_name) {
        ConnectMessage* usernameMsg = new ConnectMessage(user_name.c_str());
        shared_ptr<_BNetMsg> msg(static_cast<_BNetMsg*>(usernameMsg));
        writeMessage(msg);
    }

    void receive_all_message() {
        ReceiveMessage* rec_mess = new ReceiveMessage();
        shared_ptr<_BNetMsg> rec_msg(static_cast<_BNetMsg*>(rec_mess));

        this->last_message_read = false;
        std::string sender_name;
        while(this->last_message_read == false){
            writeMessage(rec_msg);

            wait_for(MessageTypes::RECEIVEREPLY);
            wait_to_receive();
        }
    }

    void show_receive_message(ReceiveReplyMessage& message) {
        std::string sender_name = get_name_by_id(message.getSenderId());
        std::cout << "<< " << sender_name << " : " << message.getMessage() << '\n';
    }

    std::string get_name_by_id(int id) {
        for(int i=0; i < this->user_names.size(); i++)
            if (this->user_ids[i] == id)
                return this->user_names[i];

        return "";   //not exist
    }



    uint16_t get_id_by_name(std::string& user_name) {
        for(int i=0; i < this->user_names.size(); i++)
            if (this->user_names[i] == user_name)
                return this->user_ids[i];

        return -1;   //not exist
    }
    void get_info(uint16_t id) {
        UserInfoMessage* list_msg = new UserInfoMessage(id);
        shared_ptr<_BNetMsg> msg(static_cast<_BNetMsg*>(list_msg));
        writeMessage(msg);

        wait_for(MessageTypes::INFOREPLY);
        wait_to_receive();
    }


public:
    void wait_to_ready() {
        for(;;)
            if(this->all_ready) 
                break;
            else
                std::this_thread::sleep_for(500ms);
    }

    void update_user_list() {
        ListMessage* list_msg = new ListMessage();
        shared_ptr<_BNetMsg> msg(static_cast<_BNetMsg*>(list_msg));

        writeMessage(msg);

        wait_for(MessageTypes::LISTREPLY);
        wait_to_receive();

        this->user_names.clear();
        for (uint16_t id : this->user_ids) {
            get_info(id);
        }
    }

    void print_user_list() {
        for (auto name : this->user_names)
            cout << "   - " << name << endl;
    }



    void send_message(std::string& receiver_name, std::string& message) {
        uint16_t id = get_id_by_name(receiver_name);
        if (id == -1) {
            std::cout << "[ERROR] User does not exists you can update the user list by 'list' command\n";
            return;
        }
        SendMessage* send_msg = new SendMessage(id, message.c_str());
        shared_ptr<_BNetMsg> msg(static_cast<_BNetMsg*>(send_msg));
        writeMessage(msg);
    }



};
