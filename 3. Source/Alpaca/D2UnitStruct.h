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

#include <windows.h>

struct CBPlayerData;
struct CBItemData;
struct Unit;
struct SkillsBIN;
struct SkillData;
struct ObjectsBIN;

/*=================================================================*/
/*  Skill Structure.                                               */
/*=================================================================*/

struct SkillData
{								//Offset from Code. (size = 0x3C or 0x40)
	SkillsBIN*	ptSkillBin;		//+00
	SkillData*	ptNextSkill;	//+04
	DWORD	mode;           //+08
	DWORD	uk1[3];         //+0C
	DWORD	targetInfo;     //+18
	DWORD	targetType;     //+1c
	DWORD	targetUNID;     //+20
	DWORD	uk2;            //+24
	DWORD	slvl;           //+28
	DWORD	slvlBonus;      //+2c
	DWORD	uk3;            //+30
	int		state;          //+34 (-1 if index exist)
	DWORD	param1;			//+38 (nb current charge)
	DWORD	param2;			//+3C
};

struct Skills
{								//Offset from Code.
	DWORD		gameRelated;	//+00
	SkillData*	ptFirstSkill;	//+04
	SkillData*	ptLeftSkill;	//+08
	SkillData*	ptRightSkill;	//+0c
    SkillData*	ptCurrentSkill;	//+10
};

/*=================================================================*/
/*   Unit Data Structure.                                          */
/*=================================================================*/

struct Position
{
	DWORD x;
	DWORD y;
};

struct Room//size=0x80
{
//ptRoom +48 0 = spawn new units (monster, objects e.tc), 1 = don't spawn any new units
	DWORD	seed1;						//+00
	DWORD	seed2;						//+04
	BYTE	uk8[0x1C];					//+08
	Room*	ptNearRooms;				//+24
	DWORD	nbNearRooms;				//+28
	Unit*	unit;						//+2C
	BYTE	uk5[0x44];					//+30
	Room*	ptNextRoom;					//+74
	union
	{
		BYTE	flags;					//+78
		struct
		{
			BYTE isGenerated:1;
			BYTE isGenerated2:1;//???
		};
	};
};

struct ActMap//ptGame+BC size=0x60
{
	DWORD	isNotManaged;
	DWORD	uk4;
	DWORD	uk8;//size = 0x488
	Room*	ptFirstRoom;
};

struct NetClient;

struct GameStruct
{                              			//Offset from Code.
	BYTE	uk1[0x18];					//+00
	DWORD	_ptLock;					//+18 Unknown  
	DWORD	memoryPool;					//+1C Memory Pool (??)  
	BYTE	uk2[0x4D];					//+20
	BYTE	difficultyLevel;			//+6D (Difficulty 0,1 or 2)
	WORD	unknown1;					//+6E Cube puts 4 here
	DWORD	isLODGame;					//+70 (D2=0 LOD =1) (DWORD ?)
	BYTE	uk3[0x04];					//+71
	WORD	unknown2;					//+78
	BYTE	uk4[0x0E];					//+7A
	NetClient*	ptClient;				//+88
	BYTE	uk8C[0x30];					//+8C
	ActMap*	mapAct[5];					//+BC
	BYTE	ukD0[0x1024];				//+D0
	DWORD*	game10F4;					//+10F4
	BYTE	uk6[0x28];					//+10F8
	Unit*	units[0xA00];				//+1120
	Unit*	roomtitles[0x200];			//+1B20
};

struct Path //(8 dword)
{                              			//Offset from Code.		Size: 20
	WORD	uk1;						//+00
	WORD	mapx;						//+02
	WORD	uk2;						//+04
	WORD	mapy;						//+06
	DWORD	uk3;						//+08
	DWORD	x;							//+0C
	DWORD	y;							//+10
	DWORD	uk6;						//+14
	DWORD	uk7;						//+18
	Room*	ptRoom;						//+1C
};

struct Inventory
{                               		//Offset from Code.		Size: 30 à 40
	DWORD	tag;						//+00	= 0x01020304
	BYTE	uk1[0x04];					//+04	=? 0
	Unit*	ptChar;						//+08
	Unit*	ptItem;						//+0C
	BYTE	uk2[0x10];					//+10
	Unit*	inventory1;					//+20
	BYTE	uk3[0x04];					//+24
	DWORD	currentUsedSocket;			//+28 //Kingpin : a variable to know how many sockets that have added to item
	DWORD	Inventory2C;				//+2C //one value
	DWORD	Inventory30;				//+30
	void*	ptCorpse;					//+34
	BYTE	uk5[0x04];					//+38
	DWORD	nextCorpseId;				//+3C //max = 15
	BYTE	uk6[0x04];					//+40
};

struct D2Stat
{
	WORD	index;
	WORD	id;
	int		value;
};

