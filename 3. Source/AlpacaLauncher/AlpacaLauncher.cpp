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

#include "Resource.h"
#include <stdlib.h>
#include <stdio.h>
#include "../Alpaca/VersionUtility.h"
#include "../Alpaca/Utility.h"

const char* LAUNCHING_LABEL = "LAUNCHING";
const char* PARAM_LABEL = "Param";
const char* PROGRAM_NAME = "Alpaca";
const char* GAME_EXE_NAME = "Game.exe";
const LPSTR ALPACA_DLL_NAME = "Alpaca.dll";
const char* INI_FILE_NAME = "Alpaca.ini";
const char* LONG_COMMAND = "The command is too long.";

BYTE loadDll[] = {
	0xFF,0x74,0x24,0x04,			//PUSH DWORD PTR SS:[ESP+4]
	0xFF,0x15,0x40,0xC0,0xA7,0x6F,  //CALL DWORD PTR DS:[<&KERNEL32.LoadLibraryA>]      ; kernel32.LoadLibraryA
	0x50,							//PUSH EAX
	0x68,0x80,0xBE,0xA7,0x6F,		//PUSH d2gfx.6FA7BE80                               ; ASCII "Alpaca.dll"
	0xFF,0x15,0x40,0xC0,0xA7,0x6F,  //CALL DWORD PTR DS:[<&KERNEL32.LoadLibraryA>]      ; kernel32.LoadLibraryA
	0xA3,0xFC,0xEF,0xA8,0x6F,		//MOV DWORD PTR DS:[6FA8EFFC],EAX
	0x85,0xC0,						//TEST EAX,EAX
	0x74,0x2B,						//JE SHORT d2gfx.6FA7BE33
	0x50,							//PUSH EAX
	0x68,0x90,0xBE,0xA7,0x6F,		//PUSH d2gfx.6FA7BE10                            ;Init String
	0x50,							//PUSH EAX
	0xFF,0x15,0x3C,0xC0,0xA7,0x6F,	//CALL DWORD PTR DS:[<&KERNEL32.GetProcAddress>]    ; kernel32.GetProcAddress
	0x85,0xC0,						//TEST EAX,EAX
	0x74,0x04,						//JE SHORT d2gfx.6FA7BDC1
	0x6A,0x00,						//PUSH 0
	0xEB,0x13,						//JMP SHORT d2gfx.6FA7BDC1
	0x68,0x10,0x27,0x00,0x00,		//PUSH 2710	                                     ;Init Ordinal(10000)
	0xFF,0x74,0x24,0x04,			//PUSH DWORD PTR SS:[ESP+4]
	0xFF,0x15,0x3C,0xC0,0xA7,0x6F,	//CALL DWORD PTR DS:[<&KERNEL32.GetProcAddress>]    ; kernel32.GetProcAddress
	0x85,0xC0,						//TEST EAX,EAX
	0x74,0x02,						//JE SHORT d2gfx.6FA7BDC1
	0xFF,0xD0,						//CALL EAX
	0x58,							//POP EAX
	0x58,							//POP EAX
	0xC2,0x04,0x00,					//RETN 4
	0x00,0x00,0x00,0x00 };			//HANDLE var;

