#pragma unmanaged

#include <BWAPI.h>

#pragma managed

#include "AIModule.h"

#pragma unmanaged

extern "C" __declspec(dllexport)
BWAPI::AIModule* newAIModule(BWAPI::Game* game)
{
	//Hack to properly locate Broodwar.dll assembly
	Preload();
	LoadGameInstance(game);
	return new AIModule();
}