struct Stats				//sizeof(Stats)=0x64
{
	DWORD	nUnitId;				//+00
	Unit*	ptUnit;					//+04
	DWORD	nUnitType;				//+08
	DWORD	nItemNum;				//+0C
	union
	{
		DWORD	flags;				//+10
		struct
		{
			DWORD fuk1:13;			//0x00001FFF
			DWORD isDisabled:1;		//0x00002000
			DWORD fuk2:17;			//0x7FFFC000
			DWORD dontUseBaseValue:1;//0x80000000
		};
	};
	DWORD	id;						//+14
	DWORD	uk18;					//+18
	BYTE	uk2[0x08];				//+1C
	D2Stat*	ptBaseStatsTable;		//+24
	WORD	nbBaseStats;			//+28
	WORD	sizeOfBaseStatsTable;	//+2A ??
	Stats*	ptStats;				//+2C
	BYTE	uk3[0x04];				//+30
	Stats*	ptItemStats;			//+34
	BYTE	uk4[0x04];				//+38
	Stats*	ptAffixStats;			//+3C
	Stats*	ptNextStats2;			//+40
	union
	{
		Unit*	ptChar;				//+44
		Unit*	ptItem;
	};
	D2Stat*	ptStatsTable;			//+48
	WORD	nbStats;				//+4C
	WORD	sizeOfStatsTable;		//+4E ??
	BYTE	uk5[0x8];				//+50
	BYTE*	unknow0;				//+58 (sizeof(*unknow0)=0x30 (calculated)
	DWORD	unknow1;				//+5C (=0)
	DWORD	unknow2;				//+60 (=0)
};

struct MonsterData					// sizeof(MonsterData)=0x60
{										//Offset from Code.
    BYTE	uk[0x16];					//+00
 	union {
		WORD	flags;					//+16
		struct {
			WORD	fuk1:1;
			WORD	isSuperUnique:1;
			WORD	isChampion:1;
			WORD	isUnique:1;
			WORD	fuk2:13;
		};
	};
   BYTE		uk1[0x0E];					//+18
   WORD		superUniqueID;				//+26
   void*	unknow1;					//+28
   BYTE		uk2[0x38];					//+28
};

struct ObjectData
{
	ObjectsBIN*	ptObjectsBIN;
	BYTE		levelID;
};

#define F2_ITEM_IS_IDENTIFIED 0x10 //or F_IS_IDENTIFIED ?

struct ItemData//size=0x74
{										//Offset from Code.
	DWORD	quality;					//+00
	DWORD	seedLow;					//+04
	DWORD	seedHi;						//+08
	DWORD	playerID;					//+0C #10734 / #10735 (PCInventory->ptPlayer->0C)
	DWORD	seedStarting;				//+10
	DWORD	flags1;						//+14
	union {
		DWORD	flags2;					//+18
		struct {
			DWORD	fuk1:1;				//0x00000001
			DWORD	isIndentified:1;	//0x00000002
			DWORD	fuk2:2;				//0x0000000C
			DWORD	isUnindentified:1;	//0x00000010
			DWORD	fuk3:3;				//0x000000E0
			DWORD	isBroken:1;			//0x00000100
			DWORD	fuk4:2;				//0x00000600
			DWORD	isSocketed:1;		//0x00000800
			DWORD	fuk5:10;			//0x003FF000
			DWORD	isEtheral:1;		//0x00400000
			DWORD	fuk6:3;				//0x03800000
			DWORD	isRuneword:1;		//0x04000000
			DWORD	fuk7:1;				//0x08000000
			DWORD	isPersonalized:1;	//0x10000000
			DWORD	fuk8:3;				//0xE0000000
		};
	};

	DWORD	guid1;						//+1C Global Unique ID 1  
	DWORD	guid2;						//+20 Global Unique ID 2  
	DWORD	guid3;						//+24 Global Unique ID 3  
	DWORD	uniqueID;					//+28
	BYTE	ilvl;						//+2C
	BYTE	uk1[0x03];					//+2D
	WORD	version;					//+30
	WORD	rarePrefix;					//+32
	WORD	rareSuffix;					//+34
	WORD	autoPref;					//+36
	WORD	prefix[3];					//+38
	WORD	suffix[3];					//+3E
	BYTE	equipLoc;					//+44
    /*	emplacement si équipé
	*	00 = noequip/inBelt
	*   01 = head
	*	02 = neck
	*	03 = tors
	*	04 = rarm
	*	05 = larm
	*	06 = lrin
	*	07 = rrin
	*	08 = belt
	*	09 = feet
	*	0A = glov
	*	0B = ralt
	*	0C = lalt
*/
	BYTE	page;						//+45
    /*	page dans laquel se trouve l'item
    *	FF = mouse/equip/onEarth
    *	00 = inventory
    *   01 = cube
    *	04 = stash
    */
	BYTE	uk4[0x01];					//+46
	BYTE	ItemData3;					//+47 //D2Common10854 D2Common10853
	BYTE	pEarLevel;					//+48
	BYTE	varGfx;						//+49
	char	IName[0x12];				//+4A //inscribed/ear name  
	Inventory*	ptInventory;			//+5C
	Unit*	ptPrevItem;					//+60
	Unit*	ptNextItem;					//+64
	BYTE	uk8[0x01];					//+68
	BYTE	ItemData2;					//+69
	BYTE	uk9[0x0A];					//+6A
};

