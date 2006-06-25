/***************************************************************************
						networking.cpp  -  description
								-------------------
	begin                : jeu dec 23 2004
	copyright            : (C) 2004-2006 by Duong-Khang NGUYEN
	email                : neoneurone @ users sourceforge net

	$Id$
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/

#include "netnode.h"
#include "networking.h"


   /*=====================================================================*/
Networking::Networking():
networkingCode( OC_NET_UNDEFINED ),
pSocketSet( NULL ),
pServerSocket( NULL )
{
	OPENCITY_DEBUG("ctor");

	boolNetworkInitialized = (SDLNet_Init() != -1);
	if (boolNetworkInitialized == false) {
		cerr << "WARNING: I couldn't initialize networking. "
			 << "The error was: " << SDLNet_GetError() << endl;
	}
}


   /*=====================================================================*/
Networking::~Networking()
{
	OPENCITY_DEBUG("dtor");

	//FIXME: free server ressources

	if (boolNetworkInitialized == true) {
		SDLNet_Quit();
		boolNetworkInitialized = false;
		networkingCode = OC_NET_UNDEFINED;
		pServerSocket = NULL;
	}
}


   /*=====================================================================*/
const string
Networking::GetClientHost(
	const uint index )
{
	assert( index < this->vClient.size() );
	return (string)(SDLNet_ResolveIP(&vClient[index].ip));
}


   /*=====================================================================*/
const uint
Networking::GetClientNum()
{
	return this->vClient.size();
}


   /*=====================================================================*/
const uint
Networking::GetClientMax()
{
	return OC_NET_CLIENT_MAX;
}


   /*=====================================================================*/
const OPENCITY_NET_CODE
Networking::GetMachineRole()
{
	return this->networkingCode;
}


   /*=====================================================================*/
const uint
Networking::GetPingTimeout()
{
	return OC_NET_PING_TIMEOUT;
}


   /*=====================================================================*/
const uint
Networking::GetServerNum()
{
	//TODO: STUB
	OPENCITY_DEBUG("TODO");
	assert(0);
	return 0;
}


   /*=====================================================================*/
const string
Networking::GetServerHost( const uint index )
{
	//TODO: STUB
	OPENCITY_DEBUG("TODO");
	assert(0);
	return "";
}


   /*=====================================================================*/
const string
Networking::GetVersion()
{
	return VERSION;
}


   /*=====================================================================*/
const string
Networking::GetProtocol()
{
	return OC_NET_PROTOCOL;
}


   /*=====================================================================*/
const OPENCITY_NET_CODE
Networking::Accept( uint & rid )
{
// IF the networking is not init THEN error
	if (this->boolNetworkInitialized == false)
		return OC_NET_ERROR;

// IF the server is not running THEN error
	if (this->pServerSocket == NULL)
		return OC_NET_SERVER_STOPED;

// IF the server is full THEN error
//	if (this->vClient.size() == OC_NET_CLIENT_MAX)
//		return OC_NET_SERVER_FULL;

// Create the new socket to handle the client
	Netnode nodeClient;
	nodeClient.socket = SDLNet_TCP_Accept(this->pServerSocket);
	if (nodeClient.socket != NULL) {
		NetMessage msg;
	// Put the client socket into the server's socket set for future use
		if (SDLNet_TCP_AddSocket( this->pSocketSet, nodeClient.socket ) == -1) {
		// Send NCK
			msg.cmd = OC_NET_NCK;
			(void)SDLNet_TCP_Send(nodeClient.socket, &msg, sizeof(NetMessage));
			SDLNet_TCP_Close(nodeClient.socket);
			rid = 0;

			return OC_NET_SERVER_FULL;
		}
		else {
			nodeClient.ip = *(SDLNet_TCP_GetPeerAddress(nodeClient.socket));
			vClient.push_back(nodeClient);
			rid = vClient.size()-1;

		// Send ACK
			msg.cmd = OC_NET_ACK;
			this->SendMessage( msg, rid );
		}
	}
	else {
		rid = 0;
		return OC_NET_NOCLIENT;
	}

	return OC_NET_OK;
}


   /*=====================================================================*/
const OPENCITY_NET_CODE
Networking::Reject( const uint id )
{
	OPENCITY_DEBUG( "TODO" );
	assert( 0 );
	return OC_NET_ERROR;
}


   /*=====================================================================*/
