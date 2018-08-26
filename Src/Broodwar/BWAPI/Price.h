#pragma once

using namespace System;

namespace BroodWar
{
  namespace Api
  {
    public ref class Price sealed
    {
    private:
      int _minerals, _gas, _timeFrames, _supply;
    public:
      Price(int minerals, int gas, int timeFrames, int supply);

      virtual String^ ToString() override
      {
        return "Minerals: " + Minerals + ", Gas: " + Gas + (Supply > 0 ? (", Supply: " + Supply) : "");
      }

      property int Minerals { int get(); }

      property int Gas { int get(); }

      property int TimeFrames { int get(); }

      property int Supply { int get(); }
    };
  }
}
