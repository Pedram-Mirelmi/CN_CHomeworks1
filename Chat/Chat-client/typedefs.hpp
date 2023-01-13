#pragma once
#include "NetMessages/MessagesTypes.hpp"
#include "AsyncNetFramework/io/BasicNetMessage.h"
// #include "AsyncNetFramework/io/INetMessageDeserializer.h"
// #include "AsyncNetFramework/Session.h"
// #include "INetMessageProcessor.hpp"
#include "./AsyncNetFramework/AbstractNetIOManager.h"

typedef NetMessage<MessageTypes> _BNetMsg;
typedef shared_ptr<_BNetMsg> _BNetMsg_ptr;

typedef NetMessageHeader<MessageTypes> _Header;

// typedef INetMessageDeserializer<MessageTypes> _MsgDeserializer ;
// typedef shared_ptr<_MsgDeserializer> _MD_ptr ;

// typedef shared_ptr<IMainManager> _MM_ptr ;

// typedef Session<MessageTypes> _Session;
// typedef shared_ptr<_Session> _Sesstion_ptr;

typedef shared_ptr<AbstractNetIOManager<MessageTypes>> _NetIOManager_ptr;

typedef uint16_t _UserId_T;
