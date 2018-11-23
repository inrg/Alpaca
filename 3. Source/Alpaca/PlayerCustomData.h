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

extern bool active_PlayerCustomData;
extern bool openSharedStashOnLoading;

struct Unit;

struct Stash
{
	DWORD id;
	union { 
		DWORD flags; 
		struct { 
			DWORD isShared : 1;
			DWORD isIndex : 1;
			DWORD isMainIndex : 1;
		};
	};
	char* name;
	Unit* ptListItem;
	Stash* previousStash;
	Stash* nextStash;
};

struct PYPlayerData
{
	union {
		DWORD flags; 
		struct { 
			DWORD selfStashIsOpened : 1;
			DWORD sharedStashIsOpened : 1;
			DWORD showSharedStash : 1;
		}; 
	};
	DWORD	sharedGold;
	DWORD	nbSelfPages;
	DWORD	nbSharedPages;
	Stash*	currentStash;
	Stash*	selfStash;
	Stash*	sharedStash;
	Stash* lastSelectedSelfStash;
	Stash* lastSelectedSharedStash;
};

void Install_PlayerCustomData();