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

#include <windows.h>

#pragma pack(1)

struct TblHeader
{
	WORD		usCRC;					// +00
	WORD		NumElementsOffset;		// +02
	DWORD		HashTableSizeOffset;	// +04
	BYTE		VersionOffset;			// +08
	DWORD		IndexStart;				// +09
	DWORD		NumLoopsOffset;			// +0D
	DWORD		IndexEnd;				// +11 (FileLen)
};

struct TblNode
{
	BYTE		ActiveOffset;			// +00
	WORD		KeyIndex;				// +01
	DWORD		HashValueOffset;		// +03
	DWORD		KeyOffset;				// +07
	DWORD		StringOffset;			// +0B
	WORD		StringLength;			// +0F
};




#pragma pack()

//#define TBLHEADERSIZE	0x15
//#define TBLNODESIZE		0x11

