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

#include <Windows.h>

extern void* currentMemoryPos;

// The below memory functions should not be used directly. Use the Memory Class.
DWORD mem_seek(DWORD newPos);
void memt_byte(BYTE old, BYTE val);
void memt_dword(DWORD old, DWORD val);
void memt_ref4(DWORD old, DWORD ref);
void memj_ref4(DWORD old, DWORD ref);
void memc_ref4(DWORD old, DWORD ref);
void memd_ref4(DWORD old, DWORD ref);

class Memory
{
public:
	static void SetCursor(DWORD position);
	static void ChangeByte(BYTE oldValue, BYTE newValue);
	static void ChangeDword(DWORD oldValue, DWORD newValue);
	static void ChangeCallA(DWORD oldValue, DWORD newValue);
	static void ChangeCallB(DWORD oldValue, DWORD newValue);
	static void ChangeCallC(DWORD oldValue, DWORD newValue);
	static void ChangeCallD(DWORD oldValue, DWORD newValue);
};
