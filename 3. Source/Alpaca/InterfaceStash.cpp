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

#include "InterfaceStash.h"
#include "UpdateServer.h"
#include "InfinityStash.h"
#include "AlpacaFiles.h"	
#include "Common.h"
#include <stdio.h>
#include "Memory.h"

static struct
{
	union{
		DWORD all;
		struct{
			DWORD previous:1;
			DWORD next:1;
			DWORD toggleToSharedStash:1;
			DWORD previousIndex:1;
			DWORD nextIndex:1;
			DWORD putGold:1;
			DWORD takeGold:1;
		};
	};
} isDownBtn;

// Display Set Item Names In Green
// This is so that if you have a set item in your personal stash
// or shared stash, the game will be able to see your set items
// (and show you that you have the set item), anywhere in any of
// of your stashes.
bool displaySharedSetItemNameInGreen = true;

const int defaultButtonPositionValue = -1;

const int posXPreviousBtn = defaultButtonPositionValue;
const int posYPreviousBtn = defaultButtonPositionValue;
const int posXNextBtn = defaultButtonPositionValue;
const int posYNextBtn = defaultButtonPositionValue;
const int posXSharedBtn = defaultButtonPositionValue;
const int posYSharedBtn = defaultButtonPositionValue;
const int posXPreviousIndexBtn = defaultButtonPositionValue;
const int posYPreviousIndexBtn = defaultButtonPositionValue;
const int posXNextIndexBtn = defaultButtonPositionValue;
const int posYNextIndexBtn = defaultButtonPositionValue;
const int posXPutGoldBtn = defaultButtonPositionValue;
const int posYPutGoldBtn = defaultButtonPositionValue;
const int posXTakeGoldBtn = defaultButtonPositionValue;
const int posYTakeGoldBtn = defaultButtonPositionValue;

// Localization Strings (English Only)
WCHAR* STR_STASH_PREVIOUS_PAGE = L"Previous Page (+Shift: First Page)";
WCHAR* STR_STASH_NEXT_PAGE = L"Next Page (+Shift: Last not empty Page)";
WCHAR* STR_TOGGLE_TO_PERSONAL = L"Switch to Personal Stash";
WCHAR* STR_TOGGLE_TO_SHARED = L"Switch to Shared Stash";
WCHAR* STR_STASH_PREVIOUS_INDEX = L"Previous Index : By %d Pages (+Shift: %d)";
WCHAR* STR_STASH_NEXT_INDEX = L"Next Index : By %d Pages (+Shift: %d)";
WCHAR* STR_DEPOSIT_GOLD = L"Deposit";
WCHAR* STR_WITHDRAW_GOLD = L"Withdraw";
WCHAR* STR_SHARED_PAGE_NUMBER = L"Shared Page [%u]";
WCHAR* STR_PERSONAL_PAGE_NUMBER = L"Personal Page [%u]";
WCHAR* STR_SHARED_GOLD_QUANTITY = L"Shared Gold";
WCHAR* STR_NO_SELECTED_PAGE = L"No page selected";
WCHAR* STR_STASH_LAN_DISABLED = L"Disabled in Multiplayer";

DWORD	getXPreviousBtn()		{return RX(posXPreviousBtn<0? D2GetResolution()?0x80:0xAF : posXPreviousBtn);}
#define	getLPreviousBtn()		32
DWORD	getYPreviousBtn()		{return RY(posYPreviousBtn<0 ? 0x40 : posYPreviousBtn);}
#define	getHPreviousBtn()		32

DWORD	getXNextBtn()			{return RX(posXNextBtn<0 ? D2GetResolution()?0xA0:0xCF :posXNextBtn);}
#define	getLNextBtn()			32
DWORD	getYNextBtn()			{return RY(posYNextBtn<0 ? 0x40 : posYNextBtn);}
#define	getHNextBtn()			32

DWORD	getXSharedBtn()			{return RX(posXSharedBtn<0 ? D2GetResolution()?0x10:0x6F :posXSharedBtn);}
#define	getLSharedBtn()			32
DWORD	getYSharedBtn()			{return RY(posYSharedBtn<0 ? 0x40 : posYSharedBtn);}
#define	getHSharedBtn()			32

