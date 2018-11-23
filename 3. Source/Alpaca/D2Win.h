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

class D2Win : public Library<D2Win>
{
public:
	static void Init();

	typedef void(__fastcall *TD2PrintLineOnTextBox) (void* screen, char* s, DWORD color);
	typedef void(__fastcall *TD2PrintString) (LPWSTR s, DWORD x, DWORD y, DWORD color, DWORD bfalse);
	typedef DWORD(__fastcall *TD2GetPixelLen) (LPWSTR s);
	typedef DWORD(__fastcall *TD2SetFont) (DWORD fontID);
	typedef void(__fastcall *TD2PrintPopup) (LPWSTR s, DWORD x, DWORD y, DWORD color, DWORD center);
	typedef void* (__stdcall *TD2CreateTextBox) (DWORD* data);

	static TD2PrintLineOnTextBox D2PrintLineOnTextBox;
	static TD2PrintString D2PrintString;
	static TD2GetPixelLen D2GetPixelLen;
	static TD2SetFont D2SetFont;
	static TD2PrintPopup D2PrintPopup;
	static TD2CreateTextBox D2CreateTextBox;
private:
	static void SetFunctions();
};