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

#include "InfinityStash.h"
#include "UpdateClient.h"
#include "InterfaceStash.h"
#include "Common.h"

#define STASH_TAG 0x5453			//"ST"
#define JM_TAG 0x4D4A 				//"JM"

DWORD maxSelfPages = -1;
DWORD maxSharedPages = -1;
DWORD nbPagesPerIndex = 10;
DWORD nbPagesPerIndex2 = 100;

char* sharedStashFilename = "SharedStashSave";

// Let's keep hardcore and softcore shared stashes separate.
bool separateHardSoftStash = true;

typedef int (*TchangeToSelectedStash)(Unit* ptChar, Stash* newStash, DWORD bOnlyItems, DWORD bIsClient);
TchangeToSelectedStash changeToSelectedStash;

Unit* firstClassicStashItem(Unit* ptChar)
{
	Unit* ptItem = D2InventoryGetFirstItem(PCInventory);
	while (ptItem)
	{
		if (D2ItemGetPage(D2GetRealItem(ptItem)) == 4)
			return ptItem;
		ptItem = D2UnitGetNextItem(ptItem);
	}
	return NULL;
}

DWORD endStashList(Unit* ptChar, Stash* ptStash)
{
	Stash* stash = ptStash;
	
	while (stash)
	{
		if (stash->ptListItem || ((stash == PCPY->currentStash) && firstClassicStashItem(ptChar))) return 0;
		if (stash->isIndex || (stash->name != NULL && stash->name[0])) return 0;
		stash = stash->nextStash;
	}
	return 1;
}

// Gets the first stash for the corresponding stash (Personal or Shared)
// If you pass true to isFlipped, it will return the alternate first stash.
Stash* getCorrespondingFirstStash(Unit* ptChar, bool isFlipped)
{
	if (!isFlipped)
	{
		return PCPY->currentStash->isShared ? PCPY->sharedStash : PCPY->selfStash;
	}
	else
	{
		return PCPY->currentStash->isShared ? PCPY->selfStash : PCPY->sharedStash;
	}
}

Stash* getLastStash(Stash* ptStash)
{
	Stash* stash = ptStash;
	
	if (!stash) return NULL;
	while (stash->nextStash)
		stash = stash->nextStash;
	
	return stash;
}

Stash* newStash(DWORD id)
{
	d2_assert( id == 0xFFFFFFFF , "Too much stash", __FILE__, __LINE__);
	
	Stash* stash = (Stash*)malloc(sizeof(Stash));
	d2_assert(!stash , "Error on stash allocation.", __FILE__, __LINE__);
	ZeroMemory(stash, sizeof(Stash));
	stash->id = id;
	
	return stash;
}

Stash* addStash(Unit* ptChar, bool isShared)
{
	Stash* previous;
	Stash* stash;

	if (isShared)
	{
		previous = getLastStash(PCPY->sharedStash);
		stash = newStash(PCPY->nbSharedPages++);
	} else {
		previous = getLastStash(PCPY->selfStash);
		stash = newStash(PCPY->nbSelfPages++);
	}
	
	stash->isShared = isShared;
	stash->previousStash = previous;
	if (previous)
		previous->nextStash = stash;
	else if (isShared)
		PCPY->sharedStash = stash;
	else
		PCPY->selfStash = stash;
	
	log_msg("AddStash: stash->id=%d\tstash->isShared=%d\tstash->previous=%08X\tnbSelf=%d\tnbShared=%d\n",
		stash->id,stash->isShared,stash->previousStash,PCPY->nbSelfPages,PCPY->nbSharedPages);
	
	return stash;
}

Stash* getStash(Unit* ptChar, DWORD isShared, DWORD id)
{
	Stash* ptStash = isShared ? PCPY->sharedStash : PCPY->selfStash;

	while (ptStash)
	{
		if (ptStash->id == id) return ptStash;
		ptStash = ptStash->nextStash;
	}

	return NULL;
}

