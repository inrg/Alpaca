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

#include "LoadPlayerData.h"
#include "InfinityStash.h"
#include "ExtendedSaveFile.h"
#include "SharedSaveFile.h"
#include "Common.h"

DWORD __stdcall LoadSPCustomData(Unit* ptChar)
{
	DWORD size = 0;
	BYTE* data;

	log_msg("--- Start LoadSPCustomData ---\n");

	if (!ptChar)
		{log_msg("LoadSPCustomData : ptChar == NULL\n");return 0x1B;}//Unknow failure
	if (ptChar->nUnitType != UNIT_PLAYER)
		{log_msg("LoadSPCustomData : ptChar->nUnitType != UNIT_PLAYER\n");return 0x1B;}//Unknow failure
	if (!PCPlayerData)
		{log_msg("LoadSPCustomData : PCPlayerData == NULL\n");return 0x1B;}//Unknow failure
	if (!PCGame)
		{log_msg("LoadSPCustomData : PCGame == NULL\n");return 0x1B;}//Unknow failure

	DWORD ret=0;
	if (D2Client::IsExpansion())
	{
		log_msg("is LOD Game\n");
		data = readExtendedSaveFile(PCPlayerData->name, &size);
		ret = loadExtendedSaveFile(ptChar, data, size);
		D2FogMemDeAlloc(data,__FILE__,__LINE__,0);
		if (!ret)
		{
			data = readSharedSaveFile(PCPlayerData->name, &size);
			ret = loadSharedSaveFile(ptChar, data, size);
			D2FogMemDeAlloc(data,__FILE__,__LINE__,0);
		}
	} else {
		log_msg("is not LOD Game\n");
	}

	log_msg("End LoadSPCustomData.\n\n");
	return ret;
}


FCT_ASM (caller_LoadSPPlayerCustomData )
	MOV ESI,EAX
	TEST ESI,ESI
	JNZ JMP_LoadSPPlayerCustomData
	PUSH DWORD PTR SS:[ESP+0x14]
	CALL LoadSPCustomData
	MOV ESI,EAX
	TEST ESI,ESI
	JNZ JMP_LoadSPPlayerCustomData
	RETN
JMP_LoadSPPlayerCustomData:
	ADD DWORD PTR SS:[ESP],0x17
	RETN
}}


/*
return value :
0:ok
1:Unable to enter game. Bad character version
2:Unable to enter game. Bad character quest data
3:Unable to enter game. Bad character waypoint data
4:Unable to enter game. Bad character stats data
5:Unable to enter game. Bad character skills data
6:Unable to enter game
7:Unable to enter game. Bad inventory data
8:Unable to enter game. Bad dead bodies
9:Unable to enter game. Bad header
0A:Unable to enter game. bad hireables
0B:Unable to enter game, bad intro data
0C:Unable to enter game. bad item
0D:Unable to enter game, bad dead body item
0E:Unable to enter game, generic bad file
0F:Game is full
10:Version do not match....
11:Unable to enter game,Your character must kill Baal to play nightmare game
12:Unable to enter game,Your character must kill Baal in nightmare difficulty to play hell game
13:Unable to enter game, a normal character cannot join a game create an harcore character.
14:Unable to enter game, an harcore character cannot join a game create by a normal character.
15:A dead character cannot join or create any game
16:unknow failure
17:Unable to enter game. A D2 Character cannot join an expansion character
18:Expansion cannot join D2
19:Failures to join game
1A:Unable to enter game
1B:unknow failure
*/
enum {
	TS_PUTGOLD=0,
	TS_TAKEGOLD
};

#pragma pack(1)
typedef struct {
	BYTE	packID;
	BYTE	packSize;
	DWORD	finalSize:31;
	DWORD	isCustom:1;
	BYTE	type;
	BYTE	data[0xFF];
} t_rcvMsg;
#pragma pack()

struct s_MPSaveFile
{
	DWORD clientID;
	int sizeExtended;
	int curExtended;
	BYTE* dataExtended;
	int sizeShared;
	int curShared;
	BYTE* dataShared;
	s_MPSaveFile* prev;
	s_MPSaveFile* next;
	bool completed;
};

static BYTE customPackID = 0x6C;
static s_MPSaveFile * receivedSaveFiles = NULL;

void freeCurrentCF(DWORD memoryPool, s_MPSaveFile** curSF)
{
	if (*curSF == NULL) return;
	D2FreeMem(memoryPool, (*curSF)->dataExtended,__FILE__,__LINE__,0);
	D2FreeMem(memoryPool, (*curSF)->dataShared, __FILE__, __LINE__, 0);

	if ((*curSF)->next)
		(*curSF)->next->prev = (*curSF)->prev;
	if ((*curSF)->prev)
		(*curSF)->prev->next = (*curSF)->next;
	else
		receivedSaveFiles = (*curSF)->next;
	D2FreeMem(memoryPool, *curSF,__FILE__,__LINE__,0);
	*curSF = NULL;
}

