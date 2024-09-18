#include "BaseLocation.h"
#include "Region.h"
#include "..\Util.h"

using namespace BroodWar;
using namespace BroodWar::Api;

namespace BroodWar
{
	namespace Terrain
	{
		BaseLocation^ ConvertBaseLocation(BWTA::BaseLocation* location)
		{
			if(location == NULL)
				return nullptr;
			return gcnew BaseLocation(location);
		}

		BWTA::BaseLocation* ConvertBaseLocation(BaseLocation^ location)
		{
			if(location == nullptr)
				return NULL;
			return location->instance;
		}

		BaseLocation::BaseLocation(BWTA::BaseLocation* location)
		{
			instance = location;
		}

		Api::Position^ BaseLocation::Position::get()
		{
			return ConvertPosition(instance->getPosition());
		}

		Api::TilePosition^ BaseLocation::TilePosition::get()
		{
			return ConvertTile(instance->getTilePosition());
		}

		Terrain::Region^ BaseLocation::Region::get()
		{
			return ConvertRegion(instance->getRegion());
		}

		int BaseLocation::MineralsCount::get()
		{
			return instance->minerals();
		}

		int BaseLocation::Gas::get()
		{
			return instance->gas();
		}

		HashSet<Api::Unit^>^ BaseLocation::Minerals::get()
		{
			return ToHashSet<BWAPI::Unit*, Api::Unit^>(instance->getMinerals(), &Api::ConvertUnit);
		}

		HashSet<Api::Unit^>^ BaseLocation::StaticMinerals::get()
		{
			return ToHashSet<BWAPI::Unit*, Api::Unit^>(instance->getStaticMinerals(), &Api::ConvertUnit);
		}

		HashSet<Api::Unit^>^ BaseLocation::Geysers::get()
		{
			return ToHashSet<BWAPI::Unit*, Api::Unit^>(instance->getGeysers(), &Api::ConvertUnit);
		}
    
		double BaseLocation::GroundDistanceTo(BaseLocation^ other)
		{
			return instance->getGroundDistance(ConvertBaseLocation(other));
		}

		double BaseLocation::AirDistanceTo(BaseLocation^ other)
		{
			return instance->getAirDistance(ConvertBaseLocation(other));
		}

		bool BaseLocation::IsIsland::get()
		{
			return instance->isIsland();
		}

		bool BaseLocation::IsMineralOnly::get()
		{
			return instance->isMineralOnly();
		}

		bool BaseLocation::IsStartLocation::get()
		{
			return instance->isStartLocation();
		}
	}
}