DWORD	getXPreviousIndexBtn()	{return RX(posXPreviousIndexBtn<0 ?  D2GetResolution()?0x60:0x8F :posXPreviousIndexBtn);}
#define	getLPreviousIndexBtn()	32
DWORD	getYPreviousIndexBtn()	{return RY(posYPreviousIndexBtn<0 ? 0x40 : posYPreviousIndexBtn);}
#define	getHPreviousIndexBtn()	32

DWORD	getXNextIndexBtn()		{return RX(posXNextIndexBtn<0? D2GetResolution()?0xC0:0xEF : posXNextIndexBtn);}
#define	getLNextIndexBtn()		32
DWORD	getYNextIndexBtn()		{return RY(posYNextIndexBtn<0 ? 0x40 : posYNextIndexBtn);}
#define	getHNextIndexBtn()		32

DWORD	getXPutGoldBtn()		{return RX(posXPutGoldBtn<0? 0x1C : posXPutGoldBtn);}
#define	getLPutGoldBtn()		32
DWORD	getYPutGoldBtn()		{return RY(posYPutGoldBtn<0 ? 0x1A8 : posYPutGoldBtn);}
#define	getHPutGoldBtn()		32

DWORD	getXTakeGoldBtn()		{return RX(posXTakeGoldBtn<0? 0x105 : posXTakeGoldBtn);}
#define	getLTakeGoldBtn()		32
DWORD	getYTakeGoldBtn()		{return RY(posYTakeGoldBtn<0 ? 0x1A8 : posYTakeGoldBtn);}
#define	getHTakeGoldBtn()		32

#define isOnButtonNextStash(x,y) isOnRect(x, y, getXNextBtn(), getYNextBtn(), getLNextBtn(), getHNextBtn())
#define isOnButtonPreviousStash(x,y) isOnRect(x, y, getXPreviousBtn(), getYPreviousBtn(), getLPreviousBtn(), getHPreviousBtn())
#define isOnButtonToggleSharedStash(x,y) isOnRect(x, y, getXSharedBtn(), getYSharedBtn(), getLSharedBtn(), getHSharedBtn())
#define isOnButtonNextIndexStash(x,y) isOnRect(x, y, getXNextIndexBtn(), getYNextIndexBtn(), getLNextIndexBtn(), getHNextIndexBtn())
#define isOnButtonPreviousIndexStash(x,y) isOnRect(x, y, getXPreviousIndexBtn(), getYPreviousIndexBtn(), getLPreviousIndexBtn(), getHPreviousIndexBtn())
#define isOnButtonPutGold(x,y) isOnRect(x, y, getXPutGoldBtn(), getYPutGoldBtn(), getLPutGoldBtn(), getHPutGoldBtn())
#define isOnButtonTakeGold(x,y) isOnRect(x, y, getXTakeGoldBtn(), getYTakeGoldBtn(), getLTakeGoldBtn(), getHTakeGoldBtn())

// Checks to see if the user is in a multiplayer game. This is a hacky way to do
// it but it's pretty reliable and a simple implementation. Our "sharedStashIsOpened"
// value only gets set to true when a single player game loads. Thus by checking this
// boolean, we can determine whether or not we should disable sharing features in the stash.
// Stash sharing features are disabled in order to prevent data loss when two characters
// on the same machine join a MP game (and thus have access to the same shared stash object
// loaded in memory). There are implementations that people could do in order to still allow
// sharing in LAN games, but it would require a bit work and I would rather not have them.
bool inMultiplayerGame(Unit* character)
{
	Unit* ptChar = character ? character : D2GetClientPlayer();
	return !PCPY->sharedStashIsOpened;
}