void sendData(BYTE* data, DWORD size)
{
	t_rcvMsg pack;

	pack.packID = customPackID;
	pack.finalSize = size;
	pack.isCustom = true;

	DWORD sended = 0;
	while (sended < size)
	{
		pack.packSize = (BYTE) (size - sended > 0xFE ? 0xFF : size - sended + 1);
		CopyMemory(pack.data, &data[sended], pack.packSize);
		log_msg("Loading Send Packet: \t finalSize=%X\t packSize=%02X\t data=%08X\n", pack.finalSize, pack.packSize, pack.data);
		D2SendToServer(pack.packSize+7, 0, &pack);
		sended += pack.packSize -1;
	}
	log_msg("\n");
}

static DWORD	sizeExtended;
static BYTE*	dataExtended;
static DWORD	sizeShared;
static BYTE*	dataShared;

void __fastcall SendSaveFiles(char* ptPath, DWORD maxsize, char* name)
{
	D2FogGetSavePath(ptPath,maxsize);

	log_msg("\n--- Start SendSaveFiles ---\n");

	// Send Extended Save File
	log_msg("Send Extended Save File\n");
	dataExtended = readExtendedSaveFile(name, &sizeExtended);
	sendData(dataExtended, sizeExtended);
	D2FogMemDeAlloc(dataExtended,__FILE__,__LINE__,0);

	// Ending load
	log_msg("End SendSaveFiles.\n\n");
}

DWORD __stdcall ReceiveSaveFiles (DWORD clientID, t_rcvMsg* msg)
{
	if( (msg->packID != customPackID) || !msg->isCustom) return msg->packID;

	log_msg("Loading Receive Packet: clientID=%d\t finalSize=%X\t packSize=%02X\t data=%08X\n", clientID, msg->finalSize, msg->packSize, msg->data);

	NetClient* ptClient = ptClientTable[clientID & 0xFF];

	// If the client is unable to be retrieved, then abort because we will crash.
	// This will happen if the player tries to join the same LAN game with the same char.
	// PlugY 11.02 in this situation seems to still retrieve the client and not crash,
	// even though at the end of the day the game won't allow the player to join.
	// Returning here as a permanent fix (unless we need to change)
	if (!ptClient) return 0;

	s_MPSaveFile* curSF = receivedSaveFiles;
	while (curSF && (clientID != curSF->clientID) )
		curSF = curSF->next;

	if (curSF && curSF->completed)
		freeCurrentCF(0 * PClientGame->memoryPool, &curSF);

	if (!curSF)
	{
		curSF = (s_MPSaveFile *)D2AllocMem(0 * PClientGame->memoryPool, sizeof(s_MPSaveFile),__FILE__,__LINE__,0);
		ZeroMemory(curSF, sizeof(s_MPSaveFile));
		curSF->clientID = clientID;
		curSF->next = receivedSaveFiles;
		if (receivedSaveFiles) receivedSaveFiles->prev = curSF;
		receivedSaveFiles = curSF;
	}

	log_msg("curSF = %08X\tcurSF->completed = %d\n", curSF, curSF->completed);

	DWORD size = msg->packSize - 1;
	
	// Only the personal stash is loaded in LAN games.
	if (curSF->sizeExtended == 0)
	{
		curSF->sizeExtended = msg->finalSize;
	}

	d2_assert(curSF->sizeExtended != msg->finalSize, "Size of extented file has change", __FILE__, __LINE__);

	if (!curSF->dataExtended)
	{
		curSF->dataExtended = (BYTE *)D2AllocMem(0 * PClientGame->memoryPool, curSF->sizeExtended, __FILE__, __LINE__, 0);
	}

	CopyMemory(&curSF->dataExtended[curSF->curExtended], msg->data, size);
	curSF->curExtended += size;
	
	if(curSF->sizeExtended && (curSF->sizeExtended == curSF->curExtended))
	{
		curSF->completed = true;
		log_msg("ReceiveSaveFiles completed.\n");
	}
	log_msg("End Load Receive Packet\n");

	return msg->packID+10;
}

