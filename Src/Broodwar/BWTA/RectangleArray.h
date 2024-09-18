#pragma once
#pragma managed

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Linq;

namespace BroodWar
{
	namespace Terrain
	{
		generic<typename T>
		public ref class RectangleArray sealed
		{
		internal:
			unsigned int _width, _height;
			List<T>^ _container;

			RectangleArray(unsigned int width, unsigned int height)
			{
				_width = width;
				_height = height;
				_container = gcnew List<T>(_width * _height);
				_container->AddRange(Enumerable::Repeat(T(), _width * _height));
			}

		public:
			property unsigned int Width
			{
				virtual unsigned int get() sealed { return _width; }
			}

			property unsigned int Height
			{
				virtual unsigned int get() sealed { return _height; }
			}

			property T default[unsigned int, unsigned int]
			{
				virtual T get(unsigned int x, unsigned int y) sealed
				{
					if(x >= _width || y >= _height)
						throw gcnew IndexOutOfRangeException();
					return _container[y*_width + x];
				}
				internal:
				virtual void set(unsigned int x, unsigned int y, T value) sealed
				{
					if(x >= _width || y >= _height)
						throw gcnew IndexOutOfRangeException();
					_container[y*_width + x] = value;
				}
			}
		};
	}
}
