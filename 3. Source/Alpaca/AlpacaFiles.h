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

#include "Common.h"

extern void* stashBtnsImages;
extern void* sharedGoldBtnsImages;

#define CREATE_TABLE_DESCRIPTION( N )	\
BINField  TableDesc[N]; int i=0

// 0 - null data, used by the End (Term) fields only
#define ADD_TERM_FIELD()				\
TableDesc[i].fieldName = "end";			\
TableDesc[i].type = 0x00;				\
TableDesc[i].strLength = 0x00;			\
TableDesc[i].offset = 0x00;				\
TableDesc[i].lookup = 0x00000000; i++

// 1 - String (needs length field)
#define ADD_STRING_FIELD( O,N,L )		\
TableDesc[i].fieldName = N;				\
TableDesc[i].type = 0x01;				\
TableDesc[i].strLength = L;				\
TableDesc[i].offset = O;				\
TableDesc[i].lookup = 0x00000000; i++

// 2 - DWORD field
#define ADD_DWORD_FIELD( O,N )			\
TableDesc[i].fieldName = N;				\
TableDesc[i].type = 0x02;				\
TableDesc[i].strLength = 0x00;			\
TableDesc[i].offset = O;				\
TableDesc[i].lookup = 0x00000000; i++

// 3 - WORD field
#define ADD_WORD_FIELD( O,N )			\
TableDesc[i].fieldName = N;				\
TableDesc[i].type = 0x03;				\
TableDesc[i].strLength = 0x00;			\
TableDesc[i].offset = O;				\
TableDesc[i].lookup = 0x00000000; i++

// 4 - BYTE field
#define ADD_BYTE_FIELD( O,N )			\
TableDesc[i].fieldName = N;				\
TableDesc[i].type = 0x04;				\
TableDesc[i].strLength = 0x00;			\
TableDesc[i].offset = O;				\
TableDesc[i].lookup = 0x00000000; i++

// 6 - UBYTE field
#define ADD_UBYTE_FIELD( O,N )			\
TableDesc[i].fieldName = N;				\
TableDesc[i].type = 0x06;				\
TableDesc[i].strLength = 0x00;			\
TableDesc[i].offset = O;				\
TableDesc[i].lookup = 0x00000000; i++

// 9 - three and four letter codes
#define ADD_STRCODE_FIELD( O,N )		\
TableDesc[i].fieldName = N;				\
TableDesc[i].type = 0x09;				\
TableDesc[i].strLength = 0x00;			\
TableDesc[i].offset = O;				\
TableDesc[i].lookup = 0x00000000; i++

// A,B,D,10,11 - Lookup code
#define ADD_LOOKUP_FIELD( O,N,T,L )		\
TableDesc[i].fieldName = N;				\
TableDesc[i].type = T;					\
TableDesc[i].strLength = 0x00;			\
TableDesc[i].offset = O;				\
TableDesc[i].lookup = L; i++

#define ADD_LOOKUP_WORD( O,N,L )		\
TableDesc[i].fieldName = N;				\
TableDesc[i].type = 0x14;				\
TableDesc[i].strLength = 0x00;			\
TableDesc[i].offset = O;				\
TableDesc[i].lookup = (void*)(L); i++


//D,F:Lookup for BYTE (F : UBYTE ?)
//17: call function
//14,16:Lookup for WORD
//11:Lookup fo DWORD
//1A: flag

#define BUILD_BIN( T,V,N,F ) V = (T*)D2CompileTxtFile(mempool, F, TableDesc, (DWORD*)&nbStatsInterface, sizeof(T));

void Install_AlpacaFiles();
void Install_AlpacaImagesFiles();