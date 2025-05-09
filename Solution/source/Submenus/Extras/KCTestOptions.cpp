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
#include "KCTestScripts.h"

#include "macros.h"

#include "Menu.h"
#include "Routine.h"

#include "natives2.h"
#include "enums.h"
//#include "GTAvehicle.h"
//#include "GTAped.h"
//#include "GTAprop.h"
//#include "GTAplayer.h"
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

#define ENTITY_POOL_MAX 40
static std::list<Entity> m_Entities;

// Copied from Chaos mod, TODO Move this
// Use like this: static const Hash ballHash = "prop_juicestand"_hash;
//constexpr Hash operator""_hash(const char* str, size_t n)
Hash operator""_hash(const char* str, size_t n)
{
	return GET_HASH_KEY(str);
}

// TODO Move these into another file

// Load the model and wait on it.
inline void LoadModel(Hash model)
{
	if (IS_MODEL_VALID(model))
	{
		REQUEST_MODEL(model);
		while (!HAS_MODEL_LOADED(model))
			WAIT(0);
	}
}


static void HandleEntity(Entity entity)
{
	m_Entities.push_back(entity);

	// Clean up entities which don't exist anymore first
	for (auto it = m_Entities.begin(); it != m_Entities.end();)
		if (!DOES_ENTITY_EXIST(*it))
			it = m_Entities.erase(it);
		else
			it++;

	// Delete front entity if size above limit
	if (m_Entities.size() > ENTITY_POOL_MAX)
	{
		auto frontEntity = m_Entities.front();

		if (DOES_ENTITY_EXIST(frontEntity))
			SET_ENTITY_AS_NO_LONGER_NEEDED(&frontEntity);

		m_Entities.pop_front();
	}
}

// This works for spawning props, tested with spawning the orange ball
Object CreatePoolProp(Hash modelHash, float x, float y, float z, bool dynamic)
{
	LoadModel(modelHash);
	auto prop = CREATE_OBJECT(modelHash, x, y, z, true, false, dynamic);
	HandleEntity(prop);
	SET_MODEL_AS_NO_LONGER_NEEDED(modelHash);
	return prop;
}

// End from Chaos mod

/// <summary>
/// This should work for getting the players coords
/// </summary>
/// <returns></returns>
Vector3 GetPlayerCoords()
{
	int myPlayer = PLAYER_ID();
	int player = GET_PLAYER_PED(myPlayer);

	Vector3 playerCoords = GET_ENTITY_COORDS(myPlayer, true);

	return playerCoords;
}

