/***************************************************************************
						agentdemonstrator.h  -  description
							-------------------
	begin                : nov 29th 2005
	copyright            : (C) 2005-2006 by Duong-Khang NGUYEN
	email                : neoneurone @ users sourceforge net
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

#ifndef _OPENCITY_AGENTDEMONSTRATOR_H_
#define _OPENCITY_AGENTDEMONSTRATOR_H_ 1

#include "agent.h"


   /*=====================================================================*/
/** Specialized demonstrator agent
	@author Victor STINNER
*/
class AgentDemonstrator : public Agent
{
public:
	AgentDemonstrator(Kernel& kernel, Environment& env, int x, int y);
	virtual ~AgentDemonstrator();

	virtual void processMessage();
	virtual void born();
	virtual void live();
	virtual void die();
	virtual void output(std::ostream &os) const;

private:
	unsigned int m_ttl;
};

#endif

































