#pragma once
#pragma unmanaged

#include <BWTA.h>

#pragma managed

#include "..\BWAPI\Player.h"
#include "..\BWAPI\Position.h"
#include "..\BWAPI\TilePosition.h"

#include "Chokepoint.h"
#include "Polygon.h"
#include "Region.h"
#include "BaseLocation.h"
#include "RectangleArray.h"

using namespace System::Collections::Generic;

namespace BroodWar
{
	namespace Terrain
	{
		public ref class TerrainAnalyzer abstract sealed
		{
		public:
			static void ReadMap();
			static void Analyze();
			static property HashSet<Terrain::Region^>^ Regions { HashSet<Terrain::Region^>^ get(); }
			static property HashSet<Terrain::Chokepoint^>^ Chokepoints { HashSet<Terrain::Chokepoint^>^ get(); }
			static property HashSet<Terrain::BaseLocation^>^ BaseLocations { HashSet<Terrain::BaseLocation^>^ get(); }
			static property HashSet<Terrain::BaseLocation^>^ StartLocations { HashSet<Terrain::BaseLocation^>^ get(); }
			static property HashSet<Terrain::Polygon^>^ UnwalkablePolygons { HashSet<Terrain::Polygon^>^ get(); }
			
			static Terrain::BaseLocation^ PlayerStartLocation(Api::Player^ player);
			
			static Terrain::Region^ GetRegion(int x, int y);
			static Terrain::Region^ GetRegion(Api::TilePosition^ tilePosition);
			static Terrain::Region^ GetRegion(Api::Position^ position);

			static Terrain::Chokepoint^ GetNearestChokepoint(int x, int y);
			static Terrain::Chokepoint^ GetNearestChokepoint(Api::TilePosition^ tilePosition);
			static Terrain::Chokepoint^ GetNearestChokepoint(Api::Position^ position);

			static Terrain::BaseLocation^ GetNearestBaseLocation(int x, int y);
			static Terrain::BaseLocation^ GetNearestBaseLocation(Api::TilePosition^ tilePosition);
			static Terrain::BaseLocation^ GetNearestBaseLocation(Api::Position^ position);

			static Terrain::Polygon^ GetNearestUnwalkablePolygon(int x, int y);
			static Terrain::Polygon^ GetNearestUnwalkablePolygon(Api::TilePosition^ tilePosition);
			static Api::Position^ GetNearestUnwalkablePosition(Api::Position^ position);

			static bool IsConnected(int x1, int y1, int x2, int y2);
			static bool IsConnected(Api::TilePosition^ a, Api::TilePosition^ b);

			static double GetGroundDistance(Api::TilePosition^ start, Api::TilePosition^ end);
			static Tuple<Api::TilePosition^, double>^ GetNearestTilePosition(Api::TilePosition^ start, ICollection<Api::TilePosition^>^ targets);
			static Dictionary<Api::TilePosition^, double>^ GetGroundDistances(Api::TilePosition^ start, ICollection<Api::TilePosition^>^ targets);
			static RectangleArray<double>^ GetGroundDistanceMap(Api::TilePosition^ start);
			static RectangleArray<double>^ GetGroundWalkDistanceMap(int walkx, int walky);
			static List<Api::TilePosition^>^ GetShortestPath(Api::TilePosition^ start, Api::TilePosition^ end);
			static List<Api::TilePosition^>^ GetShortestPath(Api::TilePosition^ start, ICollection<Api::TilePosition^>^ targets);
		};
	}
}
