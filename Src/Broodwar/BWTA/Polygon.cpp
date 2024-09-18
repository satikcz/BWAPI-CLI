#include "Polygon.h"
#include "..\Util.h"

using namespace System;
using namespace BroodWar::Api;

namespace BroodWar
{
	namespace Terrain
	{
		private ref class PolygonEnumerator : public IEnumerator<Api::Position^>
		{
		private:
			std::vector<BWAPI::Position>* instance;
			std::vector<BWAPI::Position>::iterator* it;
			bool first;

		public:
			PolygonEnumerator(std::vector<BWAPI::Position>* vc)
			{
				instance = vc;
				it = new std::vector<BWAPI::Position>::iterator();
				*it = instance->begin();
				first = true;
			}

			~PolygonEnumerator() //this is our dispose method
			{
				if(it != NULL)
					delete it;
			}

			virtual property Api::Position^ Current
			{
				Api::Position^ get()
				{
					if(it == NULL)
						throw gcnew NullReferenceException("iterator");
					return gcnew Api::Position(**it);
				}
			}

			virtual void Reset()
			{
				*it = instance->begin();
				first = true;
			}

			virtual bool MoveNext()
			{
				if(first)
					first = false;
				else
					(*it)++;

				if(*it == instance->end())
					return false;
				return true;
			}

		private:
			virtual property Object^ CurrentObject
			{
				virtual Object^ get() sealed = System::Collections::IEnumerator::Current::get
				{
					return this->Current;
				}
			}
		};

		Terrain::Polygon^ ConvertPolygon(BWTA::Polygon polygon)
		{
			return gcnew Polygon(polygon);
		}

		Terrain::Polygon^ ConvertPolygon(BWTA::Polygon* polygon)
		{
			if(polygon == NULL)
				return nullptr;
			return gcnew Polygon(polygon);
		}

		BWTA::Polygon* ConvertPolygon(Terrain::Polygon^ polygon)
		{
			if(polygon == nullptr)
				return NULL;
			return polygon->instance;
		}

		Polygon::Polygon(BWTA::Polygon* polygon)
		{
			instance = polygon;
			dispose = false;
		}

		Polygon::Polygon(BWTA::Polygon polygon)
		{
			instance = new BWTA::Polygon(polygon);
			dispose = true;
		}

		Polygon::~Polygon()
		{
			if(dispose)
				delete instance;
		}

		Polygon::!Polygon()
		{
			if(dispose)
				delete instance;
		}

		Polygon::Polygon()	
		{
			instance = new BWTA::Polygon();
			dispose = true;
		}
			
		Polygon::Polygon(Polygon^ copy)
		{
			instance = new BWTA::Polygon(*(copy->instance));
			dispose = true;
		}

		double Polygon::Area::get()
		{
			return instance->getArea();
		}
			
		double Polygon::Perimeter::get()
		{
			return instance->getPerimeter();
		}
			
		Api::Position^ Polygon::Center::get()
		{
			return ConvertPosition(instance->getCenter());
		}
			
		bool Polygon::IsInside(Api::Position^ p)
		{
			return instance->isInside(ConvertPosition(p));
		}
			
		Api::Position^ Polygon::GetNearestPosition(Api::Position^ p)
		{
			return ConvertPosition(instance->getNearestPoint(ConvertPosition(p)));
		}
			
		List<Terrain::Polygon^>^ Polygon::Holes::get()
		{
			return ToList<BWTA::Polygon, Terrain::Polygon^>(instance->getHoles(), &ConvertPolygon);
		}

		void Polygon::Add(Api::Position^ item)
		{
			instance->push_back(ConvertPosition(item));
		}
		
		void Polygon::Clear()
		{
			instance->clear();
		}
		
		bool Polygon::Contains(Api::Position^ item)
		{
			throw gcnew NotImplementedException();
		}
		
		void Polygon::CopyTo(array<Api::Position^>^ target, int arrayIndex)
		{
			throw gcnew NotImplementedException();
		}
		
		IEnumerator<Api::Position^>^ Polygon::GetEnumerator()
		{
			return gcnew PolygonEnumerator(instance);
		}
		
		int Polygon::IndexOf(Api::Position^ item)
		{
			throw gcnew NotImplementedException();
		}
		
		void Polygon::Insert(int index, Api::Position^ item)
		{
			instance->insert(instance->begin() + index, *(item->instance));
		}
		
		bool Polygon::Remove(Api::Position^ item)
		{
			throw gcnew NotImplementedException();
		}
		
		void Polygon::RemoveAt(int index)
		{
			instance->erase(instance->begin() + index);
		}
			
		int Polygon::Count::get()
		{
			return instance->size();
		}
		
		bool Polygon::IsReadOnly::get()
		{
			return false;
		}
		
		Api::Position^ Polygon::default::get(int index)
		{
			if((unsigned int)index >= instance->size())
				throw gcnew IndexOutOfRangeException();
			return ConvertPosition(instance->at(index));
		}

		void Polygon::default::set(int index, Api::Position^ item)
		{
			if((unsigned int)index >= instance->size())
				throw gcnew IndexOutOfRangeException();
			instance->at(index) = ConvertPosition(item);
		}

		System::Collections::IEnumerator^ Polygon::GetEnumerator2()
		{
			return this->GetEnumerator();
		}
	}
}
