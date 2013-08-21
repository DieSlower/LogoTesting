/*
* //////////////////////////////////////C++//////////////////////////////////////
* 
* Virtual Reality Simulation Lab
* 
* This file is part of Virtual Reality Simulation Lab software. You can redistribute 
* it and/or modify it only with the authors permission. All unlicensed use and modification
* of this file are prohibited. 
* 
* This does not restrict the user from creating custom modules and additional files to 
* interface with the VR-Sim Lab.
* 
* This software was developed by Maxim Serebrennik for Various projects he worked on.
* It is sold and distributed only with his permission. 
* 
* For more information e-mail him at MaximSter@gmail.com
* 
* @author Maxim Serebrennik 
*////////////////////////////////////////////////////////////////////////////////

#ifndef COREDLL_GAME_EVENTS_H
#define COREDLL_GAME_EVENTS_H

#include <dtUtil/RefString.h>

namespace vrsCore
{
	// Constant identifier for our game event handler method.

	const dtUtil::RefString GAME_EVENT_IGNITION("Ignition");
	const dtUtil::RefString GAME_EVENT_AUTOLEVEL("AutoLevel");

	const dtUtil::RefString GAME_EVENT_MOVEFORWARD("MoveForward");
	const dtUtil::RefString GAME_EVENT_MOVEBACK("MoveBack");
	const dtUtil::RefString GAME_EVENT_YAWLEFT("YawLeft");
	const dtUtil::RefString GAME_EVENT_YAWRIGHT("YawRight");
	const dtUtil::RefString GAME_EVENT_PITCHUP("PitchUp");
	const dtUtil::RefString GAME_EVENT_PITCHDOWN("PitchDown");
	const dtUtil::RefString GAME_EVENT_ROLLLEFT("RollLeft");
	const dtUtil::RefString GAME_EVENT_ROLLRIGHT("RollRight");
	const dtUtil::RefString GAME_EVENT_STRAFERIGHT("StrafeRight");
	const dtUtil::RefString GAME_EVENT_STRAFELEFT("StrafeLeft");
	const dtUtil::RefString GAME_EVENT_STRAFEUP("StrafeUp");
	const dtUtil::RefString GAME_EVENT_STRAFEDOWN("StrafeDown");
	
	const dtUtil::RefString GAME_EVENT_RESETSTUFF("ResetStuff");
	const dtUtil::RefString GAME_EVENT_SHOWSTATISTICS("ShowStat");

	const dtUtil::RefString GAME_EVENT_F11("F11_Event");
	const dtUtil::RefString GAME_EVENT_F12("F12_Event");
}

#endif //COREDLL_GAME_EVENTS_H