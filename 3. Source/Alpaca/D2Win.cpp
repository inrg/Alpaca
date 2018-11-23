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

#include "D2Win.h"

void D2Win::Init()
{
	Name = "D2Win.dll";
	Offset = LoadDiabloLibrary();
	SetFunctions();
}

void D2Win::SetFunctions()
{
	D2PrintLineOnTextBox = (TD2PrintLineOnTextBox)GetAddress(0x17320);
	D2PrintString = (TD2PrintString)GetAddress(0x13B30);
	D2GetPixelLen = (TD2GetPixelLen)GetAddress(0x136B0);
	D2SetFont = (TD2SetFont)GetAddress(0x13B70);
	D2PrintPopup = (TD2PrintPopup)GetAddress(0x12480);
	D2CreateTextBox = (TD2CreateTextBox)GetAddress(0xE8B0);
}

D2Win::TD2PrintLineOnTextBox D2Win::D2PrintLineOnTextBox;
D2Win::TD2PrintString D2Win::D2PrintString;
D2Win::TD2GetPixelLen D2Win::D2GetPixelLen;
D2Win::TD2SetFont D2Win::D2SetFont;
D2Win::TD2PrintPopup D2Win::D2PrintPopup;
D2Win::TD2CreateTextBox D2Win::D2CreateTextBox;