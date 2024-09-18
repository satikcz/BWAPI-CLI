#pragma once
#pragma unmanaged

#include <BWTA\Region.h>

#pragma managed

#include "Polygon.h"

using namespace System::Collections::Generic;

namespace BroodWar
{
	namespace Terrain
	{
		ref class Chokepoint;
		ref class BaseLocation;

		public ref class Region sealed
		{
		internal:
			BWTA::Region* instance;

			Region(BWTA::Region* region);

		public:
			property Terrain::Polygon^ Polygon { Terrain::Polygon^ get(); }
			
			property Api::Position^ Center { Api::Position^ get(); }
			
			property HashSet<Chokepoint^>^ Chokepoints { HashSet<Chokepoint^>^ get(); }
			
			property HashSet<BaseLocation^>^ BaseLocations { HashSet<BaseLocation^>^ get(); }
			
			bool IsReachable(Terrain::Region^ region);
			
			property HashSet<Region^>^ ReachableRegions { HashSet<Region^>^ get(); }
		};

		Terrain::Region^ ConvertRegion(BWTA::Region* region);
		BWTA::Region* ConvertRegion(Terrain::Region^ region);
	}
}