int changeToSelectedStash_10(Unit* ptChar, Stash* newStash, DWORD bOnlyItems, DWORD bIsClient)
{
	if (!newStash) return 0;

	log_msg("changeToSelectedStash ID:%d\tshared:%d\tonlyItems:%d\tclient:%d\n", newStash->id, newStash->isShared, bOnlyItems, bIsClient);

	Stash* currentStash = PCPY->currentStash;
	if (currentStash == newStash) return 0;

	d2_assert(currentStash && currentStash->ptListItem, "ERROR : currentStash isn't empty (ptListItem != NULL)", __FILE__, __LINE__);

	// Remove items from current page
	Unit* ptNextItem;
	Unit* ptItem = D2InventoryGetFirstItem(PCInventory);
	while (ptItem)
	{
		ptNextItem = D2UnitGetNextItem(ptItem);
		if (D2ItemGetPage(ptItem) == 4)
		{
			BYTE tmp = ptItem->ptItemData->ItemData2;
			ptItem = D2InvRemoveItem(PCInventory, ptItem);
			ptItem->ptItemData->ItemData2 = tmp;
			if (currentStash)
			{
				ptItem->ptItemData->ptNextItem = currentStash->ptListItem;
				currentStash->ptListItem = ptItem;
			}
		}
		ptItem = ptNextItem;
	}

	// add items of new stash
	ptItem = newStash->ptListItem;
	while (ptItem)
	{
		D2InvAddItem(PCInventory, ptItem, ptItem->path->x, ptItem->path->y, 0xC, bIsClient, 4);
		ptItem = D2UnitGetNextItem(ptItem);
	}
	if (bOnlyItems)
		newStash->ptListItem = PCPY->currentStash->ptListItem;
	else
		PCPY->currentStash = newStash;
	PCPY->currentStash->ptListItem = NULL;

	return 1;
}

DWORD loadStash(Unit* ptChar, Stash* ptStash, BYTE data[], DWORD startSize, DWORD maxSize, DWORD* retSize)
{
	DWORD curSize = startSize;
	DWORD nbBytesRead;

	log_msg("loadStash\n");

	*retSize = curSize;
	if( *(WORD *)&data[curSize] != STASH_TAG )//"ST"
	{
		log_msg("loadStash -> Bad tag of stash of character %s : %04X\n",PCPlayerData->name,*(WORD *)&data[curSize]);
		return 0x7;//Unable to enter game. Bad inventory data
	}
	curSize += 2;

	// Read flags.
	int len = strlen((char*)&data[curSize]);
	if (*(WORD*)&data[curSize + len + 1] != JM_TAG)
	{
		ptStash->flags = *(DWORD *)&data[curSize];
		curSize += 4;
	}

	// Read Name
//	if (strlen((char *)&data[curSize]) > 0xF)
//		*(char *)&data[curSize+0xF] = NULL;
	if (strlen((char *)&data[curSize]))
		ptStash->name = (char*)malloc(strlen((char *)&data[curSize]));//D2AllocMem(PCGame->memoryPool, strlen((char *)&data[curSize]),__FILE__,__LINE__,0);
	if (ptStash->name)
		strcpy(ptStash->name, (char *)&data[curSize]);
	curSize += strlen((char *)&data[curSize]) + 1;

	// Read inventory.
	DWORD ret = D2LoadInventory(PCGame, ptChar, (saveBitField*)&data[curSize], 0x60, maxSize-curSize, 0, &nbBytesRead);
	if (ret) log_msg("loadStash -> D2LoadInventory failed\n");
	log_msg("Stash loaded (%d : %s)\n", ptStash->id ,ptStash->name);

	*retSize=curSize + nbBytesRead;
	return ret;
}

