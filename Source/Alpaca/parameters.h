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

#define PROGRAM_NAME "Alpaca"
#define PROGRAM_VERSION "1.3.2"
#define PROGRAM_AUTHOR_NAME "Jonathan Vasquez"
#define PROGRAM_AUTHOR_EMAIL "jon@xyinn.org"
#define PROGRAM_BUILD_DATE "October 7, 2018 @ 19:52 ET"

enum TargetMod
{
	MOD_NO = 0,
};

// Global variable (user parameters)
extern char* modDataDirectory;
extern bool active_plugin;
extern bool active_CheckMemory;
extern char* dllFilenames;
extern bool active_D2Mod;
extern TargetMod selectModParam;

void LoadParameters();