#pragma unmanaged

#include <BWAPI.h>

#pragma managed

#include "AIModule.h"
#include "TournamentModule.h"

#pragma unmanaged

bool preload = false;

extern "C" __declspec(dllexport)
BWAPI::AIModule* newAIModule(BWAPI::Game* game)
{
	//Hack to properly locate Broodwar.dll assembly
	if(!preload)
	{
		Preload();
		preload = true;
	}
	LoadGameInstance(game);
	return new AIModule();
}

extern "C" __declspec(dllexport)
BWAPI::TournamentModule* newTournamentModule()
{
	if(!preload)
	{
		Preload();
		preload = true;
	}
	return new TournamentModule();
}
