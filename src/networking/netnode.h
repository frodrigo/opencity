/***************************************************************************
						netnode.h  -  description
							-------------------
	begin                : dec 23th, 2004
	copyright            : (C) 2004-2006 by Duong Khang NGUYEN
	email                : neoneurone @ gmail com
	
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

#ifndef _OPENCITY_NETNODE_H_
#define _OPENCITY_NETNODE_H_ 1

#include "main.h"

#include "SDL_net.h"


//========================================================================
/** This is a helper class of the "Networking" class. Each host is
considered as a "Netnode" by the networking layer.
*/
class Netnode {
public:
	Netnode() {}
	Netnode( IPaddress ipAdr, TCPsocket sock ):ip(ipAdr), socket(sock) {}
	bool operator==( const Netnode& rc ) {
		return ((this->ip.host == rc.ip.host) && (this->ip.port == rc.ip.port)
			 && (this->socket == rc.socket));
	}

	~Netnode() {}

	IPaddress ip;
	TCPsocket socket;
private:
};

#endif





























