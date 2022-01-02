// C_NextBot.h
// Next generation bot system
// Author: Michael Booth, April 2005
//========= Copyright (c) Valve Corporation, All rights reserved. ============//
#ifndef _C_NEXT_BOT_H_
#define _C_NEXT_BOT_H_

#include "../c_ai_basenpc.h"

//----------------------------------------------------------------------------------------------------------------
/**
* The interface holding IBody information
*/
class IBodyClient
{
public:
	enum ActivityType
	{
		MOTION_CONTROLLED_XY = 0x0001,	// XY position and orientation of the bot is driven by the animation.
		MOTION_CONTROLLED_Z = 0x0002,	// Z position of the bot is driven by the animation.
		ACTIVITY_UNINTERRUPTIBLE = 0x0004,	// activity can't be changed until animation finishes
		ACTIVITY_TRANSITORY = 0x0008,	// a short animation that takes over from the underlying animation momentarily, resuming it upon completion
		ENTINDEX_PLAYBACK_RATE = 0x0010,	// played back at different rates based on entindex
	};
};

//--------------------------------------------------------------------------------------------------------
/**
 * The client-side implementation of the NextBot
**/
//--------------------------------------------------------------------------------------------------------
class C_NextBotCombatCharacter : public C_BaseCombatCharacter
{
public:

};

#endif // _C_NEXT_BOT_H_