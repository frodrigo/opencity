/***************************************************************************
                        networking.h  -  description
         $Id: networking.h,v 1.8 2006/03/12 23:14:18 neoneurone Exp $
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

#ifndef _OPENCITY_NETWORKING_H_
	#define _OPENCITY_NETWORKING_H_ 1

	#include "main.h"
	#include "SDL_net.h"
//	#include "netnode.h"

	#include <vector>

	enum OPENCITY_NET_CODE {
		OC_NET_OK = 0,
		OC_NET_ERROR,
		OC_NET_NODATA,
		OC_NET_NOCLIENT,				///< No waiting client
		OC_NET_UNDEFINED,
		OC_NET_SENDERROR,				///< Send error
		OC_NET_RECVERROR,				///< Receive error
		OC_NET_BUFFERROR,				///< Buffer error
		OC_NET_PINGTMOUT,				///< Ping timeout

		OC_NET_CLIENT_ALONE,			///< Stand-alone client the simulator and data are on a server
		OC_NET_CLIENT_SERVER,			///< Client running a server

		OC_NET_CLIENT_CONNECTED,		///< Already connected
		OC_NET_CLIENT_NOTCONNECTED,
		OC_NET_CLIENT_ACCEPTED,
		OC_NET_CLIENT_REJECTED,
		OC_NET_CLIENT_TIMEOUT,

		OC_NET_SERVER_ALONE,			///< Stand-alone server
		OC_NET_SERVER_DISTRIBUTED,		///< Distributed server

		OC_NET_SERVER_READY,			///< The server is up and running
		OC_NET_SERVER_FULL,				///< The server is full
		OC_NET_SERVER_STARTED,			///< The server has been started already
		OC_NET_SERVER_STOPED,			///< The server is not running

		OC_NET_PING,					///< Only server sends this to its clients
		OC_NET_PONG,					///< Only client sends this to the server
		OC_NET_DISCONNECT,
		OC_NET_GETVER,					///< Get version command
		OC_NET_GETPRO
	};

/**
	The following commands are describe in the design document.
	Please read it for more details. Any OpenCity host should
	use those commands instead of hard coding them.
*/
	enum OPENCITY_NET_CMD {
		OC_NET_CNT = 0,
		OC_NET_DNT,
		OC_NET_ACK,
		OC_NET_NCK,
		OC_NET_PIN,
		OC_NET_PON,
		OC_NET_MSG,
		OC_NET_CMD_NUMBER
	};

	const OC_CHAR OPENCITY_NET_CMD_ARRAY [OC_NET_CMD_NUMBER][4] = {
		"CNT", "DNT",
		"ACK", "NCK",
		"PIN", "PON",
		"MSG"
	};


	#define OC_NET_CLIENT_MAX		10
	#define OC_NET_PROTOCOL		"0.0.1"
	#define OC_NET_SERVER_PORT	0xBEEF		// la vache !
	#define OC_NET_PING_TIMEOUT		2000

	#define OC_NET_COMMAND_LENGTH	16
	#define OC_NET_DATA_LENGTH		512


class Netnode;			// forward declaration

/**
	Define the network message's structure for easy use.
	See OpenCity's design documentation for more details about each member.
*/
struct NetMessage {
	OC_CHAR cmd [OC_NET_COMMAND_LENGTH];
	uint cmdLen;
	uint dataLen;
	IPaddress srcIP;
	IPaddress dstIP;
	OC_CHAR reserved[28];
	OC_CHAR data [OC_NET_DATA_LENGTH];
};

/**
	Offer the OpenCity gamers a possibility to share their experience
	with the others through the networks.
*/
class Networking {
public:
	Networking();
	~Networking();

/**
	\return the host (resolved IP) of the specified client
*/
	const string
	GetClientHost( const uint index );

/**
	\return the current number of active connections
*/
	const uint
	GetClientNum();

/**
	\return the maximum number of the connections that the server can handle.
	If the queried machine is running OpenCity as a client application,
	the returned value is undefined.
*/
	const uint
	GetClientMax();