struct NetClient
{
	DWORD	clientID;					//+00
	BYTE	uk1[0x06];					//+04
	union {								//+0A
		WORD	flag;
		struct {
			WORD f1:1;
			WORD f2:1;
			WORD isHardCoreGame:1;
		};
	};
	BYTE	uk2;						//+0C
	char	name[0x10];					//+0D
	BYTE	uk3[0x15F];					//+1D
	BYTE*	savefile;					//+17C
	DWORD	finalSize;					//+180
	DWORD	counter;					//+184
	DWORD	currentSize;				//+188
	BYTE	uk4[0x1C];					//+18C
	GameStruct*	ptGame;						//+1A8
};

struct PlayerData   
{  
	char		name[0x10];				//+00	Player Name  
	void*		ptQuest[3];				//+10	Quest Pointers for each difficulty  
	BYTE		uk1[0x18];				//+1C		//before : 0x14
	void*		ptArenaUnit;			//+34	ptArena for the Unit  
	BYTE		uk2[0x4];				//+38		//before : 0x7
	WORD		MPSourcePortalUniqueID;	//+3C	Source Portal Unique_ID  
	BYTE		uk3[0x2];				//+3E
	WORD		MPDestPortalUniqueID;	//+40	Destination Portal Unique_ID  
	BYTE		uk4[0x06];				//+42  
	BYTE		ptObjectUnID;			//+48	Object UniqueID for TownPortals       
	BYTE		uk5[0x53];				//+49  
	NetClient*	ptNetClient;			//+9C	ptClient  
};

#define F1_IS_SPAWNED 0x10 //or F_IS_IDENTIFIED ?
#define F2_IS_LOD 0x2000000
struct Unit
{										//Offset from Code.		Size: 0xF4+4
	DWORD		nUnitType;				//+00
	union{
		DWORD			nPlayerClass;
		DWORD			nTxtFileNo;         
    };									//+04
	DWORD		nUnitId;				//+08
	DWORD		nItemNum;				//+0C
	DWORD		CurrentAnim;			//+10
	union{
		MonsterData*	ptMonsterData;
		ObjectData*		ptObjectData;
		ItemData*		ptItemData;
		PlayerData*		ptPlayerData;
	};									//+14
	BYTE		act;					//+18
	BYTE		uk12[3];				//+19
	void*		ptAct;					//+1C
	DWORD		SeedLow;				//+20
	DWORD		SeedHi;					//+24
	DWORD		SeedStarting;			//+28
	Path*		path;					//+2C
	BYTE		uk1[0x08];				//+30
	Path*		ptPos;					//+38
	DWORD		animSpeed;				//+3C
	BYTE		uk2[0x04];				//+40
	DWORD		curFrame;				//+44
	DWORD		remainingFrame;			//+48
	WORD		animSpeedW;				//+4C
	BYTE		actionFlag;				//+4E
	BYTE		uk3[0x1];				//+4F
	void*		ptAnimData;				//+50
	BYTE		uk4[0x08];				//+54
	Stats*		ptStats;				//+5C
	Inventory*	ptInventory;		    //+60
	BYTE		uk5[0x1C];				//+64
	GameStruct*		ptGame;					//+80
	BYTE		uk6[0x24];				//+84
	Skills*		ptSkills;				//+A8
	void*		ptCombatData;			//+AC
	BYTE		uk7[0x08];				//+B0
	DWORD		itemCode;				//+B8 for an item being dropped by this unit
	BYTE		uk8[0x08];				//+BC
	DWORD		flags1;					//+C4
	union{
		DWORD		flags2;				//+C8	//flags
		struct{
			DWORD	ukf1:25;
			DWORD	isLod:1;
		};
	};
	BYTE		uk9[0x04];				//+CC
	DWORD		clientId;				//+D0
	BYTE		uk10[0x0C];				//+D4
	Unit*		ptFirstMonster;			//+E0
	Unit*		Unit1;					//+E4
	Unit*		Unit2;					//+E8
	BYTE		uk11[0x08];				//+EC
	union{
		CBPlayerData*	ptCBPlayerData;
		CBItemData*		ptCBItemData;
		void*			ptCBData;
	};									//+F4
};