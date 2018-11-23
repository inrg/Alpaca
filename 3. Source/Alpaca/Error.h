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

extern bool active_logFile;

// This value is just used in order to decide whether or
// not we will keep logging enable since logging will be
// stopped if we log and then read the ini file with a disabled
// logging value.
extern bool active_logFileIniOriginal;

// Developers: If you want to print out memory writes in the log when doing patching,
// you can enable this option.
extern bool active_logFileMemory;

void log_initfile();
void log_box(const char* pFormat, ...);
void log_msg(const char* pFormat, ...);
void log_patch(const char* message);
void d2_assert(bool pCondition, char* pLocation, char* pMessage, int pLineNbr);