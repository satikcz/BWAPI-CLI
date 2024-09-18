#include "Chokepoint.h"
#include "Region.h"
#include "..\Util.h"

using namespace BroodWar::Api;

namespace BroodWar
{
	namespace Terrain
	{
		Chokepoint^ ConvertChokepoint(BWTA::Chokepoint* point)
		{
			if(point == NULL)
				return nullptr;
			return gcnew Chokepoint(point);
		}

		BWTA::Chokepoint* ConvertChokepoint(Chokepoint^ point)
		{
			if(point == nullptr)
				return NULL;
			return point->instance;
		}

		Chokepoint::Chokepoint(BWTA::Chokepoint* point)
		{
			instance = point;
		}

		Tuple<Region^, Region^>^ Chokepoint::Regions::get()
		{
			return ToTuple<BWTA::Region*, BWTA::Region*, Region^, Region^>(
				instance->getRegions(),
				&ConvertRegion,
				&ConvertRegion);
		}

		Tuple<Api::Position^, Api::Position^>^ Chokepoint::Sides::get()
		{
			return ToTuple<BWAPI::Position, BWAPI::Position, Api::Position^, Api::Position^>(
				instance->getSides(),
				&Api::ConvertPosition,
				&Api::ConvertPosition);
		}

		Api::Position^ Chokepoint::Center::get()
		{
			return ConvertPosition(instance->getCenter());
		}

		double Chokepoint::Width::get()
		{
			return instance->getWidth();
		}
	}
}
