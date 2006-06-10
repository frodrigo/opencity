/***************************************************************************
                           main.cpp  -  description
         $Id: main.cpp,v 1.3 2005/12/11 17:38:09 haypo Exp $
                             -------------------
    begin                : nov 29th 2005
    copyright            : (C) 2005 by Duong-Khang NGUYEN
    email                : neoneurone @ users sourceforge net
    author               : Victor STINNER
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/


#include "kernel.h"
#include "environment.h"
#include "agentpolice.h"
#include "agentdemonstrator.h"

#include <iostream>

/** The purpose of this program is for testing the Multi-Agent System (MAS)
*/
int main()
{
	Kernel kernel;
	Environment env(100,100);
	(void)new AgentPolice(kernel, env, 1, 2);
    (void)new AgentPolice(kernel, env, 3, 4);
	AgentDemonstrator *demonstrator = new AgentDemonstrator(kernel, env, 4, 2);
	demonstrator->live();
	return 0;
}

































