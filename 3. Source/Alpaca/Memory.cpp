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

#include "Error.h"
#include "Parameters.h"
#include "Memory.h"

const char* MSG_ERROR_READ_MEMORY = "Error: Read access missing to patch memory at %08X.\n\nPlease install a clean version of Diablo II.\n\n";
const char* MSG_CRITICAL_WRITE_MEMORY = "Error: Write access missing to patch memory at %08X.\n\nPlease install a clean version of Diablo II.\n\n";
const char* ERROR_TESTING1 = "Error: BYTE %02X wanted but %02X found to change memory at %08X\n\n";
const char* ERROR_TESTING4 = "Error: DWORD %08X wanted but %08X found to change memory at %08X\n\n";

void* currentMemoryPos = 0;

DWORD mem_seek(DWORD newPos)
{
	currentMemoryPos = (void*)newPos;
	return (DWORD)currentMemoryPos;
}

void patchMemory1(BYTE value)
{
	if (IsBadWritePtr(currentMemoryPos,1))
	{
		log_box(MSG_CRITICAL_WRITE_MEMORY, currentMemoryPos);
		exit(1);
	}
	*(BYTE*)currentMemoryPos = value;
	currentMemoryPos = (LPVOID)((DWORD)currentMemoryPos + 1);
}

void patchMemory4(DWORD value)
{
	if (IsBadWritePtr(currentMemoryPos,4))
	{
		log_box(MSG_CRITICAL_WRITE_MEMORY, currentMemoryPos);
		exit(1);
	}
	*(DWORD*)currentMemoryPos = value;
	currentMemoryPos = (LPVOID)((DWORD)currentMemoryPos + 4);
}

BYTE getMemory1(LPVOID mempos)
{
	if (IsBadReadPtr(mempos,1))
	{
		log_box(MSG_ERROR_READ_MEMORY, currentMemoryPos);
		exit(1);
	}
	return *(BYTE*)mempos;
}

DWORD getMemory4(LPVOID mempos)
{
	if (IsBadReadPtr(mempos,4))
	{
		log_box(MSG_ERROR_READ_MEMORY, currentMemoryPos);
		exit(1);
	}
	return *(DWORD*)mempos;
}

void memt_byte(BYTE old, BYTE val)
{
	BYTE current = getMemory1(currentMemoryPos);
	if ( current == val)
	{
		if (active_logFileMemory)
		{
			log_msg("Warning: BYTE %02X is already set at memory %08X\n", val, currentMemoryPos);
		}
		
		currentMemoryPos = (LPVOID)((DWORD)currentMemoryPos + 1);
		return;
	}
	if ( old != current)
	{
		if (active_CheckMemory)
		{
			log_box(ERROR_TESTING1, old, current, currentMemoryPos);
			exit(1);
		} else log_msg(ERROR_TESTING1, old, current, currentMemoryPos);
	}
	if (active_logFileMemory)
	{
		log_msg("BYTE\t%08X : %02X->%02X\n", currentMemoryPos, old, val);
	}
	patchMemory1(val);
}

bool testIfAlreadySet(DWORD current, DWORD wanted)
{
	if (current == wanted)
	{
		if (active_logFileMemory)
		{
			log_msg("Warning: DWORD %08X is already set at memory %08X\n", wanted, currentMemoryPos);
		}
		currentMemoryPos = (LPVOID)((DWORD)currentMemoryPos + 4);
		return true;
	}
	return false;
}

void testMemory4(DWORD old, DWORD found)
{
	if (old != found)
		if (active_CheckMemory)
		{
			log_box(ERROR_TESTING4, old, found, currentMemoryPos);
			exit(1);
		} else log_msg(ERROR_TESTING4, old, found, currentMemoryPos);
}

void memt_dword(DWORD old, DWORD val)
{
	DWORD current = getMemory4(currentMemoryPos);
	DWORD wanted = val;
	if (testIfAlreadySet(current, wanted)) return;
	testMemory4(old, current);
	if (active_logFileMemory)
	{
		log_msg("DWORD\t%08X : %08X->%08X\n", currentMemoryPos, old, wanted);
	}
	patchMemory4(wanted);
}

void memt_ref4(DWORD old, DWORD ref)
{
	DWORD current = getMemory4(currentMemoryPos);
	DWORD wanted = ref-(DWORD)currentMemoryPos-4;
	if (testIfAlreadySet(current, wanted)) return;
	testMemory4(old, current);
	if (active_logFileMemory)
	{
		log_msg("DWORD\t%08X : %08X->%08X\n", currentMemoryPos, old, wanted);
	}
	patchMemory4(wanted);
}

void memc_ref4(DWORD old, DWORD ref)
{
	DWORD current = getMemory4(currentMemoryPos);
	DWORD wanted = ref-(DWORD)currentMemoryPos-4;
	if (testIfAlreadySet(current, wanted)) return;
	testMemory4(old, current + (DWORD)currentMemoryPos + 4);
	if (active_logFileMemory)
	{
		log_msg("DWORD\t%08X : %08X->%08X\n", currentMemoryPos, old, wanted);
	}
	patchMemory4(wanted);
}

void memj_ref4(DWORD old, DWORD ref)
{
	DWORD current = getMemory4(currentMemoryPos);
	DWORD wanted = ref-(DWORD)currentMemoryPos-4;
	if (testIfAlreadySet(current, wanted)) return;
	testMemory4(old, getMemory4((LPVOID)getMemory4((LPVOID)(current + (DWORD)currentMemoryPos + 6))));
	if (active_logFileMemory)
	{
		log_msg("DWORD\t%08X : %08X->%08X\n", currentMemoryPos, old, wanted);
	}
	patchMemory4(wanted);
}

void memd_ref4(DWORD old, DWORD ref)
{
	DWORD current = getMemory4(currentMemoryPos);
	DWORD wanted = ref-(DWORD)currentMemoryPos-4;
	if (testIfAlreadySet(current, wanted)) return;
	testMemory4(old, getMemory4((LPVOID)current));
	if (active_logFileMemory)
	{
		log_msg("DWORD\t%08X : %08X->%08X\n", currentMemoryPos, old, wanted);
	}
	patchMemory4(wanted);
}

// Transition Memory Functions
void Memory::SetCursor(DWORD position)
{
	mem_seek(position);
}

void Memory::ChangeByte(BYTE oldValue, BYTE newValue)
{
	memt_byte(oldValue, newValue);
}

void Memory::ChangeDword(DWORD oldValue, DWORD newValue)
{
	memt_dword(oldValue, newValue);
}

void Memory::ChangeCallA(DWORD oldValue, DWORD newValue)
{
	memt_ref4(oldValue, newValue);
}

void Memory::ChangeCallB(DWORD oldValue, DWORD newValue)
{
	memj_ref4(oldValue, newValue);
}

void Memory::ChangeCallC(DWORD oldValue, DWORD newValue)
{
	memc_ref4(oldValue, newValue);
}

void Memory::ChangeCallD(DWORD oldValue, DWORD newValue)
{
	memd_ref4(oldValue, newValue);
}
