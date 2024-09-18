#pragma once
#pragma unmanaged

#include <BWAPI\Position.h>
#pragma make_public(BWAPI::Position)

#pragma managed

#include "TilePosition.h"

namespace BroodWar
{
	namespace Api
	{
		public ref class Position sealed
		{
		internal:
			BWAPI::Position *instance;
			bool dispose;

			Position(BWAPI::Position *position);
			Position(BWAPI::Position position);
			~Position();
			!Position();

		public:
			Position();
			Position(Position^ copy);
			Position(TilePosition^ copy);
			Position(int x, int y);

			property int X { int get(); }
			property int Y { int get(); }
			property bool IsValid { bool get(); }
			double Distance(Position^ position);
			int ApproximateDistance(Position^ position);
			property double Length { double get(); }
			bool HasPath(Position^ position);
			void MakeValid();
			property bool IsInvalid { bool get(); }
			property bool IsNone { bool get(); }
			property bool IsUnknown { bool get(); }

			virtual int GetHashCode() override;
			virtual bool Equals(Object^ o) override;
			bool Equals(Position^ other);

			static bool operator == (Position^ first, Position^ second);
			static bool operator != (Position^ first, Position^ second);
			static bool operator < (Position^ first, Position^ second);
			static Position^ operator + (Position^ first, Position^ second);
			static Position^ operator - (Position^ first, Position^ second);
			static Position^ operator += (Position^ first, Position^ second);
			static Position^ operator -= (Position^ first, Position^ second);
		};

		BWAPI::Position ConvertPosition(Position^ position);
		Position^ ConvertPosition(BWAPI::Position position);
		Position^ ConvertPosition(BWAPI::Position* position);
	}
}