DWORD loadStashList(Unit* ptChar, BYTE* data, DWORD maxSize, DWORD* curSize, bool isShared)
{
	DWORD curStash = 0;
	Stash* newStash;

	DWORD nbStash = *(DWORD*)&data[*curSize];
	*curSize += 4;

	while (curStash < nbStash)
	{
		newStash = addStash(ptChar, isShared);
		changeToSelectedStash(ptChar, newStash, 0, false);
		DWORD ret = loadStash(ptChar, newStash, data, *curSize, 10000000, curSize);
		if (ret) return ret;
		curStash++;
	}

	if (!isShared && !PCPY->selfStash)
	{
		newStash = addStash(ptChar, isShared);
		PCPY->currentStash = newStash;
	}
	
	if (isShared && !PCPY->sharedStash)
	{
		newStash = addStash(ptChar, isShared);
		if (!PCPY->currentStash)
			PCPY->currentStash = newStash;
	}

	return 0;
}

//ADDDATA(DWORD, curSize, 0);

#define DATA (*data + *curSize)
#define ADDDATA(T)		(T*)DATA;		*curSize += sizeof(T)
#define SETDATA(T,V)	*(T*)DATA = V;	*curSize += sizeof(T)

void saveStash(Unit* ptChar, Stash* ptStash, BYTE** data, DWORD* maxSize, DWORD* curSize)
{
	//write "ST"
	SETDATA(WORD, STASH_TAG);

	//Write flags.
	SETDATA(DWORD, ptStash->flags);

	//save name
	if (ptStash->name)
	{
		int size = strlen(ptStash->name);
		if (size > 15) size = 15;
		strncpy((char*)DATA, ptStash->name, size);
		*curSize += size;
	}
	SETDATA(char, NULL);

	//Write "JM" of inventory
	SETDATA(WORD, JM_TAG);

	//Get position of counter of items in inventory
	WORD* ptNBItem = ADDDATA(WORD);
	*ptNBItem = 0;

	//Get first item
	Unit* ptItem;
	if ((ptStash->id == PCPY->currentStash->id) && (ptStash->isShared == PCPY->currentStash->isShared))
		ptItem = D2InventoryGetFirstItem(PCInventory);
	else
		ptItem = ptStash->ptListItem;

	//Write all items
	while (ptItem)
	{
		if (D2ItemGetPage(D2GetRealItem(ptItem)) == 4)
		{
			int nbBytes = D2SaveItem(D2GetRealItem(ptItem), (saveBitField*)DATA, *maxSize - *curSize, 1, 1, 0);
			d2_assert(!nbBytes, "!\"Character has too many items\"", __FILE__, __LINE__ );
			*curSize += nbBytes;
			(*ptNBItem)++;
		}
		ptItem = D2UnitGetNextItem(ptItem);
	}
}


void saveStashList(Unit* ptChar, Stash* ptStash, BYTE** data, DWORD* maxSize, DWORD* curSize)
{
	DWORD curSizeNbStash = *curSize;
	*curSize += sizeof(DWORD);

	DWORD nbStash=0;

	while(ptStash)
	{
		if (*curSize + 0x2000 > *maxSize)
		{
			BYTE* oldData = *data;
			*maxSize *= 2;
			*data = (BYTE *)D2AllocMem(PCGame->memoryPool, *maxSize,__FILE__,__LINE__,0);
			d2_assert(!*data, "Error : Memory allocation", __FILE__, __LINE__);
			CopyMemory(*data, oldData, *curSize);
			D2FreeMem(PCGame->memoryPool, oldData,__FILE__,__LINE__,0);
		}
		saveStash(ptChar, ptStash, data, maxSize, curSize);
		nbStash++;
		ptStash = endStashList(ptChar, ptStash->nextStash) ? NULL : ptStash->nextStash;
	}
	*(DWORD*)(*data + curSizeNbStash) = nbStash;
}

/////// client
void updateSelectedStashClient(Unit* ptChar)
{
	Stash* newStash = PCPY->currentStash;
	if (!newStash)
		return;
	updateClient(ptChar, UC_SELECT_STASH, newStash->id, newStash->flags, PCPY->flags);
	updateClient(ptChar, UC_PAGE_NAME, newStash->name);
}

