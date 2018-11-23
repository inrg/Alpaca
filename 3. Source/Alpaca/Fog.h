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

#include "Library.h"

class Fog : public Library<Fog>
{
public:
	static void Init();

	typedef void(__fastcall *TD2FogAssert) (const char* ptMessage, DWORD eip, DWORD line);
	typedef void* (__fastcall *TD2FogMemAlloc) (DWORD dwMemSize, LPCSTR lpszErrFile, DWORD ErrLine, DWORD Zero);
	typedef void* (__fastcall *TD2FogMemDeAlloc) (void* ptMemLoc, LPCSTR lpszErrFile, DWORD ErrLine, DWORD Zero);
	typedef void* (__fastcall *TD2AllocMem) (DWORD, DWORD dwMemSize, LPCSTR lpszErrFile, DWORD ErrLine, DWORD Zero);
	typedef void* (__fastcall *TD2FreeMem) (DWORD, void* ptMemLoc, LPCSTR lpszErrFile, DWORD ErrLine, DWORD Zero);
	typedef DWORD(__fastcall *TD2MPQOpenFile) (char* ptPath, void** buf);
	typedef DWORD(__fastcall *TD2MPQCloseFile) (void* mpqfile);
	typedef DWORD(__fastcall *TD2MPQReadFile) (void* mpqfile, BYTE* buffer, DWORD nbToRead, DWORD* nbRead, DWORD, DWORD, DWORD);
	typedef DWORD(__fastcall *TD2MPQGetSizeFile) (void* mpqfile, DWORD* toReset);
	typedef void(__fastcall *TD2FogGetSavePath) (char* ptPath, DWORD maxsize);
	typedef int(__stdcall *TD2GetInstructionPointer) ();

	static TD2FogAssert D2FogAssert;
	static TD2FogMemAlloc D2FogMemAlloc;
	static TD2FogMemDeAlloc D2FogMemDeAlloc;
	static TD2AllocMem D2AllocMem;
	static TD2FreeMem D2FreeMem;
	static TD2MPQOpenFile D2MPQOpenFile;
	static TD2MPQCloseFile D2MPQCloseFile;
	static TD2MPQReadFile D2MPQReadFile;
	static TD2MPQGetSizeFile D2MPQGetSizeFile;
	static TD2FogGetSavePath D2FogGetSavePath;
	static TD2GetInstructionPointer D2GetInstructionPointer;
private:
	static void SetFunctions();
};