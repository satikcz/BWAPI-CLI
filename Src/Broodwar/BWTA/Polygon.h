#pragma once
#pragma unmanaged

#include <BWTA\Polygon.h>

#pragma managed

#include "..\BWAPI\Position.h"

using namespace System::Collections::Generic;

namespace BroodWar
{
	namespace Terrain
	{
		public ref class Polygon sealed : public IList<Api::Position^>
		{
		internal:
			BWTA::Polygon* instance;
			bool dispose;

			Polygon(BWTA::Polygon* polygon);
			Polygon(BWTA::Polygon polygon);
			~Polygon();
			!Polygon();

		public:
			Polygon();
			
			Polygon(Polygon^ copy);

			property double Area { double get(); }
			
			property double Perimeter { double get(); }
			
			property Api::Position^ Center { Api::Position^ get(); }
			
			bool IsInside(Api::Position^ p);
			
			Api::Position^ GetNearestPosition(Api::Position^ p);
			
			property List<Terrain::Polygon^>^ Holes { List<Terrain::Polygon^>^ get(); }

			#pragma region IList implementation
			virtual void Add(Api::Position^ item);
			virtual void Clear();
			virtual bool Contains(Api::Position^ item);
			virtual void CopyTo(array<Api::Position^>^ target, int arrayIndex);
			virtual IEnumerator<Api::Position^>^ GetEnumerator();
			virtual int IndexOf(Api::Position^ item);
			virtual void Insert(int index, Api::Position^ item);
			virtual bool Remove(Api::Position^ item);
			virtual void RemoveAt(int index);
			
			virtual property int Count { int get(); }
			virtual property bool IsReadOnly { bool get(); }
			virtual property Api::Position^ default[int]
			{
				Api::Position^ get(int index);
				void set(int index, Api::Position^ item);
			}

		private:
			virtual System::Collections::IEnumerator^ GetEnumerator2() sealed = System::Collections::IEnumerable::GetEnumerator;
			#pragma endregion
		};

		Terrain::Polygon^ ConvertPolygon(BWTA::Polygon polygon);
		Terrain::Polygon^ ConvertPolygon(BWTA::Polygon* polygon);
		BWTA::Polygon* ConvertPolygon(Terrain::Polygon^ polygon);
	}
}