	const OPENCITY_NET_CODE
	GetMachineRole();

/**
	\return the maximum ping time in millisecond.
*/
	const uint
	GetPingTimeout();

/**
	\return the number of servers that the machine knows
*/
	const uint
	GetServerNum();

/**
	\return the host (resolved IP) of the specified server
*/
	const string
	GetServerHost( const uint index );

/**
	\return the string which describes the OpenCity version
	of the remote machine.
	Example: 0.0.2beta
*/
	const string
	GetVersion();

/**
	\return the string which describes the latest OpenCity
	networking protocol that the machine can understand.
	The returned string is in the form: x.y.z like 0.0.1
*/
	const string
	GetProtocol();

/**
	SERVER: Accept the connection request from a machine.
	\param rid The index of the newly accepted client (>=0)
*/
	const OPENCITY_NET_CODE
	Accept( uint & rid );

/**
	SERVER: Reject the connection request from a machine.
*/
	const OPENCITY_NET_CODE
	Reject( const uint id );

/**
	SERVER: Create a new socket and begin to listen for incoming
	connection requests. Return the following enumerations:
		OC_SERVER_READY
		OC_SERVER_STARTED
*/
	const OPENCITY_NET_CODE
	StartServer( const OC_SUINT port = OC_NET_SERVER_PORT );

/**
	SERVER: close the server socket and free all server's ressources.
	\return the following enumerations:
		OC_SERVER_STOPED
*/
	const OPENCITY_NET_CODE
	StopServer();

/**
	SERVER: check the avaibility of data sent by clients and process them.
	This is a non-blocking call
	\return Always return OC_NET_OK
*/
	const OPENCITY_NET_CODE
	ProcessServerData();

/**
	SERVER: try to connect to a another server
	CLIENT: try to connect to a server
	\return the following enumerations:
*/
	const OPENCITY_NET_CODE
	Open(
		const string serverHost,
		const OC_SUINT port = OC_NET_SERVER_PORT );

/**
	SERVER: close the connection to a super server
	CLIENT: close the connection to a server
	\return the following enumerations:
*/
	const OPENCITY_NET_CODE
	Close();

/**
	SERVER: close the connection to a client
	CLIENT: this command is not available
	\return the following enumerations:
*/
	const OPENCITY_NET_CODE
	Close( const uint cid );

/**
	SERVER: send the data to a super server
	CLIENT: send the data to the server
	\return the following enumerations:
*/
	const OPENCITY_NET_CODE
	Send(
		const void* const data,
		const uint len );

/**
	SERVER: send the data to a client
	CLIENT: this method is _not_ available to a client machine
	\return the following enumerations:
*/
	const OPENCITY_NET_CODE
	Send(
		const void* const data,
		const uint len,
		const uint cid );

/**
	SERVER: send the message to a super server
	CLIENT: send the message to a server
*/
	const OPENCITY_NET_CODE
	SendMessage(
		const OPENCITY_NET_CMD & rcCmd,
		NetMessage & rMsg
		);

/**
	SERVER: send the message to a super server
	CLIENT: this method is _not_ available to a client host
*/
	const OPENCITY_NET_CODE
	SendMessage(
		const OPENCITY_NET_CMD & rcCmd,
		NetMessage & rMsg,
		const uint cid
		);

/**
	SERVER: read the data sent by a super server
	CLIENT: read the data sent by the server
	\return the following enumerations:
*/
	const OPENCITY_NET_CODE
	Receive(
		void* const data,
		const uint maxlen );

/**
	SERVER: read the data sent by a super server specified by its index
	CLIENT: this method is _not_ available to a client
	\return the following enumerations:
*/
	const OPENCITY_NET_CODE
	Receive(
		const uint cid,
		void* const data,
		const uint maxlen );


private:
	OPENCITY_NET_CODE			networkingCode;
	bool						boolNetworkInitialized;

	std::vector<Netnode>		vClient;

	SDLNet_SocketSet			pSocketSet;		///< The server puts all its client sockets into this set
	TCPsocket					pServerSocket;	///< The server listens on this socket
												///< The client use this socket to communicate with the server

};
#endif









