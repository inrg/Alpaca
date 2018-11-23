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

// NOTE: ** TRY AND REDUCE USAGE OF THIS FILE. **
#pragma once

#include <windows.h>

#include "D2Constants.h"
#include "D2BinFile.h"
#include "D2Struct.h"
#include "D2StringTblStruct.h"
#include "D2UnitStruct.h"
#include "UpdatingConst.h"
#include "Error.h"
#include "Memory.h"
#include "Parameters.h"
#include "PlayerCustomData.h"
#include "LibraryLoader.h"
#include "VersionUtility.h"
#include "D2Functions.h"

bool isOnRect(DWORD x, DWORD y, DWORD x0, DWORD y0, DWORD l, DWORD h);
void freeMessage(sWinMessage* msg);