const OPENCITY_NET_CODE
Networking::StartServer( const OC_SUINT port )
{
// IF the server is already running THEN error
	if (pServerSocket != NULL) {
		return OC_NET_SERVER_STARTED;
	}

	IPaddress ip;

	if (SDLNet_ResolveHost(&ip, NULL, port) == -1) {
		cerr << "ERROR: I couldn't start the server: "
			 << SDLNet_GetError() << endl;
		return OC_NET_ERROR;
	}

	this->pServerSocket = SDLNet_TCP_Open(&ip);
	if (pServerSocket == NULL) {
		cerr << "ERROR: I couldn't open the server's socket: "
			 << SDLNet_GetError() << endl;
		return OC_NET_ERROR;
	}

// Allocate the socket set for the server
	this->pSocketSet = SDLNet_AllocSocketSet(OC_NET_CLIENT_MAX);
	if (this->pSocketSet == NULL) {
		cerr << "ERROR: I couldn't allocate a socket set: "
			 << SDLNet_GetError() << endl;
		return OC_NET_ERROR;
	}

	return OC_NET_OK;
}


   /*=====================================================================*/
const OPENCITY_NET_CODE
Networking::StopServer()
{
// IF the server is not running THEN error
	if (pServerSocket == NULL) {
		return OC_NET_SERVER_STOPED;
	}

	SDLNet_TCP_Close(this->pServerSocket);
	pServerSocket = NULL;

	SDLNet_FreeSocketSet(this->pSocketSet);
	this->pSocketSet = NULL;

	return OC_NET_OK;
}


   /*=====================================================================*/
const OPENCITY_NET_CODE
Networking::ProcessServerData()
{
	uint id;
	OPENCITY_NET_CODE netCode;
	NetMessage msg;
	ClientIter i;
	int recv;
	std::vector<ClientIter> vToClose;

// Accept an incoming connection and automatically ACK
	netCode = this->Accept( id );
	if (netCode == OC_NET_OK) {
		OPENCITY_DEBUG( "New number of clients: " << this->GetClientNum() );
	}
	else if (netCode == OC_NET_SERVER_FULL) {
		OPENCITY_DEBUG( "Server is full, new connection rejected" );
	}

// Check socket set for activity
// Otherwise return
	if (SDLNet_CheckSockets( this->pSocketSet, OC_NET_CHECK_TIMEOUT ) < 1)
		return OC_NET_OK;

// Process waiting messages sent by client
	OPENCITY_DEBUG( "Begin reading socket" );
	for (i = this->vClient.begin(); i != this->vClient.end(); i++) {
		if (SDLNet_SocketReady((*i).socket) != 0) {
			recv = SDLNet_TCP_Recv((*i).socket, &msg, sizeof(NetMessage) );

			if (recv > 0) {
			// Disconnect request ?
				if (msg.cmd == OC_NET_DNT) {
					OPENCITY_DEBUG( "One client disconnected." );
				// FIXME: send ACK
					vToClose.push_back( i );
				}
			}
			else {
				OPENCITY_DEBUG( "No data received or error. Closing client socket." );
				vToClose.push_back( i );
			}
		}
	}
	OPENCITY_DEBUG( "End reading socket" );

	uint c;
	for ( c = 0; c < vToClose.size(); c++ ) {
		this->Close(vToClose[c]);
	}

	return OC_NET_OK;
}


   /*=====================================================================*/
const OPENCITY_NET_CODE
Networking::Open(
	const string serverHost,
	const OC_SUINT port )
{
// IF we are already connected THEN
	if (this->pServerSocket != NULL)
		return OC_NET_CLIENT_CONNECTED;

// IF we can not resolve the host THEN error
	IPaddress ip;
	if (SDLNet_ResolveHost(&ip, serverHost.c_str(), port) == -1) {
		cerr << "ERROR: I couldn't resolve the server name: "
			 << SDLNet_GetError() << endl;
		return OC_NET_ERROR;
	}

// Open the server socket
	this->pServerSocket = SDLNet_TCP_Open(&ip);
	if (pServerSocket == NULL) {
		cerr << "ERROR: I couldn't connect to the server: "
			 << SDLNet_GetError() << endl;
		return OC_NET_ERROR;
	}

// Reading ACK/NCK
	NetMessage msg;
	OPENCITY_NET_CODE nc = OC_NET_OK;
	if (this->Receive( &msg, sizeof(NetMessage) ) == OC_NET_OK) {
		switch (msg.cmd) {
		case OC_NET_ACK:
			nc = OC_NET_CLIENT_ACCEPTED;
			break;

		case OC_NET_NCK:
		// Close the socket
			SDLNet_TCP_Close(this->pServerSocket);
			this->pServerSocket = NULL;
			nc = OC_NET_CLIENT_REJECTED;
			break;

		default:
			nc = OC_NET_ERROR;
		}
	}

	return nc;
}


   /*=====================================================================*/
