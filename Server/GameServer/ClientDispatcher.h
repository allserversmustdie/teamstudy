#pragma once

#include "Dispatcher.h"

class ClientDispatcher : public IDispatcher
{
public:
	ClientDispatcher();
	virtual ~ClientDispatcher();

	virtual void OnConnected(Session* session);
	virtual void OnDisconnected(Session* session, int err);

	static int DISPATCHER_LOGIN_REQUEST(Session* session, Packet* packet);
};