void* __stdcall printBtns()
{
	if (!D2Client::IsExpansion()) return D2LoadBuySelBtn();

	Unit* ptChar = D2GetClientPlayer();

	sDrawImageInfo data;
	ZeroMemory(&data, sizeof(data));
	setImage(&data, stashBtnsImages);

	setFrame(&data, 0 + isDownBtn.previous);
	D2PrintImage(&data, getXPreviousBtn(), getYPreviousBtn(), -1, 5, 0);

	setFrame(&data, 2 + isDownBtn.next);
	D2PrintImage(&data, getXNextBtn(), getYNextBtn(), -1, 5, 0);

	if (!inMultiplayerGame())
	{
		setFrame(&data, 4 + isDownBtn.toggleToSharedStash + (PCPY->showSharedStash ? 2 : 0));
		D2PrintImage(&data, getXSharedBtn(), getYSharedBtn(), -1, 5, 0);
	}

	setFrame(&data, 8 + isDownBtn.previousIndex);
	D2PrintImage(&data, getXPreviousIndexBtn(), getYPreviousIndexBtn(), -1, 5, 0);

	setFrame(&data, 10 + isDownBtn.nextIndex);
	D2PrintImage(&data, getXNextIndexBtn(), getYNextIndexBtn(), -1, 5, 0);

	if (!inMultiplayerGame())
	{
		setImage(&data, sharedGoldBtnsImages);
		setFrame(&data, 0 + isDownBtn.putGold);
		D2PrintImage(&data, getXPutGoldBtn(), getYPutGoldBtn(), -1, 5, 0);

		setFrame(&data, 2 + isDownBtn.takeGold);
		D2PrintImage(&data, getXTakeGoldBtn(), getYTakeGoldBtn(), -1, 5, 0);
	}
		
	LPWSTR lpText;
	WCHAR text[100];
	DWORD mx = D2GetMouseX();
	DWORD my = D2GetMouseY();

	D2SetFont(1);

	if (isOnButtonPreviousStash(mx, my))
	{
		lpText = STR_STASH_PREVIOUS_PAGE;
		D2PrintPopup(lpText, getXPreviousBtn() + getLPreviousBtn() / 2, getYPreviousBtn() - getHPreviousBtn(), WHITE, 1);
	}
	else if (isOnButtonNextStash(mx, my))
	{
		lpText = STR_STASH_NEXT_PAGE;
		D2PrintPopup(lpText, getXNextBtn() + getLNextBtn() / 2, getYNextBtn() - getHNextBtn(), WHITE, 1);
	}
	else if (isOnButtonToggleSharedStash(mx, my))
	{
		if (!inMultiplayerGame())
		{
			lpText = PCPY->showSharedStash ? STR_TOGGLE_TO_PERSONAL : STR_TOGGLE_TO_SHARED;
		}
		else
		{
			lpText = STR_STASH_LAN_DISABLED;
		}
		D2PrintPopup(lpText, getXSharedBtn() + getLSharedBtn() / 2, getYSharedBtn() - getHSharedBtn(), WHITE, 1);
	}
	else if (isOnButtonPreviousIndexStash(mx, my))
	{
		_snwprintf(text, sizeof(text), STR_STASH_PREVIOUS_INDEX, nbPagesPerIndex, nbPagesPerIndex2);
		D2PrintPopup(text, getXPreviousIndexBtn() + getLPreviousIndexBtn() / 2, getYPreviousIndexBtn() - getHPreviousIndexBtn(), 0, 0);
	}
	else if (isOnButtonNextIndexStash(mx, my))
	{
		_snwprintf(text, sizeof(text), STR_STASH_NEXT_INDEX, nbPagesPerIndex, nbPagesPerIndex2);
		D2PrintPopup(text, getXNextIndexBtn() + getLNextIndexBtn() / 2, getYNextIndexBtn() - getHNextIndexBtn(), WHITE, 1);
	}
	else if (isOnButtonPutGold(mx, my))
	{
		if (!inMultiplayerGame())
		{
			lpText = STR_DEPOSIT_GOLD;
			D2PrintPopup(lpText, getXPutGoldBtn() + getLPutGoldBtn() / 2, getYPutGoldBtn() - getHPutGoldBtn(), WHITE, 1);
		}
	}
	else if (isOnButtonTakeGold(mx, my))
	{
		if (!inMultiplayerGame())
		{
			lpText = STR_WITHDRAW_GOLD;
			D2PrintPopup(lpText, getXTakeGoldBtn() + getLTakeGoldBtn() / 2, getYTakeGoldBtn() - getHTakeGoldBtn(), WHITE, 1);
		}
	}

	return D2LoadBuySelBtn();
}

