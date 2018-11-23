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

#include "D2CMP.h"

void D2CMP::Init()
{
	Name = "D2CMP.dll";
	Offset = LoadDiabloLibrary();
	SetFunctions();
}

void D2CMP::SetFunctions()
{
	D2CMP10014 = (TD2CMP10014)GetAddress(0x135B0);
}

D2CMP::TD2CMP10014 D2CMP::D2CMP10014;