void setSelectedStashClient(DWORD stashId, DWORD stashFlags, DWORD flags, bool bOnlyItems)
{
	log_msg("setSelectedStashClient ID:%d, stashFlags:%d, flags:%08X\n", stashId, stashFlags, flags);
	Unit* ptChar = D2GetClientPlayer();
	Stash* newStash = getStash(ptChar, (stashFlags & 1) == 1, stashId);
	if (!newStash) do
		newStash = addStash(ptChar, (stashFlags & 1) == 1);
	while (newStash->id < stashId);
	newStash->flags = stashFlags;
	changeToSelectedStash(ptChar, newStash, bOnlyItems, 1);
	PCPY->flags = flags;
}

void selectStash(Unit* ptChar, Stash* newStash, bool isRunningDuringInit)
{
	if (!newStash) return;
	changeToSelectedStash(ptChar, newStash, 0, 0);
	updateSelectedStashClient(ptChar);
	rememberLastSelectedStash(ptChar, newStash, isRunningDuringInit);
}

// Jumps to the target page
void jumpToPage(Unit* ptChar, DWORD targetPageIndex)
{
	bool isShared = PCPY->currentStash->isShared;
	Stash* targetStash = getStash(ptChar, isShared, targetPageIndex);

	if (targetStash == NULL)
	{
		targetStash = createStashesUpToPageIndex(ptChar, getCorrespondingFirstStash(ptChar), targetPageIndex);
	}

	selectStash(ptChar, targetStash);
}

// Creates all the pages up to a page index
Stash* createStashesUpToPageIndex(Unit* ptChar, Stash* currentStash, DWORD targetPageIndex)
{
	for (DWORD currentPageIndex = 0; currentPageIndex < targetPageIndex; currentPageIndex++)
	{
		if (currentStash->nextStash == NULL)
		{
			addStash(ptChar, currentStash->isShared);
		}
		currentStash = currentStash->nextStash;
	}

	return currentStash;
}

///// public functions
void toggleToSelfStash(Unit* ptChar)
{
	Stash* selStash = NULL;
	
	if (PCPY->lastSelectedSelfStash != NULL)
	{
		selStash = PCPY->lastSelectedSelfStash;
	}
	else
	{
		selStash = PCPY->selfStash;
	}

	toggleAndSelectStash(ptChar, selStash, false);
}

void toggleToSharedStash(Unit* ptChar)
{
	Stash* selStash = NULL;

	if (PCPY->lastSelectedSharedStash != NULL)
	{
		selStash = PCPY->lastSelectedSharedStash;
	}
	else
	{
		selStash = PCPY->sharedStash;
	}

	toggleAndSelectStash(ptChar, selStash, true);
}

void toggleAndSelectStash(Unit* ptChar, Stash* selectedStash, bool showSharedStash)
{
	if (selectedStash && (selectedStash != PCPY->currentStash))
	{
		PCPY->showSharedStash = showSharedStash;
		selectStash(ptChar, selectedStash);
	}
}

// Swaps the stash metadata
void swapStashMetadata(Stash* currentStash, Stash* targetStash)
{
	// Name
	char* tempName = currentStash->name;
	currentStash->name = targetStash->name;
	targetStash->name = tempName;

	// Do not switch the "flags" variable since this will cause problems
	// due to the personal/shared stashes. Just flip the individual indexes,
	// and let the system automatically compute the proper flag number.

	// Index/Main Index
	DWORD tempIndex = currentStash->isIndex;
	DWORD tempMainIndex = currentStash->isMainIndex;

	currentStash->isIndex = targetStash->isIndex;
	currentStash->isMainIndex = targetStash->isMainIndex;

	targetStash->isIndex = tempIndex;
	targetStash->isMainIndex = tempMainIndex;
}

