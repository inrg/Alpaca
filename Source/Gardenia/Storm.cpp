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

#include "Storm.h"

void Storm::Init()
{
	Name = "Storm.dll";
	Offset = LoadDiabloLibrary();
	SetFunctions();
}

void Storm::SetFunctions()
{
	D2StormMPQOpenFile = (TD2StormMPQOpenFile)GetOffsetByProc(268);
	D2Storm503 = (TD2Storm503)GetOffsetByProc(503);
	D2FreeWinMessage = (TD2FreeWinMessage)GetOffsetByProc(511);
}

Storm::TD2StormMPQOpenFile Storm::D2StormMPQOpenFile;
Storm::TD2Storm503 Storm::D2Storm503;
Storm::TD2FreeWinMessage Storm::D2FreeWinMessage;