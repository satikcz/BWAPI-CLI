#pragma once
#pragma unmanaged

#include <BWTA\Chokepoint.h>

#pragma managed

#include "..\BWAPI\Position.h"

using namespace System;

namespace BroodWar
{
	namespace Terrain
	{
		ref class Region;

		public ref class Chokepoint sealed
		{
		internal:
			BWTA::Chokepoint* instance;

			Chokepoint(BWTA::Chokepoint* point);

		public:
			property Tuple<Region^, Region^>^ Regions { Tuple<Region^, Region^>^ get(); }

			property Tuple<Api::Position^, Api::Position^>^ Sides { Tuple<Api::Position^, Api::Position^>^ get(); }

			property Api::Position^ Center { Api::Position^ get(); }

			property double Width { double get(); }
		};
		
		Chokepoint^ ConvertChokepoint(BWTA::Chokepoint* point);
		BWTA::Chokepoint* ConvertChokepoint(Chokepoint^ point);
	}
}