void swapStash(Unit* ptChar, Stash* curStash, Stash* swpStash)
{
	if (!ptChar || !curStash || !swpStash || curStash == swpStash)
		return;

	swapStashMetadata(curStash, swpStash);
	changeToSelectedStash(ptChar, swpStash, 1, 0);
	updateClient(ptChar, UC_SELECT_STASH, swpStash->id, swpStash->flags | 8, PCPY->flags);
	updateSelectedStashClient(ptChar);
}

void toggleStash(Unit* ptChar, DWORD page)
{
	log_msg("toggle stash page = %u\n", page);
	Stash* curStash = PCPY->currentStash;
	Stash* swpStash = curStash->isShared ? PCPY->selfStash : PCPY->sharedStash;
	swapStash(ptChar, curStash, swpStash);
}

void swapStash(Unit* ptChar, DWORD targetPageIndex, bool toggle)
{
	log_msg("swap stash page = %u\n", targetPageIndex);

	// Get the current stash
	Stash* curStash = PCPY->currentStash;

	// Get the stash we want to swap to (on the opposite type), the reference starts at id 0 (aka Page 1).
	Stash* swpStash = getCorrespondingFirstStash(ptChar, toggle);

	// Loop through each page until we get up to the page we want to toggle our items into
	// in the opposite stash type, each stash in between needs to be allocated so that our
	// pointer to the next stash can point to the correct location.
	swpStash = createStashesUpToPageIndex(ptChar, swpStash, targetPageIndex);

	// Now that we've arrived at stash page we want to switch our
	// items to in the opposite stash type, go ahead and do the swap.
	swapStash(ptChar, curStash, swpStash);
}

void insertStash(Unit* ptChar)
{
	Stash* curStash = PCPY->currentStash;
	Stash* stash = addStash(ptChar, curStash->isShared);
	while (stash->previousStash != curStash) 
	{
		stash->flags = stash->previousStash->flags;
		stash->name = stash->previousStash->name;
		stash->ptListItem = stash->previousStash->ptListItem;
		stash = stash->previousStash;
	}
	stash->isIndex = 0;
	stash->isMainIndex = 0;
	stash->name = NULL;
	stash->ptListItem = NULL;
}

bool deleteStash(Unit* ptChar, bool isClient)
{
	if (firstClassicStashItem(ptChar) != NULL)
		return false;

	Stash* stash = PCPY->currentStash;
	if (stash->nextStash == NULL)
	{
		stash->isIndex = 0;
		stash->isMainIndex = 0;
		stash->name = NULL;
		return true;
	}
	stash->flags = stash->nextStash->flags;
	stash->name = stash->nextStash->name;
	if (stash->nextStash->ptListItem != NULL)
		changeToSelectedStash(ptChar, stash->nextStash, 1, isClient);
	stash = stash->nextStash;
	while (stash->nextStash)
	{
		stash->flags = stash->nextStash->flags;
		stash->name = stash->nextStash->name;
		stash->ptListItem = stash->nextStash->ptListItem;
		stash = stash->nextStash;
	}
	stash->isIndex = 0;
	stash->isMainIndex = 0;
	stash->name = NULL;
	stash->ptListItem = NULL;
	return true;
}

void renameCurrentStash(Unit* ptChar, char* name)
{
	log_msg("renameCurrentStash : %08X, %s\n", ptChar, name);
	Stash* stash = PCPY->currentStash;
	int len = 0;
	if (name != NULL)
		len = strlen(name);
	log_msg("renameCurrentStash : %d\n", len);
	if (stash->name)
		D2FogMemDeAlloc(stash->name, __FILE__, __LINE__, 0);
	log_msg("renameCurrentStash 3\n");
	if (len > 0)
	{
		stash->name = (char *)malloc(len);
		strcpy(stash->name, name);
	}
	else
		stash->name = NULL;
	log_msg("renameCurrentStash 4\n");
}

void setCurrentStashIndex(Unit* ptChar, int index)
{
	if (!PCPY->currentStash)
		return;
	PCPY->currentStash->isIndex = index >= 1;
	PCPY->currentStash->isMainIndex = index == 2;
	updateSelectedStashClient(ptChar);
}

