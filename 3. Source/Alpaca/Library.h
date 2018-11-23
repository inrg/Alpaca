// Copyright (C) 2004-2017 Yohann Nicolas
// Copyright (C) 2017-2018 Jonathan Vasquez <jon@xyinn.org>
//
// This program is free software : you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <windows.h>

#include "Error.h"
#include "VersionUtility.h"
#include "D2UnitStruct.h"
#include "D2BinFile.h"
#include "D2Struct.h"
#include "Memory.h"

template <class Child>
class Library
{
public:
	static char* Name;
	static DWORD Offset;

	// Load the Library (This is the DLLs base Offset that we can use to
	// determine the game version for the dll). This should be identical
	// to the Game Version of the .exe.
	static DWORD LoadDiabloLibrary()
	{
		DWORD proposedOffset = (DWORD)LoadLibrary(Name);
		if (proposedOffset == NULL)
		{
			log_msg("Failed to load library: %s\n", Name);
			exit(-1);
		}
		log_msg("%s successfully loaded.\n", Name);
		return proposedOffset;
	}

	static void HookLibrary()
	{
		DWORD dw = 0;
		BYTE* offsetPESignature = Offset + *(BYTE**)((BYTE*)Offset + 0x3C);
		DWORD sizeOfCode = *(DWORD*)(offsetPESignature + 0x1C);
		BYTE* baseOfCode = Offset + *(BYTE**)(offsetPESignature + 0x2C);
		if (!VirtualProtect(baseOfCode, sizeOfCode, PAGE_EXECUTE_READWRITE, &dw))
		{
			log_msg("Failed to hook library: %s.\n", Name);
			exit(-1);
		}
		log_msg("%s successfully hooked.\n", Name);
	}

	static void UnhookLibrary()
	{
		DWORD dw = 0;
		BYTE* offsetPESignature = Offset + *(BYTE**)((BYTE*)Offset + 0x3C);
		DWORD sizeOfCode = *(DWORD*)(offsetPESignature + 0x1C);
		BYTE* baseOfCode = Offset + *(BYTE**)(offsetPESignature + 0x2C);
		if (!VirtualProtect(baseOfCode, sizeOfCode, PAGE_EXECUTE_READ, &dw))
			log_msg("Failed to unhook library: %s.\n", Name);
		else
			log_msg("%s successfully unhooked.\n", Name);
	}

	// Retrieves the address by adding the DLLs base offset to the recorded offset
	static DWORD GetAddress(const DWORD offset)
	{
		DWORD proposedOffset = Offset + offset;
		//log_msg("Address by Addition: (%08X + %08X @ %08X)\n", Offset, offset, proposedOffset);
		return proposedOffset;
	}
};

template <class Child>
char* Library<Child>::Name;

template <class Child>
DWORD Library<Child>::Offset;