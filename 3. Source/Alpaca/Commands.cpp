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

#include "UpdateServer.h"
#include "UpdateClient.h"
#include "InfinityStash.h"
#include "InterfaceStash.h"
#include "Common.h"

#define MAX_CMD_SIZE 200

// Regular Commands
const char* CMD_SAVE = "/save";

// Stash Management Commands
const char* CMD_RENAME_PAGE = "/rename";
const char* CMD_SET_INDEX = "/set";
const char* CMD_UNSET_INDEX = "/unset";
const char* CMD_SET_MAIN_INDEX = "/setmain";
const char* CMD_INSERT_PAGE = "/insert";
const char* CMD_DELETE_PAGE = "/delete";
const char* CMD_SELECT_PAGE = "/page";
const char* CMD_SWAP = "/swap";
const char* CMD_TOGGLE = "/toggle";

void savePlayers(Unit* ptChar)
{
	D2SaveGame(PCGame);
}

void maxGold(Unit* ptChar)
{
	log_msg("maxGold\n");

	DWORD maxGold =     D2GetMaxGold(ptChar);
	DWORD maxGoldBank = D2GetMaxGoldBank(ptChar);
	DWORD playerGold = D2GetPlayerStat(ptChar, STATS_GOLD, 0);
	DWORD playerGoldBank = D2GetPlayerStat(ptChar, STATS_GOLDBANK, 0);
	if ( (playerGold < maxGold) || (playerGoldBank < maxGoldBank) ) {
		D2AddPlayerStat( ptChar, STATS_GOLD,	 maxGold-playerGold, 0 );
		D2AddPlayerStat( ptChar, STATS_GOLDBANK, maxGoldBank-playerGoldBank, 0 );
	} else {
		D2AddPlayerStat( ptChar, STATS_GOLD,	 100000, 0 );
	}

	PCPY->sharedGold = 0xFFFFFFFF;
	updateClient(ptChar, UC_SHARED_GOLD, PCPY->sharedGold, 0, 0);
}

void putGold(Unit* ptChar, DWORD amount)
{
	log_msg("putGold : %d\n", amount);

	DWORD playerGold = D2GetPlayerStat(ptChar, STATS_GOLD, 0);
	DWORD toAdd = 0xFFFFFFFF - PCPY->sharedGold;
	if (playerGold < toAdd)
		toAdd = playerGold;
	if (amount && (toAdd > amount))
		toAdd = amount;
	D2AddPlayerStat(ptChar, STATS_GOLD, 0 - toAdd, 0);
	PCPY->sharedGold += toAdd;
	updateClient(ptChar, UC_SHARED_GOLD, PCPY->sharedGold, 0, 0);
}

void takeGold(Unit* ptChar, DWORD amount)
{
	log_msg("takeGold : %d\n", amount);

	DWORD maxGold = D2GetMaxGold(ptChar) - D2GetPlayerStat(ptChar, STATS_GOLD, 0);
	DWORD toAdd = maxGold < PCPY->sharedGold ? maxGold : PCPY->sharedGold;
	if (amount && (toAdd > amount))
		toAdd = amount;
	D2AddPlayerStat(ptChar, STATS_GOLD, toAdd, 0);
	PCPY->sharedGold -= toAdd;
	updateClient(ptChar, UC_SHARED_GOLD, PCPY->sharedGold, 0, 0);
}

void updateSharedGold(DWORD goldAmount)
{
	Unit* ptChar = D2GetClientPlayer();
	log_msg("SharedGold = %d\n",goldAmount);
	PCPY->sharedGold = goldAmount;
}