void selectPreviousStash(Unit* ptChar)
{
	Stash* selStash = PCPY->currentStash->previousStash;
	if (selStash && (selStash != PCPY->currentStash))
	{
		selectStash(ptChar, selStash);
	}
}

void selectPrevious2Stash(Unit* ptChar)// Select first stash
{
	Stash* selStash = PCPY->showSharedStash ? PCPY->sharedStash : PCPY->selfStash;
	if (selStash && (selStash != PCPY->currentStash))
		selectStash(ptChar, selStash);
}

void selectNextStash(Unit* ptChar)
{
	Stash* selStash = PCPY->currentStash;
	if (!selStash->isShared && (selStash->id >= maxSelfPages))	return;
	if (selStash->isShared && (selStash->id >= maxSharedPages)) return;

	selStash = selStash->nextStash ? selStash->nextStash : addStash(ptChar, PCPY->showSharedStash);

	if (selStash && (selStash != PCPY->currentStash))
	{
		selectStash(ptChar, selStash);
	}
}

// Remembers the last selected stash for the particular stash type
// for the duration of the game session.
void rememberLastSelectedStash(Unit* ptChar, Stash* selectedStash, bool isRunningDuringInit)
{
	// It seems the code at the moment while scan all the stashes until it reaches the last stash
	// during game initialization. For this case, we don't want to update it since then the user
	// would see the item in the last page when they open their stash. We want to start on the
	// first pages of both (personal/shared), and only remember the pages within the user's
	// game session.
	if (isRunningDuringInit) return;

	if (selectedStash->isShared)
	{
		PCPY->lastSelectedSharedStash = selectedStash;
	}
	else
	{
		PCPY->lastSelectedSelfStash = selectedStash;
	}
}

void selectNext2Stash(Unit* ptChar)//select last stash
{
	Stash* selStash = PCPY->showSharedStash ? PCPY->sharedStash : PCPY->selfStash;//PCPY->currentStash;
	Stash* lastStash = NULL;
	Unit* currentStashItem = firstClassicStashItem(ptChar);
	while (selStash)
	{
		if (selStash->ptListItem || (selStash == PCPY->currentStash) && currentStashItem) lastStash=selStash;
		selStash = selStash->nextStash;
	}
	if (!lastStash)
		lastStash = PCPY->showSharedStash ? PCPY->sharedStash : PCPY->selfStash;
	if (lastStash != PCPY->currentStash)
		selectStash(ptChar, lastStash);
}

void selectPreviousIndexStash(Unit* ptChar)
{
	selectPreviousStash(ptChar);
	Stash* selStash = PCPY->currentStash;
	while (selStash && !selStash->isIndex)
		selStash = selStash->previousStash;

	if (selStash == NULL)
	{
		selStash = PCPY->currentStash;
		while (selStash->previousStash && ((selStash->id + 1) % nbPagesPerIndex != 0))
			selStash = selStash->previousStash;
	}

	if (selStash && (selStash != PCPY->currentStash))
		selectStash(ptChar, selStash);
}

void selectPreviousIndex2Stash(Unit* ptChar)
{
	selectPreviousStash(ptChar);
	Stash* selStash = PCPY->currentStash;
	while (selStash && !selStash->isMainIndex)
		selStash = selStash->previousStash;

	if (selStash == NULL)
	{
		selStash = PCPY->currentStash;
		while (selStash->previousStash && ((selStash->id+1) % nbPagesPerIndex2 != 0))
			selStash = selStash->previousStash;
	}

	if (selStash && (selStash != PCPY->currentStash))
		selectStash(ptChar, selStash);
}

