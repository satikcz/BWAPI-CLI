#pragma once
#pragma unmanaged

#include <BWTA\BaseLocation.h>

#pragma managed

#include "..\BWAPI\Position.h"
#include "..\BWAPI\TilePosition.h"
#include "..\BWAPI\Unit.h"

using namespace System::Collections::Generic;

namespace BroodWar
{
	namespace Terrain
	{
		ref class Region;

		public ref class BaseLocation sealed
		{
		internal:
			BWTA::BaseLocation* instance;

			BaseLocation(BWTA::BaseLocation* location);

		public:
			property Api::Position^ Position { Api::Position^ get(); }
			
			property Api::TilePosition^ TilePosition { Api::TilePosition^ get(); }

			property Terrain::Region^ Region { Terrain::Region^ get(); }

			property int MineralsCount { int get(); }

			property int Gas { int get(); }

			property HashSet<Api::Unit^>^ Minerals { HashSet<Api::Unit^>^ get(); }

			property HashSet<Api::Unit^>^ StaticMinerals { HashSet<Api::Unit^>^ get(); }

			property HashSet<Api::Unit^>^ Geysers { HashSet<Api::Unit^>^ get(); }
    
			double GroundDistanceTo(BaseLocation^ other);

			double AirDistanceTo(BaseLocation^ other);

			property bool IsIsland { bool get(); }

			property bool IsMineralOnly { bool get(); }

			property bool IsStartLocation { bool get(); }
		};

		BaseLocation^ ConvertBaseLocation(BWTA::BaseLocation* location);
		BWTA::BaseLocation* ConvertBaseLocation(BaseLocation^ location);
	}
}
