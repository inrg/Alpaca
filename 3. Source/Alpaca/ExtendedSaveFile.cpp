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

#include "ExtendedSaveFile.h"
#include "InfinityStash.h"
#include "Common.h"
#include <stdio.h>

#define BUFFER_SIZE 0x4000
#define FILE_VERSION 0x3130			//"01"
#define FILE_EXTENDED 0x4D545343	//"CSTM"

BYTE * readExtendedSaveFile(char* name, DWORD* size)//WORKS
{
	char filename[512];
	BYTE* data;
	d2_assert(!name, "Bad file name (NULL)", __FILE__, __LINE__);
	D2FogGetSavePath( filename, 512-5);
	strncat(filename, name, 512 - strlen(filename) - 5);
	strcat(filename, ".d2x");

	log_msg("Extended file to read : %s\n",filename);

	FILE* file = fopen(filename, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		*size = ftell(file);
		fseek(file, 0, SEEK_SET);
		data = (BYTE*)D2FogMemAlloc(*size,__FILE__,__LINE__,0);
		DWORD nbRead = fread(data, 1, *size, file);
		fclose(file);
		d2_assert(nbRead != *size , "nbRead from extented save file != size", __FILE__, __LINE__);
	} else {
		log_msg("Can't open extented save file in mode \"rb\" (is not an error if it's a new player)\n");
		DWORD maxSize = 100;
		data = (BYTE*)D2FogMemAlloc(maxSize,__FILE__,__LINE__,0);
		*size = 14;
		*((DWORD*)&data[0])  = FILE_EXTENDED; //"CSTM"
		*((WORD *)&data[4])  = FILE_VERSION;
		*((DWORD*)&data[10]) = 0;// number of stash
	}
	return data;
}

int loadExtendedSaveFile(Unit* ptChar, BYTE data[], DWORD maxSize)
{
	if (!ptChar || !PCPY || !data) return 0;

	log_msg("Load extended file\n");

	DWORD curSize = 0;

	if (*(DWORD*)&data[curSize] != FILE_EXTENDED)
	{
		log_msg("loadExtendedSaveFile -> bad header\n");
		return 9;
	}
	curSize += 4;

	if (*(WORD *)&data[curSize] != FILE_VERSION)
	{
		log_msg("loadExtendedSaveFile -> bad file version\n");
		return 9;
	}
	curSize += 2;
	curSize += 1;
	curSize += 3;

	int ret = loadStashList(ptChar, data, maxSize, &curSize, false);

	PCPY->selfStashIsOpened = true;
	return ret;
}

void writeExtendedSaveFile(char* name, BYTE* data, DWORD size)
{
	char szTempName[MAX_PATH];
	char szSaveName[MAX_PATH];

	//Get temporary savefile name.
	D2FogGetSavePath(szTempName, MAX_PATH);
	strcat(szTempName, name);
	strcat(szTempName, ".d2~");
	log_msg("Extended temporary file for saving : %s\n",szTempName);

	//Write data in savefile.
	FILE* customSaveFile = fopen(szTempName, "wb+");
	fwrite(data, size, 1, customSaveFile);
	fclose(customSaveFile);

	//Get real savefile name.
	D2FogGetSavePath(szSaveName, MAX_PATH);
	strcat(szSaveName, name);
	strcat(szSaveName, ".d2x");
	log_msg("Extended file for saving : %s\n",szSaveName);

	DeleteFile(szSaveName);
	if (!MoveFile(szTempName, szSaveName))
		log_box("Could not create the extended save file.");
}

void saveExtendedSaveFile(Unit* ptChar, BYTE** data, DWORD* maxSize, DWORD* curSize)
{
	*(DWORD *)(*data + *curSize) = FILE_EXTENDED; //"CSTM"
	*curSize += 4;
	*(WORD *)(*data + *curSize) = FILE_VERSION;
	*curSize += 2;
	*curSize += 4;

	saveStashList(ptChar, PCPY->selfStash, data, maxSize, curSize);
}