const OPENCITY_NET_CODE
Networking::Close()
{
	NetMessage msg;
	OPENCITY_NET_CODE netCode;

// Send disconnect command
	msg.cmd = OC_NET_DNT;
	netCode = SendMessage( msg );

// Close the socket
	SDLNet_TCP_Close(this->pServerSocket);
	this->pServerSocket = NULL;

	return netCode;
}


   /*=====================================================================*/
const OPENCITY_NET_CODE
Networking::Close( ClientIter i )
{
//	assert( i != NULL );

//TODO: send the "disconnect" command to the client

// Remove the socket from the set
	(void)SDLNet_TCP_DelSocket( this->pSocketSet, (*i).socket );

// Close the client socket
	SDLNet_TCP_Close((*i).socket);
	(*i).socket = NULL;

// Erase the client from the vector
	this->vClient.erase(i);
//	std::remove( i, i, (*i) );

	return OC_NET_OK;
}


   /*=====================================================================*/
const OPENCITY_NET_CODE
Networking::Send(
	const void* const data,
	const uint len )
{
	static uint sent;

// IF we are not connected THEN error
	if (this->pServerSocket == NULL)
		return OC_NET_CLIENT_NOTCONNECTED;

// IF there is no data to send THEN error
	if (len == 0)
		return OC_NET_NODATA;

	sent = SDLNet_TCP_Send(this->pServerSocket, const_cast<void*>(data), len);
	if (sent != len) {
		OPENCITY_DEBUG("ERROR: Net send");
		return OC_NET_SENDERROR;
	}

	return OC_NET_OK;
}


   /*=====================================================================*/
const OPENCITY_NET_CODE
Networking::Send(
	const void* const data,
	const uint len,
	const uint cid )
{
	static uint sent;

// WARNING: the code is similar to the Send() method
	assert( cid < this->vClient.size() );

// IF there is no data to send THEN error
	if (len == 0)
		return OC_NET_NODATA;

	sent = SDLNet_TCP_Send(this->vClient[cid].socket, const_cast<void*>(data), len);
	if (sent != len) {
		OPENCITY_DEBUG("ERROR: Net send");
		return OC_NET_SENDERROR;
	}

	return OC_NET_OK;
}


   /*=====================================================================*/
const OPENCITY_NET_CODE
Networking::SendMessage( NetMessage & rMsg )
{
// IF we are not connected THEN
	if (this->pServerSocket == NULL)
		return OC_NET_CLIENT_NOTCONNECTED;

// Send the message to the node
	rMsg.dataLength = OC_NET_DATA_LENGTH;		// hack
	return this->Send( &rMsg, sizeof(NetMessage) );
}


   /*=====================================================================*/
const OPENCITY_NET_CODE
Networking::SendMessage(
	NetMessage & rMsg,
	const uint cid )
{
// Send the message to the node
	rMsg.dataLength = OC_NET_DATA_LENGTH;		// hack
	return this->Send( &rMsg, sizeof(NetMessage), cid );
}


   /*=====================================================================*/
const OPENCITY_NET_CODE
Networking::Receive(
	void* const data,
	const uint maxlen )
{
	static int recv;

// IF we are not connected THEN error
	if (this->pServerSocket == NULL)
		return OC_NET_CLIENT_NOTCONNECTED;

// IF the buffer is not big enough THEN
	if (maxlen == 0)
		return OC_NET_BUFFERROR;

	recv = SDLNet_TCP_Recv( this->pServerSocket, data, maxlen );
	if (recv <= 0) {
		OPENCITY_DEBUG("WARNING: no data received or error");
		return OC_NET_RECVERROR;
	}

	return OC_NET_OK;
}


   /*=====================================================================*/
const OPENCITY_NET_CODE
Networking::ReceiveMessage(
	NetMessage & rMsg )
{
// IF we are not connected THEN
	if (this->pServerSocket == NULL)
		return OC_NET_CLIENT_NOTCONNECTED;

// Wait for the commands sent by the server
	return this->Receive( &rMsg, sizeof(NetMessage) );
}


   /*=====================================================================*/
const OPENCITY_NET_CODE
Networking::Receive(
	void* const data,
	const uint maxlen,
	const uint cid )
{
	static int recv;

// WARNING: the code is similar to the client's Receive() method
	assert( cid < this->vClient.size() );

// IF the buffer is not big enough THEN
	if (maxlen == 0)
		return OC_NET_BUFFERROR;

	recv = SDLNet_TCP_Recv( this->vClient[cid].socket, data, maxlen );
	if (recv <= 0) {
		return OC_NET_NODATA;
	}

	return OC_NET_OK;
}












