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

#include "D2Net.h"

void D2Net::Init()
{
	Name = "D2Net.dll";
	Offset = LoadDiabloLibrary();
	SetFunctions();
}

void D2Net::SetFunctions()
{
	D2SendToServer = (TD2SendToServer)GetAddress(0x6F20);
	D2SendToClient = (TD2SendToClient)GetAddress(0x7470);
}

D2Net::TD2SendToServer D2Net::D2SendToServer;
D2Net::TD2SendToClient D2Net::D2SendToClient;