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

#include "D2Game.h"
#include "D2Common.h"

void D2Game::Init()
{
	Name = "D2Game.dll";
	Offset = LoadDiabloLibrary();
	SetFunctions();
}

void D2Game::SetFunctions()
{
	D2GetClient = (TD2GetClient)D2GetClient_111;
	D2SendPacket = (TD2SendPacket)D2SendPacket_111;
	D2LoadInventory = (TD2LoadInventory)D2LoadInventory_111;
	D2GameGetObject = (TD2GameGetObject)D2GameGetObject_111;
	D2SaveGame = (TD2SaveGame)GetAddress(0xBE660);
	ptClientTable = (NetClient**)GetAddress(0x1105E0);
	D2VerifIfNotCarry1 = (TD2VerifIfNotCarry1)GetAddress(0xB2F70);

	D2SendPacketDirect = (TD2SendPacket)GetAddress(0xDB780);
	D2LoadInventoryDirect = (TD2LoadInventory)GetAddress(0x3A4C0);
	D2GameGetObjectDirect = (TD2GameGetObject)GetAddress(0x6DC40);
}

__declspec (naked) void D2Game::D2GetClient_111()
{
	__asm {
		PUSH ECX
		CALL D2Common::D2GetPlayerData
		MOV EAX, DWORD PTR DS : [EAX + 0x9C]
		RETN 4
	}
}

__declspec (naked) void D2Game::D2SendPacket_111()
{
	__asm {
		POP EAX
		PUSH EDX
		PUSH EAX
		MOV EAX, ECX
		JMP D2SendPacketDirect
	}
}

__declspec (naked) void D2Game::D2LoadInventory_111()
{
	__asm {
		MOV EAX, DWORD PTR SS : [ESP + 4]
		MOV DWORD PTR SS : [ESP + 4], EDX
		JMP D2LoadInventoryDirect
	}
}

__declspec (naked) void D2Game::D2GameGetObject_111()
{
	__asm {
		MOV EAX, EDX
		MOV EDX, DWORD PTR SS : [ESP + 4]
		CALL D2GameGetObjectDirect
		RETN 4
	}
}

D2Game::TD2GetClient D2Game::D2GetClient;
D2Game::TD2SendPacket D2Game::D2SendPacket;
D2Game::TD2LoadInventory D2Game::D2LoadInventory;
D2Game::TD2GameGetObject D2Game::D2GameGetObject;
D2Game::TD2SaveGame D2Game::D2SaveGame;
NetClient** D2Game::ptClientTable;
D2Game::TD2SendPacket D2Game::D2SendPacketDirect;
D2Game::TD2GameGetObject D2Game::D2GameGetObjectDirect;
D2Game::TD2LoadInventory D2Game::D2LoadInventoryDirect;
D2Game::TD2VerifIfNotCarry1 D2Game::D2VerifIfNotCarry1;