void selectNextIndexStash(Unit* ptChar)
{
	selectNextStash(ptChar);
	Stash* selStash = PCPY->currentStash;
	while (selStash && !selStash->isIndex)
		selStash = selStash->nextStash;

	if (selStash == NULL)
	{
		selStash = PCPY->currentStash;
		while ((selStash->id + 1) % nbPagesPerIndex != 0)
		{
			if (!selStash->isShared && (selStash->id >= maxSelfPages))	break;
			if (selStash->isShared && (selStash->id >= maxSharedPages)) break;
			selStash = selStash->nextStash ? selStash->nextStash : addStash(ptChar, PCPY->showSharedStash);;
		}
	}
	if (selStash && (selStash != PCPY->currentStash))
		selectStash(ptChar, selStash);
}

void selectNextIndex2Stash(Unit* ptChar)
{
	selectNextStash(ptChar);
	Stash* selStash = PCPY->currentStash;
	while (selStash && !selStash->isMainIndex)
		selStash = selStash->nextStash;

	if (selStash == NULL)
	{
		selStash = PCPY->currentStash;
		while ((selStash->id+1) % nbPagesPerIndex2 != 0)
		{
			if (!selStash->isShared && (selStash->id >= maxSelfPages))	break;
			if (selStash->isShared && (selStash->id >= maxSharedPages)) break;
			selStash = selStash->nextStash ? selStash->nextStash : addStash(ptChar, PCPY->showSharedStash);;
		}
	}
	if (selStash && (selStash != PCPY->currentStash))
		selectStash(ptChar, selStash);
}

//////////////////////////////////////////////////////////////////////
Stash* curStash2;
DWORD currentSawStash2;

Unit* __stdcall getNextItem(Unit* ptChar, Unit* ptItem)
{
	Unit* item = D2UnitGetNextItem(ptItem);
	if (item) return item;

	if (!curStash2)
	{
		switch (currentSawStash2)
		{
		case 0: curStash2 = PCPY->selfStash;
				currentSawStash2 = 1;
				break;
		case 1: curStash2 = PCPY->sharedStash;
				currentSawStash2 = 2;
				break;
		default: return NULL;//case 2:
		}
	} else {
		curStash2 = curStash2->nextStash;
	}
	if (curStash2)
	{
		item = curStash2->ptListItem;
		if (item) return item;
	}
	return getNextItem(ptChar,item);
}

Unit* __stdcall initGetNextItem(Unit* ptChar, Unit* ptItem)
{
	if (ptChar->nUnitType != UNIT_PLAYER) return NULL;
	if (!PCPY) return NULL;

	curStash2 = NULL;
	currentSawStash2 = 0;
	if (ptItem) return ptItem;
	return getNextItem(ptChar,ptItem);
}

DWORD __stdcall carry1Limit(Unit* ptChar, Unit* ptItemParam, DWORD itemNum, BYTE page)
{
	if (!ptChar) return 0;
	Unit* ptItem = ptItemParam ? ptItemParam : D2GameGetObject(PCGame, UNIT_ITEM, itemNum);
	if ((page != 4) && (D2Common::D2GetItemQuality(ptItem) == 7) && ptChar)
	{
		auto dataTables = D2Common::GetDataTables();
		int uniqueID = D2Common::D2GetUniqueID(ptItem);
		if ((uniqueID >= 0) && (uniqueID < (int)dataTables->nbUniqueItems))
		{
			UniqueItemsBIN*	uniqueItems = dataTables->uniqueItems + uniqueID;
			if (uniqueItems && (uniqueItems->carry1 == 1))
			{
				ItemsBIN* ptItemsBin = D2Common::D2GetItemsBIN(ptItem->nTxtFileNo);
				Unit* ptFirstItem = D2InventoryGetFirstItem(PCInventory);
				if (ptItemsBin && ptFirstItem)
					return D2Game::D2VerifIfNotCarry1(ptItem, ptItemsBin, ptFirstItem);
			}
		}
	}
	return 0;
}

FCT_ASM ( caller_initGetNextItem )
	PUSH DWORD PTR SS:[ESP+0x20]
	PUSH DWORD PTR SS:[ESP+0xC]
	CALL initGetNextItem
	MOV EDI,EAX
	TEST EDI,EDI
	RETN
}}


