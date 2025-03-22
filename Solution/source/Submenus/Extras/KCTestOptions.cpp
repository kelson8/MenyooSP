/*
* Menyoo PC - Grand Theft Auto V single-player trainer mod
* Copyright (C) 2019  MAFINS
* Copyright (c) 2025 kelson8 - this file and other modifications.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*/
#include "KCTestOptions.h"

#include "macros.h"

#include "Menu.h"
#include "Routine.h"

#include "natives2.h"
#include "enums.h"
#include "GTAvehicle.h"
#include "GTAped.h"
#include "GTAprop.h"
#include "GTAplayer.h"
//#include "Model.h"
//#include "World.h"
//#include "Game.h"
//#include "GTAblip.h"
//#include "GTAmath.h"
//#include "Raycast.h"
//#include "DxHookIMG.h"
//#include "ExePath.h"

//#include "VehicleCruise.h"
//#include "VehicleTow.h"
//#include "VehicleFly.h"

#include <math.h>
#include <string>
#include <vector>

// This is how to add basic menus to Menyoo
// First, add the menu to MainMenu.cpp:
// 
// #ifdef CUSTOM_CODE
//AddOption("Test Options", null, nullFunc, SUB::KCTEST);
//#endif

// Also this needs to be added into submenu_switch.cpp (after everything, inside the namespace at the end):
//
// #ifdef CUSTOM_CODE
//	case SUB::KCTEST: sub::SpKCOptions_catind::Sub_SpKcTest(); break;
//#endif

// And this goes in submenu_enum.h:
// This goes before MAX_SUBS.
//
//#ifdef CUSTOM_CODE
//KCTEST,
//#endif

namespace sub {

	namespace SpKCOptions_catind {

		/// <summary>
		/// TODO Test this, should return the player ped.
		/// </summary>
		/// <returns></returns>
		int GetPlayerPed()
		{
			int myPlayer = PLAYER_ID();
			int player = GET_PLAYER_PED(myPlayer);
			
			return player;
		}

		void BlowupPlayer()
		{
			// This works
			int myPlayer = PLAYER_ID();
			int player = GET_PLAYER_PED(myPlayer);

			// I don't think this gets the entity
			if (DOES_ENTITY_EXIST(player))
			{
				Vector3 playerCoords = GET_ENTITY_COORDS(player, true);
				float heading = GET_ENTITY_HEADING(player);
				float posX = playerCoords.x;
				float posY = playerCoords.y;
				float posZ = playerCoords.z;

				int explosionType = 1;
				float damageScale = 1000.0f;

				// First get the players current coords, then run this

				// https://nativedb.dotindustries.dev/gta5/natives/0xE3AD2BDBAEE269AC?search=add_ex
				// Oops, I think I had this set to be invisible..
				ADD_EXPLOSION(posX, posY, posZ, explosionType, damageScale, true, false, 1.0f, false);
				// TODO Test this.
				//ADD_EXPLOSION_WITH_USER_VFX(posX, posY, posZ, explosionType, EXPLOSION::CAR, 1.0f, true, true, 1.0f);
			}
		}

		// Oops, I was using the player ped instead of the ID for the wanted levels, now these work fine.
		void Give6Stars()
		{
			int myPlayer = PLAYER_ID();
			int player = GET_PLAYER_PED(myPlayer);
			if (DOES_ENTITY_EXIST(player))
			{
				SET_PLAYER_WANTED_LEVEL(myPlayer, 6, false);
				SET_PLAYER_WANTED_LEVEL_NOW(myPlayer, false);
			}
		}

		void ClearWantedLevel()
		{
			int myPlayer = PLAYER_ID();
			int player = GET_PLAYER_PED(myPlayer);
			if (DOES_ENTITY_EXIST(player))
			{
				SET_PLAYER_WANTED_LEVEL(myPlayer, 0, false);
				SET_PLAYER_WANTED_LEVEL_NOW(myPlayer, false);
			}
		}
		
		// Oh this is where it gets added to the menu, I forgot about that.
		void Sub_SpKcTest()
		{
			AddTitle("KC Test");
			//AddOption("Bomb", null, nullFunc, );
			AddOption("Blow up player", null, SpKCOptions_catind::BlowupPlayer);

			AddOption("Get 6 stars", null, SpKCOptions_catind::Give6Stars);
			AddOption("Clear cops", null, SpKCOptions_catind::ClearWantedLevel);

		}


	}
}