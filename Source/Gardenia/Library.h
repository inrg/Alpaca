#pragma once

#include <windows.h>
#include <map>
#include "Error.h"
#include "Utilities/VersionUtility.h"

#include "../Commons/D2UnitStruct.h"
#include "../Commons/d2BinFile.h"
#include "../Commons/d2Struct.h"

using VersionOffsets = std::map<VersionUtility::Versions, DWORD>;

class Library
{
public:
	// Load the Library (This is the DLLs base Offset that we can use to
	// determine the game version for the dll). This should be identical
	// to the Game Version of the .exe.
	DWORD LoadDiabloLibrary();
	
	void HookLibrary();
	void UnhookLibrary();

	int DllVersion;
	DWORD DllOffset;
	LPCTSTR DllName;

	/*DWORD GameOffset;
	int GameVersion;*/

	// Retrieves the address by using GetProcAddress
	DWORD GetOffsetByProc(const VersionOffsets moduleOffsets);
	DWORD GetOffsetByProc(DWORD V109, DWORD V109D, DWORD V110, DWORD V111, DWORD V111B, DWORD V112, DWORD V113C, DWORD V113D);

	// Retrieves the address by adding the DLLs base offset to the recorded offset
	DWORD GetOffsetByAddition(const VersionOffsets moduleOffsets);
	DWORD GetOffsetByAddition(DWORD V109, DWORD V109D, DWORD V110, DWORD V111, DWORD V111B, DWORD V112, DWORD V113C, DWORD V113D);

	// Offsets for functions in these specific versions. When updating to a new diablo version, you will
	// want to add an entry to each of the function sets in each library that require an update.
	DWORD GetOffsetForVersion(const VersionOffsets& offsets);
	DWORD GetOffsetForVersion(DWORD V109, DWORD V109D, DWORD V110, DWORD V111, DWORD V111B, DWORD V112, DWORD V113C, DWORD V113D);

	// Contains all offsets that are possible for this particular function pointer.
	VersionOffsets CreateOffsets(DWORD V109, DWORD V109D, DWORD V110, DWORD V111, DWORD V111B, DWORD V112, DWORD V113C, DWORD V113D);

protected:
	// This constructor is only used to load up the Game.exe since at this point we don't have the version information.
	// Prevent this class from being instantiated. (pure virtual classes can't be instantiated anyways)
	Library();

	// Each subclass will locate the functions that we need.
	virtual void SetFunctions() = 0;

	// Retrieves the address using GetProcAddress
	DWORD GetFunctionAddress(LPCSTR index);
};