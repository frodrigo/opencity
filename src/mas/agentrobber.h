/***************************************************************************
						agentrobber.h  -  description
							-------------------
	begin                : nov 29th 2005
	copyright            : (C) 2005-2006 by Duong Khang NGUYEN
	email                : neoneurone @ gmail com
	author               : Victor STINNER

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

#ifndef _OPENCITY_AGENTROBBER_H_
#define _OPENCITY_AGENTROBBER_H_ 1

#include "agent.h"


   /*=====================================================================*/
/** Specialized robber agent
	@author Victor STINNER
*/
class AgentRobber : public Agent
{
public:
	AgentRobber(Kernel& kernel, Environment& env, int x, int y);
	virtual ~AgentRobber();

	virtual void processMessage();
	virtual void born();
	virtual void live();
	virtual void die();
	virtual void output(std::ostream &os) const;
};

#endif

