DWORD __stdcall manageBtnDown(sWinMessage* msg)
{
	if (!D2Client::IsExpansion()) return 0;

	bool shouldBlockSoundIfOnLan = false;

	if (isOnButtonPreviousStash(msg->x,msg->y))
		isDownBtn.previous = 1;
	else if (isOnButtonNextStash(msg->x,msg->y))
		isDownBtn.next = 1;
	else if (isOnButtonToggleSharedStash(msg->x, msg->y))
	{
		if (!inMultiplayerGame())
		{
			isDownBtn.toggleToSharedStash = 1;
		}
		else
		{
			shouldBlockSoundIfOnLan = true;
		}
	}
	else if (isOnButtonPreviousIndexStash(msg->x,msg->y))
		isDownBtn.previousIndex = 1;
	else if (isOnButtonNextIndexStash(msg->x,msg->y))
		isDownBtn.nextIndex = 1;
	else if (isOnButtonPutGold(msg->x, msg->y))
	{
		if (!inMultiplayerGame())
		{
			isDownBtn.putGold = 1;
		}
		else
		{
			shouldBlockSoundIfOnLan = true;
		}
	}
	else if (isOnButtonTakeGold(msg->x, msg->y))
	{
		if (!inMultiplayerGame())
		{
			isDownBtn.takeGold = 1;
		}
		else
		{
			shouldBlockSoundIfOnLan = true;
		}
	}
	else return 0;

	if (!shouldBlockSoundIfOnLan)
	{
		D2PlaySound(4, 0, 0, 0, 0);
	}

	freeMessage(msg);
	return 1;
}

DWORD __stdcall manageBtnUp(sWinMessage* msg)
{
	if (!D2Client::IsExpansion()) return 0;

	Unit* ptChar = D2GetClientPlayer();

	if (isOnButtonPreviousStash(msg->x,msg->y))
	{
		log_msg("push up left button previous\n");
		if (isDownBtn.previous)
			if (GetKeyState(VK_SHIFT)<0)
				updateServer(US_SELECT_PREVIOUS2);
			else
				updateServer(US_SELECT_PREVIOUS);
	}
	else if (isOnButtonNextStash(msg->x,msg->y))
	{
		log_msg("push up left button next\n");
		if (isDownBtn.next)
			if (GetKeyState(VK_SHIFT)<0)
				updateServer(US_SELECT_NEXT2);
			else
				updateServer(US_SELECT_NEXT);
	}
	else if (isOnButtonToggleSharedStash(msg->x,msg->y))
	{
		log_msg("push up left button shared\n");
		if (isDownBtn.toggleToSharedStash)
		{
			if (!inMultiplayerGame())
			{
				if (PCPY->showSharedStash)
					updateServer(US_SELECT_SELF);
				else
					updateServer(US_SELECT_SHARED);
			}
		}
	}
	else if (isOnButtonPreviousIndexStash(msg->x,msg->y))
	{
		log_msg("select left button previous index\n");
		if (isDownBtn.previousIndex)
			if (GetKeyState(VK_SHIFT)<0)
				updateServer(US_SELECT_PREVIOUS_INDEX2);
			else
				updateServer(US_SELECT_PREVIOUS_INDEX);
	}
	else if (isOnButtonNextIndexStash(msg->x,msg->y))
	{
		log_msg("push up left button next index\n");
		if (isDownBtn.nextIndex)
			if (GetKeyState(VK_SHIFT)<0)
				updateServer(US_SELECT_NEXT_INDEX2);
			else
				updateServer(US_SELECT_NEXT_INDEX);
	}
	else if (isOnButtonPutGold(msg->x,msg->y))
	{
		log_msg("push up left put gold\n");
		if (isDownBtn.putGold)
		{
			if (!inMultiplayerGame())
			{
				updateServer(US_PUTGOLD);
			}
		}
	}
	else if (isOnButtonTakeGold(msg->x,msg->y))
	{
		log_msg("push up left take gold\n");
		if (isDownBtn.takeGold)
		{
			if (!inMultiplayerGame())
			{
				updateServer(US_TAKEGOLD);
			}
		}
	}
	else
	{
		return 0;
	}

	return 1;
}

