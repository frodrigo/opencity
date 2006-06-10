/***************************************************************************
                          audiomanager.cpp  -  description
      $Id: audiomanager.cpp,v 1.14 2006/03/12 23:14:18 neoneurone Exp $
                             -------------------
    begin                : ven déc 26 2003
    copyright            : (C) 2003-2005 by Duong-Khang NGUYEN
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

#include "audiomanager.h"


   /*====================================================================*/
AudioManager::AudioManager():
boolAudioDeviceInitialized( false ),
boolMusicEnabled( false ),
boolSoundEnabled( false ),
uiNumberSound( 0 ),
uiNumberMusic( 0 ),
uiCurrentMusic( 0 )
{
	OPENCITY_DEBUG("ctor");
}


   /*====================================================================*/
AudioManager::~AudioManager()
{
	OPENCITY_DEBUG("dtor");
}


   /*====================================================================*/
OPENCITY_ERR_CODE
AudioManager::OpenAudio(void)
{
	return OC_ERR_FREE;
}


   /*====================================================================*/
OPENCITY_ERR_CODE
AudioManager::CloseAudio(void)
{
	return OC_ERR_FREE;
}


   /*====================================================================*/
OPENCITY_ERR_CODE
AudioManager::LoadMusicList
(
	const string & csrFilename,
	const string& csrPrefix
)
{
        return OC_ERR_FREE;
}


   /*====================================================================*/
const uint &
AudioManager::GetNumberMusic(void) const
{
	return this->uiNumberMusic;
}


   /*====================================================================*/
bool
AudioManager::PlayingMusic(void) const
{
	return false;
}


   /*====================================================================*/
OPENCITY_ERR_CODE
AudioManager::PlayMusic
(
	const uint & rcuiMusicIndex,
	const int & rciLoops )
{
	return OC_ERR_FREE;
}


   /*====================================================================*/
OPENCITY_ERR_CODE
AudioManager::PlayNextMusic
(
	const int & rciLoops
)
{
	return OC_ERR_FREE;
}


   /*====================================================================*/
OPENCITY_ERR_CODE
AudioManager::PlayPreviousMusic
(
	const int & rciLoops
)
{
	return OC_ERR_FREE;
}


   /*====================================================================*/
void
AudioManager::StopMusic(void) const
{
	return;
}


   /*====================================================================*/
void
AudioManager::ToggleRandomMusic(void)
{
	return;
}


   /*====================================================================*/
void
AudioManager::ToggleMusic(void)
{
	return;
}


   /*====================================================================*/
void
AudioManager::VolumeMusic
(
	const int & rciVol
) const
{
	return;
}








   /*====================================================================*/
OPENCITY_ERR_CODE
AudioManager::LoadSoundList
(
	const string & csrFilename,
	const string& csrPrefix
)
{
	return OC_ERR_FREE;
}


   /*====================================================================*/
const uint &
AudioManager::GetNumberSound(void) const
{
	return this->uiNumberSound;
}


   /*====================================================================*/
OPENCITY_ERR_CODE
AudioManager::PlaySound
(
	const uint & rcuiSoundIndex,
	const AUDIO_CHANNEL & enumChannel
)
{
	return OC_ERR_FREE;
}


   /*====================================================================*/
void
AudioManager::ToggleSound(void)
{
	return;
}


   /*====================================================================*/
void
AudioManager::VolumeSound
(
	const int & rciVol
) const
{
	return;
}



























