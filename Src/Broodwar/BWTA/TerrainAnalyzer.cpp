#pragma unmanaged

#include <stdio.h>
#include <string>
#include <BWTA\RectangleArray.h>

#pragma managed

#include "TerrainAnalyzer.h"
#include "..\Util.h"

using namespace BroodWar::Api;

extern BWAPI::Game* BWAPI::Broodwar;

namespace BroodWar
{
	namespace Terrain
	{
		void TerrainAnalyzer::ReadMap()
		{
			BWTA::readMap();
		}
		
		void TerrainAnalyzer::Analyze()
		{
			BWTA::analyze();
		}
		
		HashSet<Terrain::Region^>^ TerrainAnalyzer::Regions::get()
		{
			return ToHashSet<BWTA::Region*, Terrain::Region^>(BWTA::getRegions(), &ConvertRegion);
		}
		
		HashSet<Terrain::Chokepoint^>^ TerrainAnalyzer::Chokepoints::get()
		{
			return ToHashSet<BWTA::Chokepoint*, Terrain::Chokepoint^>(BWTA::getChokepoints(), &ConvertChokepoint);
		}
		
		HashSet<Terrain::BaseLocation^>^ TerrainAnalyzer::BaseLocations::get()
		{
			return ToHashSet<BWTA::BaseLocation*, Terrain::BaseLocation^>(BWTA::getBaseLocations(), &ConvertBaseLocation);
		}
		
		HashSet<Terrain::BaseLocation^>^ TerrainAnalyzer::StartLocations::get()
		{
			return ToHashSet<BWTA::BaseLocation*, Terrain::BaseLocation^>(BWTA::getStartLocations(), &ConvertBaseLocation);
		}
		
		HashSet<Terrain::Polygon^>^ TerrainAnalyzer::UnwalkablePolygons::get()
		{
			return ToHashSet<BWTA::Polygon*, Terrain::Polygon^>(BWTA::getUnwalkablePolygons(), &ConvertPolygon);
		}

		Terrain::BaseLocation^ TerrainAnalyzer::PlayerStartLocation(Api::Player^ player)
		{
			return ConvertBaseLocation(BWTA::getStartLocation(player->instance));
		}

		Terrain::Region^ TerrainAnalyzer::GetRegion(int x, int y)
		{
			return ConvertRegion(BWTA::getRegion(x, y));
		}
		
		Terrain::Region^ TerrainAnalyzer::GetRegion(Api::TilePosition^ tilePosition)
		{
			return ConvertRegion(BWTA::getRegion(ConvertTile(tilePosition)));
		}
		
		Terrain::Region^ TerrainAnalyzer::GetRegion(Api::Position^ position)
		{
			return ConvertRegion(BWTA::getRegion(ConvertPosition(position)));
		}

		Terrain::Chokepoint^ TerrainAnalyzer::GetNearestChokepoint(int x, int y)
		{
			return ConvertChokepoint(BWTA::getNearestChokepoint(x, y));
		}
		
		Terrain::Chokepoint^ TerrainAnalyzer::GetNearestChokepoint(Api::TilePosition^ tilePosition)
		{
			return ConvertChokepoint(BWTA::getNearestChokepoint(ConvertTile(tilePosition)));
		}
		
		Terrain::Chokepoint^ TerrainAnalyzer::GetNearestChokepoint(Api::Position^ position)
		{
			return ConvertChokepoint(BWTA::getNearestChokepoint(ConvertPosition(position)));
		}

		Terrain::BaseLocation^ TerrainAnalyzer::GetNearestBaseLocation(int x, int y)
		{
			return ConvertBaseLocation(BWTA::getNearestBaseLocation(x, y));
		}
		
		Terrain::BaseLocation^ TerrainAnalyzer::GetNearestBaseLocation(Api::TilePosition^ tilePosition)
		{
			return ConvertBaseLocation(BWTA::getNearestBaseLocation(ConvertTile(tilePosition)));
		}
		
		Terrain::BaseLocation^ TerrainAnalyzer::GetNearestBaseLocation(Api::Position^ position)
		{
			return ConvertBaseLocation(BWTA::getNearestBaseLocation(ConvertPosition(position)));
		}

		Terrain::Polygon^ TerrainAnalyzer::GetNearestUnwalkablePolygon(int x, int y)
		{
			return ConvertPolygon(BWTA::getNearestUnwalkablePolygon(x, y));
		}
		
		Terrain::Polygon^ TerrainAnalyzer::GetNearestUnwalkablePolygon(Api::TilePosition^ tilePosition)
		{
			return ConvertPolygon(BWTA::getNearestUnwalkablePolygon(ConvertTile(tilePosition)));
		}
		