void __fastcall printPageNumber(LPWSTR maxGoldText, DWORD x, DWORD y, DWORD color, DWORD bfalse)
{
	if (!D2Client::IsExpansion())
	{
		D2PrintString(maxGoldText,x,y,color,bfalse);
		return;
	}

	Unit* ptChar = D2GetClientPlayer();
	WCHAR popupText[100];

	if (PCPY->currentStash)
	{
		bool isShared = PCPY->currentStash->isShared;
		bool isIndex = PCPY->currentStash->isIndex;
		DWORD currentId = PCPY->currentStash->id;

		// Display the appropriate stash name (Personal/Shared)
		if (PCPY->currentStash->name && PCPY->currentStash->name[0])
		{
			const int BUFFER_MAX = 50;

			// Convert the custom page number to a string and concat
			// it at the end of the custom name. This code is ugly,
			// there should be a more efficient/safer C way of doing this.
			char pageAsString[BUFFER_MAX];
			char customNameWithPage[100];

			// Convert the integer to a string
			sprintf(pageAsString, "%d", currentId + 1);

			// Concat page number to the end of the custom stash name
			strcpy(customNameWithPage, PCPY->currentStash->name);
			strcat(customNameWithPage, " [");
			strcat(customNameWithPage, pageAsString);
			strcat(customNameWithPage, "]");

			// Display the custom page name with the page number
			mbstowcs(popupText, customNameWithPage, BUFFER_MAX);
		}
		else
		{
			_snwprintf(popupText, sizeof(popupText), isShared ? STR_SHARED_PAGE_NUMBER : STR_PERSONAL_PAGE_NUMBER, currentId + 1);
		}

		int stashNameColor = isShared ? (isIndex ? GREEN : RED) : (isIndex ? GREEN : WHITE);
		D2PrintString(popupText, x, y, stashNameColor, bfalse);
	}
	else
	{
		_snwprintf(popupText, sizeof(popupText), STR_NO_SELECTED_PAGE);
		D2PrintString(popupText, x, y, WHITE, bfalse);
	}

	// printGoldMaxPopup
	DWORD mx = D2GetMouseX();
	DWORD my = D2GetMouseY();
	if ((RX(0x5E) < mx) && (mx < RX(0xF8)) && (RY(0x1C8) < my) && (my < RY(0x1B6)) )
	{
		if (!inMultiplayerGame())
		{
			_snwprintf(popupText, sizeof(popupText), L"%s\n%s: %u", maxGoldText, STR_SHARED_GOLD_QUANTITY, PCPY->sharedGold);
		}
		else
		{
			_snwprintf(popupText, sizeof(popupText), L"%s", maxGoldText);
		}
		DWORD x = D2GetPixelLen(maxGoldText);
		DWORD x2 = D2GetPixelLen(popupText) - x;
		D2PrintPopup(popupText, RX(0xA8 - max(x, x2) / 2), RY(0x1CA), WHITE, 0);
	}
}

static Stash* curStash = NULL;
static DWORD currentSawStash = 0;

Unit* __stdcall getNextItemForSet(Unit* ptItem)
{
	Unit* item = ptItem?D2UnitGetNextItem(ptItem):NULL;
	if (item) return item;

	if (!curStash)
	{
		Unit* ptChar = D2GetClientPlayer();
		switch (currentSawStash)
		{
		case 0:
			curStash = PCPY->selfStash;
			currentSawStash = displaySharedSetItemNameInGreen ? 1 : 2;
			break;
		case 1:
			curStash = PCPY->sharedStash;
			currentSawStash = 2;
			break;
		default:
			return NULL;
		}
	}
	else
	{
		curStash = curStash->nextStash;
	}
	if (curStash)
	{
		item = curStash->ptListItem;
		if (item) return item;
	}
	return getNextItemForSet(item);
}

Unit* __stdcall initGetNextItemForSet(Inventory* ptInventory)
{
	Unit* ptChar = D2GetClientPlayer();
	if (ptChar->nUnitType != UNIT_PLAYER) return NULL;
	if (!PCPY) return NULL;

	curStash = NULL;
	currentSawStash = 0;
	Unit* item = D2InventoryGetFirstItem(ptInventory);
	if (item) return item;
	return getNextItemForSet(item);
}

