// Copyright (C) 2004-2017 Yohann Nicolas
// Copyright (C) 2017 L'Autour
// Copyright (C) 2017 Jonathan Vasquez <jon@xyinn.org>
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

class D2gfxLibrary : public Library
{
public:
	D2gfxLibrary(int gameVersion) : Library()
	{
		Name = "D2gfx.dll";
		Version = gameVersion;
		Offset = LoadDiabloLibrary();
		SetFunctions();
	};

	typedef DWORD(__stdcall *TD2GetResolution) ();
	typedef void(__stdcall *TD2FillArea) (DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD color, DWORD transTbl);
	typedef void(__stdcall *TD2PrintImage) (sDrawImageInfo* data, DWORD x, DWORD y, DWORD p4, DWORD p5, DWORD p6);

	TD2GetResolution D2GetResolution;
	TD2FillArea D2FillArea;
	TD2PrintImage D2PrintImage;
private:
	void SetFunctions();
};