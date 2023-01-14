#pragma once
#include "./NetMessages/ChatServerMessagesTypes.hpp"
#include "AsyncServerFramework/io/BasicNetMessage.h"
#include "AsyncServerFramework/io/INetMessageDeserializer.h"
#include "AsyncServerFramework/Session.h"
#include "INetMessageProcessor.hpp"
#include "./AsyncServerFramework/AbstractNetIOManager.h"

typedef NetMessage<MessageTypes> _BNetMsg;
typedef shared_ptr<_BNetMsg> _BNetMsg_ptr;

typedef NetMessageHeader<MessageTypes> _Header;

typedef INetMessageDeserializer<MessageTypes> _MsgDeserializer ;
typedef shared_ptr<_MsgDeserializer> _MD_ptr ;

typedef shared_ptr<IMainManager> _MM_ptr ;

typedef Session<MessageTypes> _Session;
typedef shared_ptr<_Session> _Sesstion_ptr;

typedef shared_ptr<AbstractNetIOManager<MessageTypes>> _NetIOManager_ptr;

typedef uint16_t _UserId_T;