FCT_ASM(caller_manageBtnDown_111)
PUSH EBP
CALL manageBtnDown
TEST EAX, EAX
JE IS_NOT_ON_BUTTON
POP EDX
MOV EDX, DWORD PTR DS : [EDX + 0x10]
MOV DWORD PTR DS : [EDX], 1
POP EDI
POP ESI
POP EBP
POP EBX
RETN 4
IS_NOT_ON_BUTTON :
	JMP D2ClickOnStashButton
}}

FCT_ASM(caller_manageBtnUp_111)
PUSH EBX
CALL manageBtnUp
MOV isDownBtn.all, 0
TEST EAX, EAX
JE IS_NOT_ON_BUTTON
POP EDX
MOV EDX, DWORD PTR DS : [EDX + 0x1A]
MOV DWORD PTR DS : [EDX], 0
SUB EDX, 8
MOV DWORD PTR DS : [EDX], 0
ADD EDX, 4
MOV DWORD PTR DS : [EDX], 0
ADD EDX, 0x68
MOV DWORD PTR DS : [EDX], 0
POP EDI
POP ESI
POP EBP
POP EBX
POP ECX
RETN 4
IS_NOT_ON_BUTTON:
JMP D2ClickOnStashButton
}}

FCT_ASM ( initBtnsStates )
	MOV isDownBtn.all,0
	ADD ESP,0x104
	RETN
}}

void Install_InterfaceStash()
{
	static bool isInstalled = false;
	if (isInstalled) return;

	Install_UpdateServer();
	Install_AlpacaImagesFiles();

	log_msg("[Patch] Stash Interface\n");

	DWORD ButtonImagesOffset = D2Client::GetAddress(0x9DE26);
	DWORD PageNumberOffset = D2Client::GetAddress(0x9DE03);
	DWORD MouseDownSoundOffset = D2Client::GetAddress(0x9FC76);
	DWORD MouseUpSoundOffset = D2Client::GetAddress(0x9FAA9);
	DWORD OpenStashPageInitStateOffset = D2Client::GetAddress(0x9441A);
	DWORD GreenSetItemSearchOffset = D2Client::GetAddress(0x91A24);
	DWORD GreenSetItemNextSearchOffset = D2Client::GetAddress(0x91ABB);
	
	// Print button images
	Memory::SetCursor(ButtonImagesOffset);
	Memory::ChangeCallC((DWORD)D2Client::D2LoadBuySelBtn, (DWORD)printBtns);

	// print page number
	Memory::SetCursor(PageNumberOffset);
	Memory::ChangeCallB((DWORD)D2Win::D2PrintString, (DWORD)printPageNumber);

	// Manage mouse down (Play sound)
	Memory::SetCursor(MouseDownSoundOffset);
	Memory::ChangeCallC((DWORD)D2ClickOnStashButton, (DWORD)caller_manageBtnDown_111);

	// Manage mouse up
	Memory::SetCursor(MouseUpSoundOffset);
	Memory::ChangeCallC((DWORD)D2ClickOnStashButton, (DWORD)caller_manageBtnUp_111);

	// init state of button on open stash page
	Memory::SetCursor(OpenStashPageInitStateOffset);
	Memory::ChangeByte(0x81, 0xE9);
	Memory::ChangeCallA(0x104C4, (DWORD)initBtnsStates);
	Memory::ChangeByte(0x00, 0x90);

	// init the search of print in green the item set name we have in stash
	Memory::SetCursor(GreenSetItemSearchOffset);
	Memory::ChangeCallB((DWORD)D2Common::D2InventoryGetFirstItem, (DWORD)initGetNextItemForSet);

	// Get next item for print in green the item set name we have in stash
	Memory::SetCursor(GreenSetItemNextSearchOffset);
	Memory::ChangeCallB((DWORD)D2Common::D2UnitGetNextItem, (DWORD)getNextItemForSet);

	if (active_logFileMemory) log_msg("\n");
	isInstalled = true;
}