int __stdcall commands(char* ptText)
{
	if (!D2Client::IsExpansion()) return 1;

	log_msg("Command: %s\n", ptText);
	Unit* ptChar = D2GetClientPlayer();

	char command[MAX_CMD_SIZE];
	ZeroMemory(command, MAX_CMD_SIZE);
	strncpy(command, ptText, MAX_CMD_SIZE - 1);
	_strlwr(command);

	// The order matters here in terms of the rename commands because
	// the code currently checks to see if the string matches a string
	// up to a certain point. Thus /rename##### will match the first match.
	// Just keep this in mind.
	if (!strncmp(command, CMD_RENAME_PAGE, strlen(CMD_RENAME_PAGE)))
	{
		char* param = &ptText[strlen(CMD_RENAME_PAGE)];
		Stash* ptStash = PCPY->currentStash;
		if (!ptStash) return 0;

		int len = strlen(param);

		while (len > 0 && param[0] == ' ')
		{
			param++;
			len--;
		}

		if (len > 0 && len <= 15)
		{
			log_msg("Rename current page on Client : %s -> %s\n", ptStash->name, param);
			renameCurrentStash(ptChar, param);
			for (int i = 0; i <= len; i++)
				updateServer(US_PAGENAME + (param[i] << 8));
		}

		else if (len == 0)
		{
			log_msg("Rename current page on Client: %s\n", ptStash->name);
			renameCurrentStash(ptChar, NULL);
			updateServer(US_PAGENAME);
		}

		return 0;
	}

	if (!strcmp(command, CMD_SAVE))
	{
		updateServer(US_SAVE);
		return 0;
	}

	if (!strcmp(command, CMD_SET_INDEX))
	{
		updateServer(US_SET_INDEX);
		return 0;
	}

	if (!strcmp(command, CMD_SET_MAIN_INDEX))
	{
		updateServer(US_SET_MAIN_INDEX);
		return 0;
	}

	if (!strcmp(command, CMD_UNSET_INDEX))
	{
		updateServer(US_UNSET_INDEX);
		return 0;
	}

	if (!strcmp(command, CMD_INSERT_PAGE))
	{
		insertStash(ptChar);
		updateServer(US_INSERT_PAGE);
		return 0;
	}

	if (!strcmp(command, CMD_DELETE_PAGE))
	{
		if (deleteStash(ptChar, true))
			updateServer(US_DELETE_PAGE);
		return 0;
	}

	if (!strncmp(command, CMD_SELECT_PAGE, strlen(CMD_SELECT_PAGE)))
	{
		int page = atoi(&command[strlen(CMD_SELECT_PAGE)]) - 1;
		if (page < 0)
			return 1;
		updateServer(US_SELECT_PAGE3 + ((page & 0xFF000000) >> 16));
		updateServer(US_SELECT_PAGE2 + ((page & 0xFF0000) >> 8));
		updateServer(US_SELECT_PAGE1 + (page & 0xFF00));
		updateServer(US_SELECT_PAGE + ((page & 0xFF) << 8));
		return 0;
	}

	if (!strncmp(command, CMD_SWAP, strlen(CMD_SWAP)))
	{
		int page = atoi(&command[strlen(CMD_SWAP)]) - 1;
		if (page < 0 && PCPY->currentStash->nextStash)
			page = PCPY->currentStash->nextStash->id;
		if (page < 0)
			return 1;
		updateServer(US_SWAP3 + ((page & 0xFF000000) >> 16));
		updateServer(US_SWAP2 + ((page & 0xFF0000) >> 8));
		updateServer(US_SWAP1 + (page & 0xFF00));
		updateServer(US_SWAP0 + ((page & 0xFF) << 8));
		return 0;
	}
	
	if (!strncmp(command, CMD_TOGGLE, strlen(CMD_TOGGLE)))
	{
		if (!inMultiplayerGame())
		{
			int page = atoi(&command[strlen(CMD_TOGGLE)]) - 1;
			if (page < 0)
				return 1;
			updateServer(US_SWAP3 + ((page & 0xFF000000) >> 16));
			updateServer(US_SWAP2 + ((page & 0xFF0000) >> 8));
			updateServer(US_SWAP1 + (page & 0xFF00));
			updateServer(US_SWAP0_TOGGLE + ((page & 0xFF) << 8));
			return 0;
		}
	}

	return 1;
}

FCT_ASM(caller_Commands_113d)
	TEST EAX, EAX
	JE MANAGESOUNDCHAOSDEBUG
	PUSH EDI
	CALL commands
	TEST EAX, EAX
	JNZ MANAGESOUNDCHAOSDEBUG
	ADD DWORD PTR SS : [ESP], 7
MANAGESOUNDCHAOSDEBUG:
	RETN 8
}}

void Install_Commands()
{
	static bool isInstalled = false;
	if (isInstalled) return;

	Install_UpdateServer();

	log_msg("[Patch] Commands Support\n");

	DWORD CustomCommandOffset = D2Client::GetAddress(0xB1FD6);

	// Run custom commmand
	Memory::SetCursor(CustomCommandOffset);
	Memory::ChangeByte(0x83, 0xE8); 
	Memory::ChangeCallA(0xC08508C4, (DWORD)caller_Commands_113d);

	if (active_logFileMemory) log_msg("\n");
	isInstalled = true;
}