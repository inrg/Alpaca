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

class Storm : public Library<Storm>
{
public:
	static void Init();

	typedef DWORD(__stdcall *TD2StormMPQOpenFile) (DWORD zero, LPCSTR fileName, DWORD dwSearchScope, void** buffer);
	typedef void(__stdcall *TD2FreeWinMessage) (sWinMessage* msg);

	static TD2StormMPQOpenFile D2StormMPQOpenFile;
	static TD2FreeWinMessage D2FreeWinMessage;
private:
	static void SetFunctions();
};