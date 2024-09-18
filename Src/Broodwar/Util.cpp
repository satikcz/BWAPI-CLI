#include "Util.h"
#include <BWAPI.h>
#include "BWAPI\Player.h"

using namespace System::IO;
using namespace System::Runtime::InteropServices;

extern BWAPI::Game* BWAPI::Broodwar;

namespace BroodWar
{
	double DoubleToDouble(double d)
	{
		return d;
	}

	int IntToInt(int i)
	{
		return i;
	}

	void Util::Log(String^ string)
	{
		String^ fileName = Path::Combine(
			Environment::GetFolderPath(Environment::SpecialFolder::Personal),
			"text-from-lib.txt");
		StreamWriter^ file = gcnew StreamWriter(fileName, true);
		file->WriteLine(string);
		file->Close();

		IntPtr ptr = Marshal::StringToHGlobalAnsi(string);
		BWAPI::Broodwar->sendText((char*)ptr.ToPointer());
		Marshal::FreeHGlobal(ptr);
	}

	void Util::LogException(String^ module, Exception^ ex)
	{
		Log(String::Format("Unexpected error occured during {0}: {1}{2}{3}", module, ex->Message, Environment::NewLine, ex->StackTrace));
		while((ex = ex->InnerException) != nullptr)
		{
			Log(String::Format("Internal error is: {0}{1}{2}", ex->Message, Environment::NewLine, ex->StackTrace));
		}
	}

	Api::Player^ Util::ConvertPlayer(BWAPI::Player* player)
	{
		return Api::ConvertPlayer(player);
	}

	Api::Unit^ Util::ConvertUnit(BWAPI::Unit* unit)
	{
		return Api::ConvertUnit(unit);
	}

	Api::Position^ Util::ConvertPosition(BWAPI::Position position)
	{
		return Api::ConvertPosition(position);
	}

	void Util::LoadGameInstance(BWAPI::Game* game)
	{
		BWAPI::Broodwar = game;
	}
}
