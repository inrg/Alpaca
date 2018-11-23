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

extern DWORD maxSelfPages;
extern DWORD maxSharedPages;
extern DWORD nbPagesPerIndex;
extern DWORD nbPagesPerIndex2;
extern bool separateHardSoftStash;
extern char* sharedStashFilename;
extern bool displaySharedSetItemNameInGreen;
extern const int posXPreviousBtn;
extern const int posYPreviousBtn;
extern const int posXNextBtn;
extern const int posYNextBtn;
extern const int posXSharedBtn;
extern const int posYSharedBtn;
extern const int posXPreviousIndexBtn;
extern const int posYPreviousIndexBtn;
extern const int posXNextIndexBtn;
extern const int posYNextIndexBtn;
extern const int posXPutGoldBtn;
extern const int posYPutGoldBtn;
extern const int posXTakeGoldBtn;
extern const int posYTakeGoldBtn;

void toggleToSharedStash(Unit* ptChar);
void toggleToSelfStash(Unit* ptChar);
void toggleAndSelectStash(Unit* ptChar, Stash* selectedStash, bool showSharedStash);

void setCurrentStashIndex(Unit* ptChar, int index);
void selectPreviousStash(Unit* ptChar);
void selectNextStash(Unit* ptChar);
void selectPreviousIndexStash(Unit* ptChar);
void selectNextIndexStash(Unit* ptChar);
void selectPrevious2Stash(Unit* ptChar);
void selectNext2Stash(Unit* ptChar);
void selectPreviousIndex2Stash(Unit* ptChar);
void selectNextIndex2Stash(Unit* ptChar);

void swapStash(Unit* ptChar, DWORD targetPageIndex, bool toggle);
void renameCurrentStash(Unit* ptChar, char* name);
void insertStash(Unit* ptChar);
bool deleteStash(Unit* ptChar, bool isClient);
void jumpToPage(Unit* ptChar, DWORD targetPageIndex);
void rememberLastSelectedStash(Unit* ptChar, Stash* selectedStash, bool isRunningDuringInit);
void selectStash(Unit* ptChar, Stash* newStash, bool isRunningDuringInit = false);
void setSelectedStashClient(DWORD stashId, DWORD stashFlags, DWORD flags, bool bOnlyItems);

Stash* addStash(Unit* ptChar, bool isShared);
DWORD loadStashList(Unit* ptChar, BYTE* data, DWORD maxSize, DWORD* curSize, bool isShared);
void saveStashList(Unit* ptChar, Stash* ptStash, BYTE** data, DWORD* maxSize, DWORD* curSize);

Stash* getCorrespondingFirstStash(Unit* ptChar, bool isFlipped = false);
Stash* createStashesUpToPageIndex(Unit* ptChar, Stash* currentStash, DWORD targetPageIndex);

void Install_MultiPageStash();
