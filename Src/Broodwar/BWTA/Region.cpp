#pragma comment(lib, "MSVCRT")

#include "Region.h"
#include "BaseLocation.h"
#include "Chokepoint.h"
#include "..\Util.h"

using namespace BroodWar::Api;

namespace BroodWar
{
	namespace Terrain
	{
		Terrain::Region^ ConvertRegion(BWTA::Region* region)
		{
			if(region == NULL)
				return nullptr;
			return gcnew Region(region);
		}

		BWTA::Region* ConvertRegion(Terrain::Region^ region)
		{
			if(region == nullptr)
				return NULL;
			return region->instance;
		}

		Region::Region(BWTA::Region* region)
		{
			instance = region;
		}

		Terrain::Polygon^ Region::Polygon::get()
		{
			return ConvertPolygon(instance->getPolygon());
		}
			
		Api::Position^ Region::Center::get()
		{
			return ConvertPosition(instance->getCenter());
		}
			
		HashSet<Chokepoint^>^ Region::Chokepoints::get()
		{
			return ToHashSet<BWTA::Chokepoint*, Chokepoint^>(instance->getChokepoints(), &ConvertChokepoint);
		}
		
		HashSet<BaseLocation^>^ Region::BaseLocations::get()
		{
			return ToHashSet<BWTA::BaseLocation*, BaseLocation^>(instance->getBaseLocations(), &ConvertBaseLocation);
		}
			
		bool Region::IsReachable(Terrain::Region^ region)
		{
			return instance->isReachable(ConvertRegion(region));
		}
			
		HashSet<Region^>^ Region::ReachableRegions::get()
		{
			return ToHashSet<BWTA::Region*, Region^>(instance->getReachableRegions(), &ConvertRegion);
		}
	}
}
