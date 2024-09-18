#pragma once
#pragma unmanaged

#include <BWAPI\TilePosition.h>

#pragma managed

namespace BroodWar
{
	namespace Api
	{
		public ref class TilePosition sealed
		{
		internal:
			BWAPI::TilePosition *instance;
			bool dispose;

			TilePosition(BWAPI::TilePosition *position);
			TilePosition(BWAPI::TilePosition position);
			~TilePosition();
			!TilePosition();

		public:
			TilePosition();
			TilePosition(TilePosition^ copy);
			TilePosition(int x, int y);

			property int X { int get(); }
			property int Y { int get(); }
			property bool IsValid { bool get(); }
			double Distance(TilePosition^ position);
			property double Length { double get(); }
			bool HasPath(TilePosition^ position);
			void MakeValid();
			property bool IsInvalid { bool get(); }
			property bool IsNone { bool get(); }
			property bool IsUnknown { bool get(); }

			virtual int GetHashCode() override;
			virtual bool Equals(Object^ o) override;
			bool Equals(TilePosition^ other);

			static bool operator == (TilePosition^ first, TilePosition^ second);
			static bool operator != (TilePosition^ first, TilePosition^ second);
			static bool operator < (TilePosition^ first, TilePosition^ second);
			static TilePosition^ operator + (TilePosition^ first, TilePosition^ second);
			static TilePosition^ operator - (TilePosition^ first, TilePosition^ second);
			static TilePosition^ operator += (TilePosition^ first, TilePosition^ second);
			static TilePosition^ operator -= (TilePosition^ first, TilePosition^ second);

			static property TilePosition^ None { TilePosition^ get(); }
			static property TilePosition^ Unknown { TilePosition^ get(); }
			static property TilePosition^ Invalid { TilePosition^ get(); }
		};

		BWAPI::TilePosition ConvertTile(TilePosition^ position);
		TilePosition^ ConvertTile(BWAPI::TilePosition position);
		TilePosition^ ConvertTile(BWAPI::TilePosition* position);
	}
}