BYTE freeDll[] = {
	0xFF,0x74,0x24,0x04,			//PUSH DWORD PTR SS:[ESP+4]
	0xFF,0x15,0x48,0xC0,0xA7,0x6F,  //CALL DWORD PTR DS:[<&KERNEL32.FreeLibrary>]       ; kernel32.FreeLibrary
	0x50,							//PUSH EAX
	0xA1,0xFC,0xEF,0xA8,0x6F,		//MOV EAX,DWORD PTR DS:[6FA8EFFC]
	0x85,0xC0,						//TEST EAX,EAX
	0x74,0x2D,						//JE SHORT d2gfx.6FA7BE74
	0x50,							//PUSH EAX
	0x68,0xA0,0xBE,0xA7,0x6F,		//PUSH d2gfx.6FA7BE20                            ;Release String
	0x50,							//PUSH EAX
	//0x33,0xC0,					//XOR EAX,EAX
	//0xA3,0xFC,0xEF,0xA8,0x6F,		//MOV DWORD PTR DS:[6FA8EFFC],EAX
	0xFF,0x15,0x3C,0xC0,0xA7,0x6F,	//CALL DWORD PTR DS:[<&KERNEL32.GetProcAdd>; kernel32.GetProcAddress
	0x85,0xC0,						//TEST EAX,EAX
	0x75,0x13,						//JNZ SHORT d2gfx.6FA7BDEF
	0x68,0x11,0x27,0x00,0x00,		//PUSH 2711	                                     ;Release Ordinal(10001)
	0xFF,0x74,0x24,0x04,			//PUSH DWORD PTR SS:[ESP+4]
	0xFF,0x15,0x3C,0xC0,0xA7,0x6F,	//CALL DWORD PTR DS:[<&KERNEL32.GetProcAdd>; kernel32.GetProcAddress
	0x85,0xC0,						//TEST EAX,EAX
	0x74,0x02,						//JE SHORT d2gfx.6FA7BDEF
	0xFF,0xD0,						//CALL EAX
	0xFF,0x15,0x48,0xC0,0xA7,0x6F,  //CALL DWORD PTR DS:[<&KERNEL32.FreeLibrar>; kernel32.FreeLibrary
	0x58,							//POP EAX
	0xC2,0x04,0x00 };				//RETN 4

LPCSTR initFctName = "_Init@4";
LPCSTR releaseFctName = "_Release@0";

typedef int(__stdcall* tDebugActiveProcessStop)(DWORD);
tDebugActiveProcessStop debugActiveProcessStop;

void assertion(const char* pFormat, ...)
{
	char msg[200];
	va_list lArgs;
	va_start(lArgs, pFormat);

	vsprintf(&msg[0], pFormat, lArgs);

	va_end(lArgs);
	MessageBox(0, msg, PROGRAM_NAME, MB_OK | MB_ICONASTERISK);
	exit(1);
}

bool isMemoryAlreadyCorrect(DWORD currentByte1, DWORD currentByte2, DWORD currentByte3, DWORD currentByte4, DWORD currentByte5, LPBYTE currentLibraryAddress, LPBYTE expectedLibraryAddress)
{
	const DWORD expectedByte1 = 0xFF;
	const DWORD expectedByte2 = 0x15;
	const DWORD expectedByte3 = 0xC0;
	const DWORD expectedByte4 = 0xA7;
	const DWORD expectedByte5 = 0x6F;

	bool isAlreadyCorrect = currentByte1 == expectedByte1 &&
							currentByte2 == expectedByte2 &&
							currentByte3 == expectedByte3 &&
							currentByte4 == expectedByte4 &&
							currentByte5 == expectedByte5 &&
							currentLibraryAddress == expectedLibraryAddress;

	return isAlreadyCorrect;
}

