#pragma unmanaged

#include <stddef.h>

#pragma managed

#include "TilePosition.h"

namespace BroodWar
{
	namespace Api
	{
		BWAPI::TilePosition ConvertTile(TilePosition^ position)
		{
			if(position == nullptr)
				return BWAPI::TilePositions::None;
			return *(position->instance);
		}

		TilePosition^ ConvertTile(BWAPI::TilePosition position)
		{
			return gcnew TilePosition(position);
		}

		TilePosition^ ConvertTile(BWAPI::TilePosition* position)
		{
			if(position == NULL)
				return nullptr;
			return gcnew TilePosition(position);
		}

		TilePosition::TilePosition(BWAPI::TilePosition *position)
		{
			instance = position;
			dispose = false;
		}

		TilePosition::TilePosition(BWAPI::TilePosition position)
		{
			instance = new BWAPI::TilePosition(position);
			dispose = true;
		}

		TilePosition::~TilePosition()
		{
			if(dispose)
				delete instance;
		}

		TilePosition::!TilePosition()
		{
			if(dispose)
				delete instance;
		}

		TilePosition::TilePosition()
		{
			instance = new BWAPI::TilePosition();
			dispose = true;
		}

		TilePosition::TilePosition(TilePosition^ copy)
		{
			instance = new BWAPI::TilePosition(*(copy->instance));
		}

		TilePosition::TilePosition(int x, int y)
		{
			instance = new BWAPI::TilePosition(x, y);
			dispose = true;
		}

		int TilePosition::X::get()
		{
			return instance->x();
		}

		int TilePosition::Y::get()
		{
			return instance->y();
		}

		bool TilePosition::IsValid::get()
		{
			return instance->isValid();
		}

		double TilePosition::Distance(TilePosition^ position)
		{
			return instance->getDistance(*(position->instance));
		}

		double TilePosition::Length::get()
		{
			return instance->getLength();
		}

		bool TilePosition::HasPath(TilePosition^ position)
		{
			return instance->hasPath(*(position->instance));
		}

		void TilePosition::MakeValid()
		{
			instance->makeValid();
		}

		bool TilePosition::IsInvalid::get()
		{
			return *instance == BWAPI::TilePositions::Invalid;
		}

		bool TilePosition::IsNone::get()
		{
			return *instance == BWAPI::TilePositions::None;
		}

		bool TilePosition::IsUnknown::get()
		{
			return *instance == BWAPI::TilePositions::Unknown;
		}

		int TilePosition::GetHashCode()
		{
			return instance->x() * 397 ^ instance->y();
		}

		bool TilePosition::Equals(Object^ o)
		{
			TilePosition^ other = dynamic_cast<TilePosition^>(o);
			return this->Equals(other);
		}

		bool TilePosition::Equals(TilePosition^ other)
		{
			if(ReferenceEquals(nullptr, other))
				return false;
			if(ReferenceEquals(this, other))
				return true;
			return this->instance->x() == other->instance->x()
				&& this->instance->y() == other->instance->y();
		}

		bool TilePosition::operator == (TilePosition^ first, TilePosition^ second)
		{
			if (ReferenceEquals(first, second))
				return true;
			if (ReferenceEquals(nullptr, first) ||ReferenceEquals(nullptr, second) )
				return false;
			return *(first->instance) == *(second->instance);
		}

		bool TilePosition::operator != (TilePosition^ first, TilePosition^ second)
		{
			if (ReferenceEquals(first, second))
				return false;
			if (ReferenceEquals(nullptr, first) ||ReferenceEquals(nullptr, second) )
				return true;
			return *(first->instance) != *(second->instance);
		}

		bool TilePosition::operator < (TilePosition^ first, TilePosition^ second)
		{
			return *(first->instance) < *(second->instance);
		}

		TilePosition^ TilePosition::operator + (TilePosition^ first, TilePosition^ second)
		{
			return ConvertTile(*(first->instance) + *(second->instance));
		}

		TilePosition^ TilePosition::operator - (TilePosition^ first, TilePosition^ second)
		{
			return ConvertTile(*(first->instance) - *(second->instance));
		}

		TilePosition^ TilePosition::operator += (TilePosition^ first, TilePosition^ second)
		{
			*(first->instance) += *(second->instance);
			return first;
		}

		TilePosition^ TilePosition::operator -= (TilePosition^ first, TilePosition^ second)
		{
			*(first->instance) -= *(second->instance);
			return first;
		}

		TilePosition^ TilePosition::None::get()
		{
			return ConvertTile(BWAPI::TilePositions::None);
		}

		TilePosition^ TilePosition::Unknown::get()
		{
			return ConvertTile(BWAPI::TilePositions::Unknown);
		}

		TilePosition^ TilePosition::Invalid::get()
		{
			return ConvertTile(BWAPI::TilePositions::Invalid);
		}
	}
}