		Api::Position^ TerrainAnalyzer::GetNearestUnwalkablePosition(Api::Position^ position)
		{
			return ConvertPosition(BWTA::getNearestUnwalkablePosition(ConvertPosition(position)));
		}

		bool TerrainAnalyzer::IsConnected(int x1, int y1, int x2, int y2)
		{
			return BWTA::isConnected(x1, y1, x2, y2);
		}
		
		bool TerrainAnalyzer::IsConnected(Api::TilePosition^ a, Api::TilePosition^ b)
		{
			return BWTA::isConnected(ConvertTile(a), ConvertTile(b));
		}

		double TerrainAnalyzer::GetGroundDistance(Api::TilePosition^ start, Api::TilePosition^ end)
		{
			return BWTA::getGroundDistance(ConvertTile(start), ConvertTile(end));
		}
		
		Tuple<Api::TilePosition^, double>^ TerrainAnalyzer::GetNearestTilePosition(Api::TilePosition^ start, ICollection<Api::TilePosition^>^ targets)
		{
			std::set<BWAPI::TilePosition> s = ToSet<Api::TilePosition^, BWAPI::TilePosition>(targets, &Api::ConvertTile);
			return ToTuple<BWAPI::TilePosition, double, Api::TilePosition^, double>(
				BWTA::getNearestTilePosition(*(start->instance), s),
				&Api::ConvertTile,
				&DoubleToDouble);
		}
		
		Dictionary<Api::TilePosition^, double>^ TerrainAnalyzer::GetGroundDistances(Api::TilePosition^ start, ICollection<Api::TilePosition^>^ targets)
		{
			std::set<BWAPI::TilePosition> s = ToSet<Api::TilePosition^, BWAPI::TilePosition>(targets, &Api::ConvertTile);
			std::map<BWAPI::TilePosition, double> p = BWTA::getGroundDistances(*(start->instance), s);
			return ToDictionary<BWAPI::TilePosition, double, Api::TilePosition^, double>(p, &Api::ConvertTile, &DoubleToDouble);
		}
		
		RectangleArray<double>^ TerrainAnalyzer::GetGroundDistanceMap(Api::TilePosition^ start)
		{
			BWTA::RectangleArray<double> map(BWAPI::Broodwar->mapWidth(), BWAPI::Broodwar->mapHeight());
			BWTA::getGroundDistanceMap(*(start->instance), map);
			RectangleArray<double>^ rez = gcnew RectangleArray<double>(BWAPI::Broodwar->mapWidth(), BWAPI::Broodwar->mapHeight());
			for(int x = 0; x < BWAPI::Broodwar->mapWidth(); x++)
			{
				for(int y = 0; y < BWAPI::Broodwar->mapHeight(); y++)
				{
					rez[x, y] = map[x][y];
				}
			}
			return rez;
		}

		RectangleArray<double>^ TerrainAnalyzer::GetGroundWalkDistanceMap(int walkx, int walky)
		{
			BWTA::RectangleArray<double> map(BWAPI::Broodwar->mapWidth(), BWAPI::Broodwar->mapHeight());
			BWTA::getGroundWalkDistanceMap(walkx, walky, map);
			RectangleArray<double>^ rez = gcnew RectangleArray<double>(BWAPI::Broodwar->mapWidth(), BWAPI::Broodwar->mapHeight());
			for(int x = 0; x < BWAPI::Broodwar->mapWidth(); x++)
			{
				for(int y = 0; y < BWAPI::Broodwar->mapHeight(); y++)
				{
					rez[x, y] = map[x][y];
				}
			}
			return rez;
		}

		List<Api::TilePosition^>^ TerrainAnalyzer::GetShortestPath(Api::TilePosition^ start, Api::TilePosition^ end)
		{
			return ToList<BWAPI::TilePosition, Api::TilePosition^>(
				BWTA::getShortestPath(Api::ConvertTile(start), Api::ConvertTile(end)),
				&Api::ConvertTile);
		}
		
		List<Api::TilePosition^>^ TerrainAnalyzer::GetShortestPath(Api::TilePosition^ start, ICollection<Api::TilePosition^>^ targets)
		{
			std::set<BWAPI::TilePosition> s = ToSet<Api::TilePosition^, BWAPI::TilePosition>(targets, &Api::ConvertTile);
			std::vector<BWAPI::TilePosition> p = BWTA::getShortestPath(*(start->instance), s);
			return ToList<BWAPI::TilePosition, Api::TilePosition^>(p, &Api::ConvertTile);
		}
	}
}