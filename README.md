# BWAPI-CLI
## .NET wrapper for the Broodwar API (BWAPI)
.NET wrapper for the Broodwar API (BWAPI) written in C++/CLI

Currently wraps BWAPI 4.2.0 and supports VS 2017.

This project is a C++/CLI wrapper for the Broodwar API. This project is also aimed to provide APIs that are higher level or more suited to F#/C# style than original BWAPI.

WARNING: API will change during development. It is recommended that maintain your own copy be prepared to manually merge any changes you specifically desire.

Project is in alpha stage, but library can run dll AI modules. Project is built with Visual Studio 2017, using BWAPI libraries directly.

It is highly recommended that you have experience in setting up a BWAPI bot before dealing with this library. For now we do not provide any binaries so you'll have to build it by yourself.

To use AI you have to place Broodwar.dll, BroodwarLoader.dll and BroodwarLoader.dll.config and your AI module into same folder. BWAPI config must point to BroodwarLoader.dll as AI module. Edit .config file. The Assembly key must point to your AI assembly (with extension), and the Module key must point to your class that implements AiBase. ExampleAIModule ported to C# and to F# is also provided.

## Original Version
This one is ported from https://github.com/Lamarth/BWAPI-CLI

That was a port of BWAPI-CLI for version 3.7.3 created by ZeroFusion at http://bwapicli.codeplex.com/

## Issues
This project a port from https://github.com/Lamarth/BWAPI-CLI with some small changes to make it somehow compile and run with one bugfix (there were some upgrade enum errors that shifted few enum entries).

There are currently no known issues, at least when I tried to write my own bot.