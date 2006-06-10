/***************************************************************************
                     networking.cpp  -  description
         $Id: networking.cpp,v 1.7 2006/03/12 23:14:18 neoneurone Exp $
                             -------------------
    begin                : jeu dec 23 2004
    copyright            : (C) 2004 by Duong-Khang NGUYEN
    email                : neoneurone @ users sourceforge net
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
	if (this->vClient.size() == OC_NET_CLIENT_MAX)
		return OC_NET_SERVER_FULL;

// Create the new socket to handle the client
	Netnode nodeClient;
	nodeClient.socket = SDLNet_TCP_Accept(this->pServerSocket);
	if (nodeClient.socket != NULL) {
		nodeClient.ip = *(SDLNet_TCP_GetPeerAddress(nodeClient.socket));
		vClient.push_back(nodeClient);
		rid = vClient.size()-1;
	// Put the client socket into the server's socket set for future use
		if (SDLNet_TCP_AddSocket( this->pSocketSet, nodeClient.socket ) == -1) {
			assert( 0 );		// FIXME: disconnect the client
			return OC_NET_SERVER_FULL;
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
	uint nbClient;
	OPENCITY_NET_CODE netCode;
	NetMessage msg;

// Accept an incoming connection
	netCode = this->Accept( id );
	if (netCode == OC_NET_OK) {
		OPENCITY_DEBUG( "New client connection accepted" );
		cout << "Number of client: " << this->GetClientNum() << endl;
	}

// Check socket set for activity
// Otherwise return
	if (SDLNet_CheckSockets( this->pSocketSet, 50 ) < 1)
		return OC_NET_OK;

// Process waiting messages sent by client
	nbClient = this->vClient.size();
	while (nbClient > 0) {
		--nbClient;
		OPENCITY_DEBUG( "Begin reading socket" );
		if (SDLNet_SocketReady(this->vClient[nbClient].socket) != 0) {
			if (this->Receive( nbClient, &msg, sizeof(msg)) != OC_NET_OK)
				continue;

		// Disconnect request ?
			if (strcmp( msg.cmd, "DNT" ) == 0) {
			// FIXME: send ACK
				(void)SDLNet_TCP_DelSocket( this->pSocketSet, this->vClient[nbClient].socket );
				(void)this->Close( nbClient );
				OPENCITY_DEBUG( "One client disconnected" );
				cout << "Client id: " << nbClient << endl;
			}
		}
		OPENCITY_DEBUG( "End reading socket" );
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

	IPaddress ip;

// IF we can not resolve the host THEN error
	if (SDLNet_ResolveHost(&ip, serverHost.c_str(), port) == -1) {
		cerr << "ERROR: I couldn't resolve the server name: "
			 << SDLNet_GetError() << endl;
		return OC_NET_ERROR;
	}

	this->pServerSocket = SDLNet_TCP_Open(&ip);
	if (pServerSocket == NULL) {
		cerr << "ERROR: I couldn't connect to the server: "
			 << SDLNet_GetError() << endl;
		return OC_NET_ERROR;
	}

	return OC_NET_OK;
}


   /*=====================================================================*/
const OPENCITY_NET_CODE
Networking::Close()
{
	NetMessage msg;
	OPENCITY_NET_CODE netCode;

// Send disconnect command
	netCode = SendMessage( OC_NET_DNT, msg );

// Close the socket
	SDLNet_TCP_Close(this->pServerSocket);
	this->pServerSocket = NULL;

	return netCode;
}


   /*=====================================================================*/
const OPENCITY_NET_CODE
Networking::Close( const uint cid )
{
	assert( cid < this->vClient.size() );

//TODO: send the "disconnect" command to the client

// Close the client socket
	SDLNet_TCP_Close(this->vClient[cid].socket);
	this->vClient[cid].socket = NULL;

// Remove the client from the vector and resize the vector
// at the same time.
	this->vClient.erase(
		remove(vClient.begin(), vClient.end(), vClient[cid]),
		vClient.end());

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
Networking::SendMessage(
	const OPENCITY_NET_CMD & rcCmd,
	NetMessage & rMsg )
{
// IF we are not connected THEN
	if (this->pServerSocket == NULL)
		return OC_NET_CLIENT_NOTCONNECTED;

// Send the requested command to the server
	strcpy( rMsg.cmd, OPENCITY_NET_CMD_ARRAY[rcCmd] );
	rMsg.cmdLen = 3;
	return this->Send( &rMsg, sizeof(rMsg) );
}


   /*=====================================================================*/
const OPENCITY_NET_CODE
Networking::SendMessage(
	const OPENCITY_NET_CMD & rcCmd,
	NetMessage & rMsg,
	const uint cid )
{
	assert( 0 );
	return OC_NET_OK;
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
	}

	return OC_NET_OK;
}


   /*=====================================================================*/
const OPENCITY_NET_CODE
Networking::Receive(
	const uint cid,
	void* const data,
	const uint maxlen )
{
	static int recv;

// WARNING: the code is similar to the client's Receive() method
	assert( cid < this->vClient.size() );

// IF the buffer is not big enough THEN
	if (maxlen == 0)
		return OC_NET_BUFFERROR;

	recv = SDLNet_TCP_Recv( this->vClient[cid].socket, data, maxlen );
	if (recv <= 0) {
//		OPENCITY_DEBUG("WARNING: no data received or error");
		return OC_NET_NODATA;
	}

	return OC_NET_OK;
}












