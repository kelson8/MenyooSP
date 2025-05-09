#pragma once
#include "natives2.h"

#define NEW_TEST


#ifdef NEW_TEST
namespace sub 
{
	namespace KcTestScripts 
	{
		void BurstAllTires();

		bool IsPlayerInVehicle();

		Vehicle GetPlayerVehicle();
		Vector3 GetWaypointCoords();

		namespace Scaleforms 
		{
			//bool drawScaleform;

			void LoadingBarScaleform(int scaleformHandle);
			void ShowLoadingBarScaleform();
			void DrawScaleform();

			void EnableScaleform();
			void DisableScaleform();
		}

	}
}
#else
class KcTestScripts
{
public:
	bool IsPlayerInVehicle();
	Vehicle GetPlayerVehicle();
	Vector3 GetWaypointCoords();

	// TODO Test this
	void BurstAllTires();


};
#endif