DWORD __stdcall LoadMPCustomData(Unit* ptChar)
{
	log_msg("Start LoadMPCustomData\n");
	if (!D2Client::IsExpansion()) return 0;
	if (!ptChar) return NULL;
	if (!ptChar)
		{log_msg("LoadMPCustomData : ptChar == NULL\n");return 0x1B;}//Unknow failure
	if (ptChar->nUnitType != UNIT_PLAYER)
		{log_msg("LoadMPCustomData : ptChar->nUnitType != UNIT_PLAYER\n");return 0x1B;}//Unknow failure
	if (!PCPlayerData)
		{log_msg("LoadMPCustomData : PCPlayerData == NULL\n");return 0x1B;}//Unknow failure

	NetClient* ptClient = D2GetClient(ptChar,__FILE__,__LINE__);

	s_MPSaveFile* curSF = receivedSaveFiles;
	while (curSF && (ptClient->clientID != curSF->clientID) )
		curSF = curSF->next;

	DWORD ret=0;

	if (!curSF)
	{
		log_msg("Server has received no data from extra save files of character %s\n", PCPlayerData->name);
		ret = 0xE; //Unable to enter game, generic bad file
	}

	if (!ret && !curSF->completed)
	{
		log_msg("Server hasn't receive all data from extra save files of character %s\n",PCPlayerData->name);
		ret = 0xE; //Unable to enter game, generic bad file
	}

	if (!ret && !PCGame)
	{
		log_msg("LoadMPCustomData : PCGame == NULL\n");
		ret = 0x1B; //Unknown failure
	}

	if (!ret)
	{
		ret = loadExtendedSaveFile(ptChar, curSF->dataExtended, curSF->sizeExtended);
	}
	
	freeCurrentCF(PCGame->memoryPool, &curSF);

	log_msg("--- End LoadMPCustomData. ---\n\n");

	return ret;
}

FCT_ASM(caller_LoadMPPlayerCustomData_111)
PUSH DWORD PTR SS : [EBX]
CALL LoadMPCustomData
TEST EAX, EAX
JNZ JMP_LoadMPlayerCustomData
CMP DWORD PTR DS : [EBX], 0
JNZ Continue_LoadMP
ADD DWORD PTR SS : [ESP], 0x21
Continue_LoadMP :
	RETN
	JMP_LoadMPlayerCustomData :
SUB DWORD PTR SS : [ESP], 0x12
RETN
}}

FCT_ASM(caller_SendSaveFiles_111)
POP EAX
PUSH DWORD PTR CS : [EAX + 0x01] //"name"
PUSH EAX
JMP SendSaveFiles
}}

FCT_ASM(caller_ReceiveSaveFiles_111)
PUSH ECX
PUSH EDX
LEA EBX, DWORD PTR DS : [EBP + 4]
PUSH EBX		//Message
MOV EBX, DWORD PTR SS : [EBP]
PUSH EBX		//ID client
CALL ReceiveSaveFiles
POP EDX
POP ECX
RETN
}}

void Install_LoadPlayerData()
{
	static bool isInstalled = false;
	if (isInstalled || !active_PlayerCustomData) return;

	log_msg("[Patch] Load Player Custom Data\n");

	DWORD LoadSinglePlayerCustomDataOffset = D2Game::GetAddress(0x3BCCD);
	DWORD LoadMultiPlayerCustomDataOffset = D2Game::GetAddress(0x3BB57);
	DWORD SendSaveFilesToServerOffset = D2Client::GetAddress(0xB638C);
	DWORD ReceiveSaveFilesFromClientOffset = D2Game::GetAddress(0xD53E9);

	// Load SP player custom data.
	Memory::SetCursor(LoadSinglePlayerCustomDataOffset);
	Memory::ChangeByte(0x8B, 0xE8);
	Memory::ChangeCallA(0x75F685F0 , (DWORD)caller_LoadSPPlayerCustomData);
	Memory::ChangeByte(0x16, 0x90);

	// Load MP player custom data.
	Memory::SetCursor(LoadMultiPlayerCustomDataOffset);
	Memory::ChangeByte(0x83, 0xE8);
	Memory::ChangeCallA(0x2174003B, (DWORD)caller_LoadMPPlayerCustomData_111);

	// Send save files to Server.
	Memory::SetCursor(SendSaveFilesToServerOffset);
	Memory::ChangeCallB((DWORD)Fog::D2FogGetSavePath, (DWORD)caller_SendSaveFiles_111);

	// Receive save files from client.
	Memory::SetCursor(ReceiveSaveFilesFromClientOffset);
	Memory::ChangeByte(0x8B, 0xE8);
	Memory::ChangeCallA(0xB60F005D, (DWORD)caller_ReceiveSaveFiles_111);
	Memory::ChangeByte(0x45, 0x90);
	Memory::ChangeByte(0x04, 0x90);

	if (active_logFileMemory) log_msg("\n");
	isInstalled = true;
}

