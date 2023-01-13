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

    shared_ptr<IResponseResolver> m_responseResolver;
protected:
    void onNewMessageReadCompletely() override {   

        // we only get recieve messages async all othe are sync
        if(m_tempHeader.getMessageType() != MessageTypes::RECEIVEREPLY)     //TODO: how message recieves ????
            return;


        ReceiveReplyMessage* msg = new ReceiveReplyMessage();
        msg->deserialize(m_messageInBuff.data());

        std::string sender_name = get_name_by_id(msg->getSenderId());
        std::cout << "<< " << sender_name << " : " << msg->getMessage() << '\n';
    }


    void onConnected() {
        std::cout << "[INFO] Connected to " << this->ip << " on port " << this->port << "\n";

        // fist seding user name
        send_username(this->user_name);
        std::cout << "[INFO] Waiting for server to ACCEPT you ...\n";
        readSyncMessage();
        if(m_tempHeader.getMessageType() != MessageTypes::CONNACK)
            throw exception();

        // second receiving messages
        std::cout << "[INFO] Recieving messeges ...\n";
        receive_all_message();
        
        
        // now read async for receive messages
        asyncRead();                                    //TODO: how handel recieve messages

    }

    void onDisconnected() override
    {
        AbstractNetIOManager::onDisconnected();
        std::cout << "disconnected from server!" << std::endl;
    }

public:
    
    void send_username(std::string& user_name) {
        ConnectMessage* usernameMsg = new ConnectMessage(user_name.c_str());
        shared_ptr<_BNetMsg> msg(static_cast<_BNetMsg*>(usernameMsg));
        writeSyncMessage(msg);
    }

    void receive_all_message() {
        ReceiveMessage* rec_mess = new ReceiveMessage();
        shared_ptr<_BNetMsg> rec_msg(static_cast<_BNetMsg*>(rec_mess));
        ReceiveReplyMessage* message = new ReceiveReplyMessage();
        std::string sender_name;
        for(;;){
            writeSyncMessage(rec_msg);
            readSyncMessage();
            if(m_tempHeader.getMessageType() != MessageTypes::RECEIVEREPLY)
                throw exception();

            message->deserialize(m_messageInBuff.data());
            if (message->getSenderId() == 0)                // no more message
                return;

            sender_name = get_name_by_id(message->getSenderId());
            std::cout << "<< " << sender_name << " : " << message->getMessage() << '\n';
        }
    }

    std::string get_name_by_id(int id) {
        UserInfoMessage* info = new UserInfoMessage(id);
        shared_ptr<_BNetMsg> msg(static_cast<_BNetMsg*>(info));
        writeSyncMessage(msg);   
        readSyncMessage();
        UserInfoReplyMessage* user_info = new UserInfoReplyMessage();
        user_info->deserialize(m_messageInBuff.data());
        return user_info->get_user_name();
    }


    void get_user_list() {
        ListMessage* list_msg = new ListMessage();
        shared_ptr<_BNetMsg> msg(static_cast<_BNetMsg*>(list_msg));
        ListReplyMessage* message = new ListReplyMessage();
        writeSyncMessage(msg);
        readSyncMessage();
        if(m_tempHeader.getMessageType() != MessageTypes::LISTREPLY)
                throw exception();

        // get user ids
        message->deserialize(m_messageInBuff.data());
        this->user_ids.clear();
        this->user_ids = message->getUsers();
        
        // get user names by user ids
        this->user_names.clear();
        for (uint16_t i : user_ids) {
            std::string name = get_name_by_id(i);
            std::cout << "   - " << name << endl;
            this->user_names.push_back(name);
        }
    }

    void send_message(std::string& receiver_name, std::string& message) {
        uint16_t id = get_id_by_name(receiver_name);
        if (id == -1) {
            std::cout << "[ERROR] User does not exists you can update the user list by list command\n";
            return;
        }

        SendMessage* send_msg = new SendMessage(id, message.c_str());
        shared_ptr<_BNetMsg> msg(static_cast<_BNetMsg*>(send_msg));
        SendReplyMessage* status = new SendReplyMessage();
        writeSyncMessage(msg);
        readSyncMessage();
        if(m_tempHeader.getMessageType() != MessageTypes::SENDREPLY)
                throw exception();

        status->deserialize(m_messageInBuff.data());
        if (status->getSuccess())
            std::cout << "[INFO] Message successfully sent\n";
        else
            std::cout << "[INFO] Message not sent successfully\n";   
    }

    uint16_t get_id_by_name(std::string& user_name) {
        for(int i=0; i < this->user_names.size(); i++)
            if (this->user_names[i] == user_name)
                return this->user_ids[i];

        return -1;   //not exist
    }


};