FCT_ASM ( caller_getNextItem )
	PUSH DWORD PTR SS:[ESP+4]
	PUSH DWORD PTR SS:[ESP+0x10]
	CALL getNextItem
	RETN 4
}}

FCT_ASM(caller_carry1Limit_111)
PUSH DWORD PTR SS : [ESP + 0x08]//page
PUSH 0//EDI
PUSH DWORD PTR SS : [ESP + 0x0C]
PUSH ESI//ptChar
CALL carry1Limit
TEST EAX, EAX
JNZ	end_carry1Limit
JMP D2Common::D2ItemSetPage
end_carry1Limit :
ADD ESP, 0xC
XOR EAX, EAX
POP EDI
POP EBX
POP ESI
POP EBP
RETN 8
}}

FCT_ASM(caller_carry1LimitSwap_112)
PUSH EAX
PUSH DWORD PTR SS : [ESP + 0x1C]
PUSH 0
PUSH ESI//ptChar
CALL carry1Limit
TEST EAX, EAX
JNZ	end_carry1Limit
JMP D2ItemGetPage
end_carry1Limit :
ADD ESP, 8
XOR EAX, EAX
POP EDI
POP EBP
POP ESI
POP EBX
POP ECX
RETN 8
}}

FCT_ASM(caller_carry1LimitWhenDrop_111)
	PUSH 0
	PUSH 0
	PUSH DWORD PTR SS : [ESP + 0x10]
	PUSH ESI
	CALL carry1Limit
	TEST EAX, EAX
	JNZ	end_carry1Limit
	JMP D2Common::D2CanPutItemInInv
end_carry1Limit:
	XOR EAX, EAX
	RETN 0x1C
}}

FCT_ASM(caller_carry1OutOfStash_111)
	PUSH EDI
	CALL D2ItemGetPage
	CMP AL, 4
	JNZ continue_carry1OutOfStash
	ADD DWORD PTR SS : [ESP], 0x17C
	RETN
continue_carry1OutOfStash:
	MOV ESI, DWORD PTR SS : [ESP + 0x10]
	TEST ESI, ESI
	RETN
}}

void Install_MultiPageStash()
{
	static bool isInstalled = false;
	if (isInstalled) return;

	Install_PlayerCustomData();
	Install_InterfaceStash();

	log_msg("[Patch] Multi Page Stash\n");

	changeToSelectedStash = changeToSelectedStash_10;

	DWORD PutCarryInventoryOffset = D2Game::GetAddress(0x6B013);
	DWORD PutCarryInventorySwapOffset = D2Game::GetAddress(0x6BC78);
	DWORD DropCarryInventoryDropCubeOffset = D2Game::GetAddress(0xB7B15);
	DWORD VerifyStashPagePickupOffset = D2Game::GetAddress(0xB301B);

	// Cannot put 2 items carry1 in inventory
	Memory::SetCursor(PutCarryInventoryOffset);
	Memory::ChangeCallB((DWORD)D2Common::D2ItemSetPage, (DWORD)caller_carry1Limit_111);

	// Cannot put 2 items carry1 in inventory by swapping
	Memory::SetCursor(PutCarryInventorySwapOffset);
	Memory::ChangeCallB((DWORD)D2Common::D2ItemGetPage, (DWORD)caller_carry1LimitSwap_112);

	// Cannot put 2 items carry1 in inventory when drop cube
	Memory::SetCursor(DropCarryInventoryDropCubeOffset);
	Memory::ChangeCallB((DWORD)D2Common::D2CanPutItemInInv, (DWORD)caller_carry1LimitWhenDrop_111);

	// Verif only carry1 out of stash page when pick up an item
	Memory::SetCursor(VerifyStashPagePickupOffset);
	Memory::ChangeByte(0x8B, 0xE8);
	Memory::ChangeCallA(0x850C2474, (DWORD)caller_carry1OutOfStash_111);
	Memory::ChangeByte(0xF6, 0x90);

	if (active_logFileMemory) log_msg("\n");
	isInstalled = true;
}
