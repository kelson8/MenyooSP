#include "KCTestScripts.h"

#include "macros.h"

#include "Routine.h"

//#include "natives2.h"
#include "enums.h"
//#include "GTAvehicle.h"
//#include "GTAped.h"
//#include "GTAprop.h"
//#include "GTAplayer.h"

#include "GTAmemory.h"

// Converted to namespace
namespace sub 
{
	namespace KcTestScripts 
	{
		/// <summary>
		/// TODO Set this one up.
		/// </summary>
		void BurstAllTires()
		{
			// This breaks the build
			//for (auto veh : GetAllVehs())
			//{
			//	SET_VEHICLE_TYRE_BURST(veh, 0, true, 1000.0f);
			//}
		}

		/// <summary>
		/// Check if the player is currently in a vehicle
		/// </summary>
		/// <returns>If the player is in a vehicle</returns>
		bool IsPlayerInVehicle()
		{
			int myPlayer = PLAYER_ID();
			int player = GET_PLAYER_PED(myPlayer);

			if (IS_PED_IN_ANY_VEHICLE(player, true))
			{
				return true;
			}

			return false;
		}

		/// <summary>
		/// Get the players current vehicle
		/// </summary>
		/// <returns>The current vehicle</returns>
		Vehicle GetPlayerVehicle()
		{
			int myPlayer = PLAYER_ID();
			int player = GET_PLAYER_PED(myPlayer);
			if (IsPlayerInVehicle())
			{
				Vehicle playerVeh = GET_VEHICLE_PED_IS_IN(player, false);
				return playerVeh;
			}

			// This should return nothing if the player doesn't have a vehicle.
			return 0;
		}

		/// <summary>
		/// Get the current waypoint coordinates if it exists.
		/// </summary>
		/// <returns>A Vector3 of the coords, such as this: Vector3(22.5, 15.2, 30.2)</returns>
		Vector3 GetWaypointCoords()
		{
			Vector3 coords;
			if (IS_WAYPOINT_ACTIVE())
			{
				coords = GET_BLIP_COORDS(GET_FIRST_BLIP_INFO_ID(8));
				return coords;
			}

			// Return all 0's if no waypoint found, TODO Setup check for this value.
			return Vector3(0,0,0);
		}

#pragma region ScaleformTests

		namespace Scaleforms 
		{

			// These are converted from LUA using FiveM and my scripts:
			// https://github.com/kelson8/fivem-scripts/blob/main/kc_test/client/misc/scaleform_test.lua

			int scaleformHandle = 0;

			/// <summary>
			/// Setup the loading bar scaleform
			/// </summary>
			/// <param name="scaleformHandle">The scaleform handle, this gets set when the scaleform is shown such 
			/// as in ShowLoadingBarScaleform()</param>
			void LoadingBarScaleform(int scaleformHandle)
			{
				BEGIN_SCALEFORM_MOVIE_METHOD(scaleformHandle, "SET_PROGRESS_BAR"); // Function to call from the AS file.
				//PushScaleformMovieMethodParameterInt
				SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(50); // Set progress of loading bar
				END_SCALEFORM_MOVIE_METHOD();
			}


			/// <summary>
			/// Show the loading bar scaleform with a progress bar.
			/// </summary>
			void ShowLoadingBarScaleform()
			{
				const char* loadingScreenNewGameScaleform = "LOADINGSCREEN_NEWGAME";
				scaleformHandle = REQUEST_SCALEFORM_MOVIE(loadingScreenNewGameScaleform);
				LoadingBarScaleform(scaleformHandle);
			}

			// TODO Setup checkbox toggle in Menyoo somewhere under my test menu.
			bool drawScaleform = false;
			void DrawScaleform()
			{
				// I got this drawing to the screen, it can be turned on with the enable scaleform test button, and 
				// turned off with the disable scaleform test button.
				if (drawScaleform)
				{
					// Check if the scaleform has loaded, if so draw it to the screen.
					if (HAS_SCALEFORM_MOVIE_LOADED(scaleformHandle)) {
						DRAW_SCALEFORM_MOVIE_FULLSCREEN(scaleformHandle, 255, 255, 255, 255, 1);
					}

				}
			}

			/// <summary>
			/// This should run cleanup for the scaleforms like original scripts would.
			/// </summary>
			/// <param name="scaleformHandle"></param>
			void CleanupScaleform(int *scaleformHandle)
			{
				if (HAS_SCALEFORM_MOVIE_LOADED(*scaleformHandle))
				{
					SET_SCALEFORM_MOVIE_AS_NO_LONGER_NEEDED(scaleformHandle);
				}
			}

			// Scaleform toggle on/off

			/// <summary>
			/// Show the load bar scaleform, and set drawScaleform to true
			/// </summary>
			void EnableScaleform()
			{
				// This is required to load the scaleform.
				ShowLoadingBarScaleform();
				drawScaleform = true;
			}

			/// <summary>
			/// Disable the load bar scaleform, cleanup, and set drawScaleform to false.
			/// </summary>
			void DisableScaleform()
			{
				CleanupScaleform(&scaleformHandle);
				drawScaleform = false;
			}
		}


#pragma endregion
	}


}


#define NEW_TEST


#ifdef NEW_TEST
// From Chaos Mod, EntityIterator.h
// 
// Common functions for VehiclePool and GenericPool
template <typename T> class PoolUtils
{
public:
	inline auto ToArray()
	{
		std::vector<Entity> arr;
		for (auto entity : *static_cast<T*>(this))
			arr.push_back(entity);

		return arr;
	}

	auto begin()
	{
		return ++PoolIterator<T>(static_cast<T*>(this), -1);
	}

	auto end()
	{
		return ++PoolIterator<T>(static_cast<T*>(this), static_cast<T*>(this)->m_Size);
	}
};


class VehiclePool : public PoolUtils<VehiclePool>
{
public:
	UINT64* m_PoolAddress;
	UINT32 m_Size;
	char _Padding2[36];
	UINT32* m_BitArray;
	char _Padding3[40];
	UINT32 m_ItemCount;

	inline bool IsValid(UINT32 i)
	{
		return (m_BitArray[i >> 5] >> (i & 0x1F)) & 1;
	}

	inline UINT64 GetAddress(UINT32 i)
	{
		return m_PoolAddress[i];
	}
};

inline auto& GetAllVehs()
{
	static VehiclePool* vehPool = []
		{
			uintptr_t rawHandle = GTAmemory::FindPattern("48 8B 05 ?? ?? ?? ?? F3 0F 59 F6 48 8B 08", "xxx????xxxxx");
			if (rawHandle == 0)
			{
				return (VehiclePool*)nullptr; // Return null if the pattern isn't found
			}
			VehiclePool** vehPoolPtrPtr = (VehiclePool**)(rawHandle + 2); // Add 2 bytes to the found address.
			if (vehPoolPtrPtr == nullptr) {
				return (VehiclePool*)nullptr;
			}
			VehiclePool* vehPoolPtr = *vehPoolPtrPtr; // Dereference to get the VehiclePool pointer.
			return vehPoolPtr;
		}();
	return *vehPool;
}

//inline auto& GetAllVehs()
//{
//	static VehiclePool* vehPool = []
//		{
//			auto handle = GTAmemory::FindPattern("48 8B 05 ?? ?? ?? ?? F3 0F 59 F6 48 8B 08", "xxx????xxxxx");
//			return *handle.At(2).Into().Value<VehiclePool**>();
//		}();
//
//	return *vehPool;
//}
#endif