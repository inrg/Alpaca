// Copyright (C) 1999-2004 Ultrafunk <info@ultrafunk.com>
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

#define INIFILE_READ		10
#define INIFILE_WRITE		20
#define INIFILE_MPQREAD		30

#define MAX_SECTIONNAME_LENGTH	128
#define MAX_KEYNAME_LENGTH		128

class INIFile
{
public:
	INIFile();
	~INIFile();

	BOOL InitReadWrite(const char *path, int readWrite, DWORD writeCacheSize);
	BOOL close();
	int GetPrivateProfileString(const char *section, const char *key, const char *def, char *dest, DWORD size);
	BOOL WritePrivateProfileString(char *section, char *key, char *string);

private:
	char	m_currentSection[MAX_SECTIONNAME_LENGTH];
	char   *m_cache;
	char   *m_sectionStart;
	char   *m_sectionEnd;
	int		m_readWrite;
	char	m_path[_MAX_PATH];
	HANDLE  m_file;
	DWORD	m_cacheWritePos;
};

class INIFileW
{
public:
	INIFileW();
	~INIFileW();

	BOOL InitReadWrite(const char *path, int readWrite, DWORD writeCacheSize);
	BOOL close();
	int GetPrivateProfileString(LPCWSTR section, LPCWSTR key, LPCWSTR def, LPWSTR dest, DWORD size);
	int GetPrivateProfileStringList(LPCWSTR section, LPCWSTR key, DWORD** codes, LPWSTR** values);
	LPWSTR m_cache;

private:
	WCHAR m_currentSection[MAX_SECTIONNAME_LENGTH];
	LPCWSTR m_sectionStart;
	LPCWSTR m_sectionEnd;
	int		m_readWrite;
	char	m_path[_MAX_PATH];
	HANDLE  m_file;
	DWORD	m_cacheWritePos;
};