bool InstallAlpaca(HANDLE h, LPBYTE addr, char* libraryName)
{
	LPBYTE loadCallerAddr = addr;
	LPBYTE freeCallerAddr = addr;
	LPBYTE loadLibraryAddr = addr;
	LPBYTE freeLibraryAddr = addr;
	LPBYTE getProcAddressAddr = addr;

	// [1.13d]
	loadCallerAddr += 0xAA03;
	freeCallerAddr += 0xA9AA;
	loadLibraryAddr += 0xD11C;
	freeLibraryAddr += 0xD124;
	getProcAddressAddr += 0xD120;

	BYTE buf[200];
	DWORD pos = 0;
	SIZE_T nb = 0;
	int res;

	// Verify if memory are ok.
	bool alreadyInstalled = false;
	res = ReadProcessMemory(h, (LPVOID)loadCallerAddr, buf, 6, &nb);
	if (!res || nb < 6)
	{
		assertion("Alpaca: Read process memory failed. [1]");
	}

	// Lets try some experimental stuff (Check if all the values are expected, and if not then we can fall
	// back to the original behavior so we can keep debugging:
	LPBYTE currentLibraryAddress = *(LPBYTE*)(buf + 2);
	LPBYTE expectedLibraryAddress = loadLibraryAddr;
	bool isAlreadyCorrect = isMemoryAlreadyCorrect(buf[0], buf[1], buf[3], buf[4], buf[5], currentLibraryAddress, expectedLibraryAddress);
	if (!isAlreadyCorrect)
	{
		if (buf[0] != 0xFF || buf[1] != 0x15 || currentLibraryAddress != expectedLibraryAddress)
		{
			if (buf[0] != 0xE8 || buf[3] != 0x00 || buf[4] != 0x00 || buf[5] != 0x90)
			{
				assertion("Alpaca: Read process memory failed. [2] (%08X, %08X, %08X, %08X, %08X, %08X, %08X)",
					currentLibraryAddress, expectedLibraryAddress, buf[0], buf[1], buf[3], buf[4], buf[5]);
			}
			else
			{
				alreadyInstalled = true;
			}
		}
	}
	
	// Reset stuff for safety
	isAlreadyCorrect = false;
	currentLibraryAddress = NULL;
	expectedLibraryAddress = NULL;

	// Let's continue ;D
	res = ReadProcessMemory(h, (LPVOID)freeCallerAddr, buf, 6, &nb);
	if (!res || nb < 6)
	{
		assertion("Alpaca: Read process memory failed. [3]");
	}

	// Try my experimental thing here as well
	currentLibraryAddress = *(LPBYTE*)(buf + 2);
	expectedLibraryAddress = freeLibraryAddr;
	isAlreadyCorrect = isMemoryAlreadyCorrect(buf[0], buf[1], buf[3], buf[4], buf[5], currentLibraryAddress, expectedLibraryAddress);
	if (!isAlreadyCorrect)
	{
		if (buf[0] != 0xFF || buf[1] != 0x15 || *(LPBYTE*)(buf + 2) != freeLibraryAddr)
		{
			if (buf[0] != 0xE8 || buf[3] != 0x00 || buf[4] != 0x00 || buf[5] != 0x90)
			{
				if (!alreadyInstalled)
				{
					assertion("Alpaca: Read process memory failed. [4] (%08X, %08X, %08X, %08X, %08X, %08X, %08X)",
						currentLibraryAddress, expectedLibraryAddress, buf[0], buf[1], buf[3], buf[4], buf[5]);
				}
			}
		}
	}

	if (alreadyInstalled) return true;

	// Alloc custom memory data.
	LPBYTE memory =  (LPBYTE)VirtualAllocEx(h, NULL, 200, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	DWORD oldProtect = -1;
	if (!memory)
	{
		res = ReadProcessMemory(h, addr, buf, 0x40, &nb);
		if (!res || nb < 0x40)
		{
			assertion("Alpaca: Read process memory failed. [5]");
		}

		DWORD offsetPESignature = *(WORD*)(buf + 0x3C);
		res = ReadProcessMemory(h, addr + offsetPESignature, buf, 0x30, &nb);

		if (!res || nb < 0x30)
		{
			assertion("Alpaca: Read process memory failed. [6]");
		}

		DWORD sizeOfCode = *(DWORD*)(buf + 0x1C);
		DWORD baseOfCode = *(DWORD*)(buf + 0x2C);

		memory = addr + baseOfCode + sizeOfCode - 200;
		if (!VirtualProtectEx(h, memory, 200, PAGE_EXECUTE_READWRITE, &oldProtect))
		{
			assertion("Alpaca: Failed to get memory pool in game thread");
		}
	}

	// Make memory data
	int len;
	pos = 0;

	// Dll name
	LPBYTE dllNameAddr = memory + pos;
	len = strlen(libraryName) + 1;
	res = WriteProcessMemory(h, dllNameAddr, libraryName, len, &nb);
	if (!res || (nb != len)) assertion("Alpaca: Write custom data in memory failed. [1]");
	pos += pos % 16 ? len + 16 - pos % 16 : len;

	// Init name
	LPBYTE initNameAddr = memory + pos;
	len = strlen(initFctName) + 1;
	res = WriteProcessMemory(h, initNameAddr, initFctName, len, &nb);
	if (!res || (nb != len)) assertion("Alpaca: Write custom data in memory failed. [2]");
	pos += pos % 16 ? len + 16 - pos % 16 : len;

	// Release name
	LPBYTE releaseNameAddr = memory + pos;
	len = strlen(releaseFctName) + 1;
	res = WriteProcessMemory(h, releaseNameAddr, releaseFctName, len, &nb);
	if (!res || (nb != len)) assertion("Alpaca: Write custom data in memory failed. [3]");
	pos += pos % 16 ? len + 16 - pos % 16 : len;

	// Load fct
	LPBYTE loadDllAddr = memory + pos;
	LPBYTE handleAddr = loadDllAddr + sizeof(loadDll) - 4;
	*(LPBYTE*)&loadDll[6] = loadLibraryAddr;
	*(LPBYTE*)&loadDll[12] = dllNameAddr;
	*(LPBYTE*)&loadDll[18] = loadLibraryAddr;
	*(LPBYTE*)&loadDll[23] = handleAddr;
	*(LPBYTE*)&loadDll[33] = initNameAddr;
	*(LPBYTE*)&loadDll[40] = getProcAddressAddr;
	*(LPBYTE*)&loadDll[63] = getProcAddressAddr;
	len = sizeof(loadDll);
	res = WriteProcessMemory(h, loadDllAddr, loadDll, len, &nb);
	if (!res || (nb != len)) assertion("Alpaca: Write custom data in memory failed. [4]");
	pos += pos % 16 ? len + 16 - pos % 16 : len;

	// Free fct
	LPBYTE freeDllAddr = memory + pos;
	*(LPBYTE*)&freeDll[6] = freeLibraryAddr;
	*(LPBYTE*)&freeDll[12] = handleAddr;
	*(LPBYTE*)&freeDll[22] = releaseNameAddr;
	*(LPBYTE*)&freeDll[29] = getProcAddressAddr;
	*(LPBYTE*)&freeDll[48] = getProcAddressAddr;
	*(LPBYTE*)&freeDll[60] = freeLibraryAddr;
	len = sizeof(freeDll);
	res = WriteProcessMemory(h, freeDllAddr, freeDll, len, &nb);
	if (!res || (nb != len)) assertion("Alpaca: Write custom data in memory failed. [5]");
	pos += pos % 16 ? len + 16 - pos % 16 : len;

	// Patch load library
	buf[0] = 0x90;
	buf[1] = 0xE8;
	*(DWORD*)(buf + 2) = (DWORD)loadDllAddr - (DWORD)loadCallerAddr - 6;
	len = 6;
	res = WriteProcessMemory(h, loadCallerAddr, buf, len, &nb);
	if (!res || (nb != len)) assertion("Alpaca: Write load library in memory failed");

	// Patch free library
	*(DWORD*)(buf + 2) = (DWORD)freeDllAddr - (DWORD)freeCallerAddr - 6;
	res = WriteProcessMemory(h, freeCallerAddr, buf, len, &nb);
	if (!res || (nb != len)) assertion("Alpaca: Write free library in memory failed");

	return true;
}

bool isD2gfxLoaded(HANDLE hProcess, LPVOID addr)
{
	const int BUFFER_SIZE = 768;

	SIZE_T nbRead;
	BYTE buf[BUFFER_SIZE];
	ReadProcessMemory(hProcess, addr, buf, BUFFER_SIZE, &nbRead);
	if (nbRead < 0x60) return false;
	int offsetPESignature = *(DWORD*)(buf + 0x3C);
	if (offsetPESignature + 0x5C >= BUFFER_SIZE) return false;
	DWORD ImageBase = *(DWORD*)(buf + offsetPESignature + 0x34);
	DWORD SizeOfImage = *(DWORD*)(buf + offsetPESignature + 0x50);
	DWORD CheckSum = *(DWORD*)(buf + offsetPESignature + 0x58);

	// [1.13d]
	if (ImageBase == 0x6FA80000 && SizeOfImage == 0x00021000 && CheckSum == 0x00018542) return true;
	return false;
}

bool launchGame(LPSTR commandLine, LPSTR currentDirectory, LPSTR libraryName)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	BOOL success = CreateProcess(0, commandLine, 0, 0, false, DEBUG_PROCESS, 0, currentDirectory, &si, &pi); //DEBUG_ONLY_THIS_PROCESS
	if (!success) return false;
	DEBUG_EVENT DebugEvent;
	DWORD status;
	while (WaitForDebugEvent(&DebugEvent, INFINITE))
	{
		status = DBG_CONTINUE;
		switch (DebugEvent.dwDebugEventCode)
		{
		case CREATE_THREAD_DEBUG_EVENT:
		case CREATE_PROCESS_DEBUG_EVENT:
			CloseHandle(DebugEvent.u.CreateThread.hThread);
			break;
		case EXIT_PROCESS_DEBUG_EVENT:
			exit(0);
		case EXCEPTION_DEBUG_EVENT:
			if (DebugEvent.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
			{
				MessageBox(0, "EXCEPTION_ACCESS_VIOLATION", "Alpaca", MB_OK | MB_ICONASTERISK);
			}
			break;
		case LOAD_DLL_DEBUG_EVENT:
			if (isD2gfxLoaded(pi.hProcess, DebugEvent.u.LoadDll.lpBaseOfDll))
			{
				InstallAlpaca(pi.hProcess, (LPBYTE)DebugEvent.u.LoadDll.lpBaseOfDll, libraryName);
				CloseHandle(DebugEvent.u.LoadDll.hFile);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
				debugActiveProcessStop(DebugEvent.dwProcessId);
				return true;
			}
			else
			{
				CloseHandle(DebugEvent.u.LoadDll.hFile);
			}
			break;
		}
		ContinueDebugEvent(DebugEvent.dwProcessId, DebugEvent.dwThreadId, status);
	}
	MessageBox(0, "ERROR : Alpaca isn't installed", "Alpaca", MB_OK | MB_ICONASTERISK);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return false;
}

int APIENTRY WinMain (
    __in HINSTANCE hInstance,
    __in_opt HINSTANCE hPrevInstance,
    __in_opt LPSTR lpCmdLine,
    __in int nShowCmd
    )
{
	//MessageBox(GetActiveWindow(), "The Alpacas have arrived!", "Alpaca", MB_APPLMODAL);

	char currentDirectory[MAX_PATH];
	Utility::GetAlpacaDirectory(currentDirectory);

	strcat(currentDirectory, "\\");

	char iniFileName[MAX_PATH + sizeof(INI_FILE_NAME) - 1];

	// Alpaca.ini Path
	strcpy(iniFileName, currentDirectory);
	strcat(iniFileName, INI_FILE_NAME);

	char command[MAX_PATH + sizeof(GAME_EXE_NAME) + 200];

	// Game.exe Path
	strcpy(command, currentDirectory);
	strcat(command, GAME_EXE_NAME);

	// Check that Game.exe is compatible with Alpaca. No point on
	// doing further processing if it isn't.
	VersionUtility::SetVersion(VersionUtility::GetVersion(command));

	if (!VersionUtility::IsSupported())
	{
		assertion("Alpaca isn't compatible with this version of Diablo II: %s", VersionUtility::GetVersionAsString());
		return 1;
	}

	if (GetFileAttributes(command) == INVALID_FILE_ATTRIBUTES)
	{
		assertion("There is an error with your path or Game.exe was not found: %s", command);
		return 1;
	}

	// Add params.
	int len = strlen(command);
	int paramLen = strlen(lpCmdLine);
	if (paramLen > 0)
	{
		len += 1 + paramLen;
		if (len > sizeof(command))
		{
			assertion(LONG_COMMAND);
		}
		strcat(command, " ");
		strcat(command, lpCmdLine);
	}
	len++;
	if (len >= sizeof(command))
	{
		assertion(LONG_COMMAND);
	}
	strcat(command, " ");
	GetPrivateProfileString(LAUNCHING_LABEL, PARAM_LABEL, NULL, command + len, sizeof(command) - len, iniFileName);

	// Install Alpaca and Launch Diablo II
	HMODULE module = GetModuleHandle("Kernel32.dll");
	if (module)
	{
		debugActiveProcessStop = (tDebugActiveProcessStop)GetProcAddress(module, "DebugActiveProcessStop");
		if (debugActiveProcessStop)
		{
			return !launchGame(command, currentDirectory, ALPACA_DLL_NAME);
		}
	}

	assertion("The game was attempted to be launched, no errors occurred, but was unable to be launched. Exiting.");
}