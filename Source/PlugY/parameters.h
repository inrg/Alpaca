/*=================================================================
	File created by Yohann NICOLAS.

  Loading parameters from ini file.

=================================================================*/
#pragma once

#define GARDENIA_NAME "Gardenia"
#define GARDENIA_VERSION "0.01"

enum TargetMod
{
	MOD_NO = 0,
};

// Global variable (user parameters)
extern char* modDataDirectory;
extern bool active_plugin;
extern bool active_DisableBattleNet;
extern bool active_CheckMemory;
extern char* dllFilenames;
extern bool active_D2Mod;
extern TargetMod selectModParam;

void loadParameters();