/// <summary>
/// TODO Test this, should clear all objects in an area.
/// </summary>
void ClearAreaOfObjects()
{
	int myPlayer = PLAYER_ID();
	int player = GET_PLAYER_PED(myPlayer);

	// Moved to above function
	Vector3 playerCoords = GET_ENTITY_COORDS(myPlayer, true);
	float playerX = playerCoords.x;
	float playerY = playerCoords.y;
	float playerZ = playerCoords.z;

	//float playerX = getPlayerCoords().x;
	//float playerY = getPlayerCoords().y;
	//float playerZ = getPlayerCoords().z;
	float radius = 15.0f;
	int flag = 0;


	// Does this not clear the orange ball?
	CLEAR_AREA_OF_OBJECTS(playerX, playerY, playerZ, radius, flag);
}



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
		void Give5Stars()
		{
			int myPlayer = PLAYER_ID();
			int player = GET_PLAYER_PED(myPlayer);
			if (DOES_ENTITY_EXIST(player))
			{
				SET_PLAYER_WANTED_LEVEL(myPlayer, 5, false);
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

		// Enabled by default
		bool specialAbilityBarToggled = true;
		/// <summary>
		/// TODO Test this, should toggle the special ability bar on and off.
		/// </summary>
		void ToggleSpecialAbilityBar()
		{
			int myPlayer = PLAYER_ID();
			int player = GET_PLAYER_PED(myPlayer);

			if (DOES_ENTITY_EXIST(player))
			{
				specialAbilityBarToggled = !specialAbilityBarToggled;
				SET_ABILITY_BAR_VISIBILITY(specialAbilityBarToggled);
				SET_ALLOW_ABILITY_BAR(specialAbilityBarToggled);
			}
		}

		void Test()
		{
			int myPlayer = PLAYER_ID();
			int player = GET_PLAYER_PED(myPlayer);
			//KcTestScripts kcTestScripts;


			// TODO Test this later, should remove the player waypoints.
			//DELETE_WAYPOINTS_FROM_THIS_PLAYER();

			//if (DOES_ENTITY_EXIST(player) && kcTestScripts.IsPlayerInVehicle())
			if (DOES_ENTITY_EXIST(player) && sub::KcTestScripts::IsPlayerInVehicle())
			{
				//Vehicle playerVeh = kcTestScripts.GetPlayerVehicle();
				Vehicle playerVeh = sub::KcTestScripts::GetPlayerVehicle();


				
				//int allVehs = GET_ALL_VEHICLES();
				//for(const& auto : )
			}

		}


		// Fix for a bug
#undef min
#undef max

		/// <summary>
		/// Copied from Chaos mod, MiscSpawnOrangeBall.cpp.
		/// TODO Test this, I added in some other features from the chaos mod for this.
		/// </summary>
		void SpawnOrangeBall()
		{
			static const Hash ballHash = "prop_juicestand"_hash;
			static const Hash weaponHash = "weapon_specialcarbine"_hash;

			static const float minDistance = 2.f;
			static const float maxDistance = 7.f;
			static const float maxSpeedCheck = 40.f;

			Ped player = PLAYER_PED_ID();
			Vector3 pos = GET_ENTITY_COORDS(player, false);

			// Make distance to player dependent on players speed
			float playerSpeed = std::min(std::max(0.f, GET_ENTITY_SPEED(player)), maxSpeedCheck);
			float fixedDistance = ((playerSpeed / maxSpeedCheck) * (maxDistance - minDistance)) + minDistance;

			Vector3 spawnPos = GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(player, 0, fixedDistance, 0);
			Object ball = CreatePoolProp(ballHash, spawnPos.x, spawnPos.y, spawnPos.z - 0.2, true);
			
			// Randomize weight
			//float weight = g_Random.GetRandomFloat(1, 100);
			float weight = 20.0f;
			SET_OBJECT_PHYSICS_PARAMS(ball, weight, 1.f, 1.f, 0.f, 0.f, .5f, 0.f, 0.f, 0.f, 0.f, 0.f);
			
			// Ball needs to be shot at to be dynamic, otherwise it will be frozen
			Vector3_t min, max;
			GET_MODEL_DIMENSIONS(ballHash, &min, &max);
			SHOOT_SINGLE_BULLET_BETWEEN_COORDS(spawnPos.x, spawnPos.y, spawnPos.z + max.z - min.z, spawnPos.x, spawnPos.y,
				spawnPos.z, 0, true, weaponHash, 0, false, true, 0.01f);

		}
		//
		
		// Oh this is where it gets added to the menu, I forgot about that.
		void Sub_SpKcTest()
		{
			AddTitle("KC Test");
			//AddOption("Bomb", null, nullFunc, );
			AddOption("Blow up player", null, SpKCOptions_catind::BlowupPlayer);

			AddOption("Get 5 stars", null, SpKCOptions_catind::Give5Stars);
			AddOption("Clear cops", null, SpKCOptions_catind::ClearWantedLevel);


			AddOption("Spawn orange ball", null, SpKCOptions_catind::SpawnOrangeBall);

			AddBreak("Clear area");
			AddOption("Clear objects", null, ClearAreaOfObjects);

#ifdef NEW_TEST
			AddBreak("Vehicles");
			AddOption("Burst all car tires", null, sub::KcTestScripts::BurstAllTires);
			
			AddBreak("Scaleforms");
			AddOption("Enable scaleform test", null, sub::KcTestScripts::Scaleforms::EnableScaleform);
			AddOption("Disable scaleform test", null, sub::KcTestScripts::Scaleforms::DisableScaleform);

			// These seem to work well
			AddBreak("Phone");
			AddOption("Enable phone", null, sub::KcTestScripts::PhoneTests::EnablePhone);
			AddOption("Disable phone", null, sub::KcTestScripts::PhoneTests::DisablePhone);


#endif
		}


	}
}