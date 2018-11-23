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

#include "UpdateClient.h"
#include "InfinityStash.h"
#include "Commands.h"
#include "Common.h"

void updateClient(Unit* ptChar, DWORD mFunc, DWORD p1, DWORD p2, DWORD p3)
{
	void* ptNetClient;
	DataPacket packet;

	// Intialize the packet to send to client
	ZeroMemory(&packet, sizeof(DataPacket));
	packet.mType = 0x9D;
	packet.mFunc = (BYTE)mFunc;
	packet.mSize = sizeof(DataPacket);
	packet.mPlayerID = ptChar->nUnitId;
	packet.mParam1 = p1;
	packet.mParam2 = p2;
	packet.mParam3 = p3;

	ptNetClient = D2GetClient( ptChar, __FILE__ , __LINE__);

	// Send packet to client for remove placed skills
	D2SendPacket(ptNetClient, &packet, sizeof(DataPacket));
}

void updateClient(Unit* ptChar, DWORD mFunc, char* msg)
{
	void* ptNetClient;
	DataPacket packet;

	// Intialize the packet to send to client
	ZeroMemory(&packet, sizeof(DataPacket));
	packet.mType = 0x9D;
	packet.mFunc = (BYTE)mFunc;
	packet.mSize = sizeof(DataPacket);
	packet.mPlayerID = ptChar->nUnitId;
	if (msg != NULL && strlen(msg) >= 20)
		return;
	if (msg != NULL)
		strcpy((char*)&packet.mItemID, msg);

	ptNetClient = D2GetClient(ptChar, __FILE__, __LINE__);

	// Send packet to client for remove placed skills
	D2SendPacket(ptNetClient, &packet, sizeof(DataPacket));
}

DWORD __fastcall handleClientUpdate(DataPacket* packet)
{
	log_msg("[CLIENT] Received custom message: %d with param: %08X , %08X , %08X\n",packet->mFunc,packet->mParam1,packet->mParam2,packet->mParam3);
	switch (packet->mFunc)
	{
	case UC_SELECT_STASH:	 setSelectedStashClient(packet->mParam1, packet->mParam2, packet->mParam3, (packet->mParam2 & 8) == 8); return 1;
	case UC_SHARED_GOLD :	 updateSharedGold(packet->mParam1); return 1;
	case UC_PAGE_NAME:		 renameCurrentStash(D2GetClientPlayer(), (char*)&packet->mItemID); return 1;
	default : return 0;
	}
}

FCT_ASM(caller_handleClientUpdate_111)
LEA ECX, DWORD PTR SS : [ESP + 8]
CALL handleClientUpdate
POP EDI
POP ESI
MOV ESP, EBP
POP EBP
RETN
}}

void Install_UpdateClient()
{
	static bool isInstalled = false;
	if (isInstalled) return;
	
	log_msg("[Patch] Receive Item Packets from Client\n");

	DWORD ExecuteOurPacketsOnlyOffset = D2Client::GetAddress(0x84D96);

	// Execute if it's our packet else continue
	Memory::SetCursor(ExecuteOurPacketsOnlyOffset);
	Memory::ChangeCallA(0xCF, (DWORD)caller_handleClientUpdate_111);

	if (active_logFileMemory) log_msg("\n");
	isInstalled = true;
}