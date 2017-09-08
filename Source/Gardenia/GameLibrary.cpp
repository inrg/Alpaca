// Copyright (C) 2004-2017 Yohann Nicolas
// Copyright (C) 2017 L'Autour
// Copyright (C) 2017 Jonathan Vasquez <jon@xyinn.org>
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

#include "GameLibrary.h"

GameLibrary::GameLibrary() : Library()
{
	Name = "Game.exe";
	Offset = (DWORD)GetModuleHandle(NULL);
	Version = VersionUtility::GetVersion(Name);
	log_msg("Game.exe loaded at:\t%08X (%s)\n", Offset, VersionUtility::GetVersionAsString(Version));
}

void GameLibrary::SetFunctions()
{

}

bool GameLibrary::IsSupported()
{
	return Version == VersionUtility::Versions::V109b || Version == VersionUtility::Versions::V113d;
}