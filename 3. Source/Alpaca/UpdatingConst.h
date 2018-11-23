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

enum UpdateClientConst
{
	UC_SELECT_STASH=0x18,
	UC_SHARED_GOLD,
	UC_PAGE_NAME
};

enum UpdateServerConst
{
	US_PLACEHOLDER = 0x10, // Don't change these first 8 value
	US_SELECT_PREVIOUS,
	US_SELECT_NEXT,
	US_SELECT_SELF,
	US_SELECT_SHARED,
	US_SELECT_PREVIOUS_INDEX,
	US_SELECT_NEXT_INDEX,
	US_SELECT_PREVIOUS2,
	US_SELECT_NEXT2,
	US_SELECT_PREVIOUS_INDEX2,
	US_SELECT_NEXT_INDEX2,
	US_SAVE,
	US_MAXGOLD,
	US_PUTGOLD,
	US_TAKEGOLD,
	US_PAGENAME,
	US_SET_INDEX,
	US_SET_MAIN_INDEX,
	US_UNSET_INDEX,
	US_INSERT_PAGE,
	US_DELETE_PAGE,
	US_SELECT_PAGE3,
	US_SELECT_PAGE2,
	US_SELECT_PAGE1,
	US_SELECT_PAGE,
	US_SWAP3,
	US_SWAP2,
	US_SWAP1,
	US_SWAP0,
	US_SWAP0_TOGGLE
};

