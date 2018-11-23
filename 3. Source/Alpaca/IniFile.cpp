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

// Description: Memory cached INI file read/write class to replace legacy MS code
#include "IniFile.h"
#include "Common.h"
#include <stdio.h>

// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/sysinfo/base/getprivateprofilestring.asp

/*****************************************************************************/
// Utils

LPSTR strstri(LPSTR text, LPSTR string)
{
	if(text && string)
	{
		int len = strlen(string);
		if(len)
		{
			while(*text)
			{
				if(_strnicmp(string, text, len) == 0)
				{
					return text;
					break;
				}

				text++;
			}
		}
	}
	return NULL;
}

LPCWSTR strstri(LPCWSTR text, LPCWSTR string)
{
	if (text && string)
	{
		int len = wcslen(string);
		if (len)
		{
			while (*text)
			{
				if (_wcsnicmp(string, text, len) == 0)
				{
					return text;
					break;
				}

				text++;
			}
		}
	}
	return NULL;
}

BOOL chrcmp(char c, char *string)
{
	for(unsigned int i=0; i<strlen(string); i++)
	{
		if(c == string[i])
			return TRUE;
	}

	return FALSE;
}


BOOL FileExists(char *name)
{
	DWORD attr = GetFileAttributes(name);
	return ( (attr != (DWORD)-1) && !(attr & FILE_ATTRIBUTE_DIRECTORY) );
}


/*****************************************************************************/
/*****************************************************************************/

INIFile::INIFile()
{
	m_readWrite		= 0;
	m_file			= NULL;
	m_cacheWritePos	= 0;
	m_cache			= NULL;
	m_sectionStart	= NULL;
	m_sectionEnd	= NULL;
	 
	ZeroMemory((void *)m_path, sizeof(m_path));
	ZeroMemory((void *)m_currentSection, sizeof(m_currentSection));
}


INIFile::~INIFile()
{
	if(m_file)
		CloseHandle(m_file);

	if(m_cache)
		delete [] m_cache;
}


BOOL INIFile::InitReadWrite(const char *path, int readWrite, DWORD writeCacheSize)
{
	if (!path)
		return false;

	m_readWrite		= readWrite;
	m_cacheWritePos	= 0;
	m_sectionStart	= NULL;
	m_sectionEnd	= NULL;

	ZeroMemory((void *)m_path, sizeof(m_path));
	ZeroMemory((void *)m_currentSection, sizeof(m_currentSection));

	strncpy(m_path, path, sizeof(m_path));

	if(m_file)
	{
		CloseHandle(m_file);
		m_file = NULL;
	}

	if(m_cache)
	{
		delete [] m_cache;
		m_cache	= NULL;
	}

	// If read, the open file and cache content in memory
	if(m_readWrite == INIFILE_MPQREAD)
	{
		void* refFile;
		if ( D2MPQOpenFile(m_path,&refFile) )
		{
			DWORD fileSize = D2MPQGetSizeFile(refFile, NULL);
			m_cache = new char[fileSize + 1];
			if(m_cache)
			{
				DWORD read;
				if(D2MPQReadFile(refFile, (BYTE*)m_cache, fileSize, &read, NULL, NULL, NULL) == 0)
				{
					delete [] m_cache;
					m_cache = NULL;
				}
				else
					m_cache[fileSize] = 0;
			}
			D2MPQCloseFile(refFile);
			if(m_cache)
				return TRUE;
		}
	}
		
//		if((m_readWrite == INIFILE_MPQREAD) || (m_readWrite == INIFILE_READ))
	if(m_readWrite == INIFILE_READ)
	{
		if(FileExists(m_path))
		{
			m_file = CreateFile(m_path,
								GENERIC_READ,
								0,
								NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL);

			if(m_file != INVALID_HANDLE_VALUE)
			{
				DWORD fileSize = GetFileSize(m_file, NULL);

				m_cache = new char[fileSize + 1];
				if(m_cache)
				{
					DWORD read;

					if(ReadFile(m_file, m_cache, fileSize, &read, NULL) == 0)
					{
						delete [] m_cache;
						m_cache = NULL;
					}
					else
						m_cache[fileSize] = 0;
				}

				CloseHandle(m_file);
				m_file = NULL;

				if(m_cache)
					return TRUE;
			}
		}
	}

	// If write, then create write cache memory buffer
	if(m_readWrite == INIFILE_WRITE)
	{
		m_cache = new char[writeCacheSize];

		if(m_cache)
			return TRUE;
	}

	return FALSE;
}


BOOL INIFile::close()
{
	BOOL success = FALSE;

	// If read, then kill the memory cached settings
	if(m_readWrite == INIFILE_READ)
	{
		// Just to be sure, close the settings file again if it's still open for some reason
		if(m_file)
		{
			CloseHandle(m_file);
			m_file = NULL;
		}

		if(m_cache)
		{
			delete [] m_cache;
			m_cache	= NULL;

			success = TRUE;
		}

		// Just to be sure, reset section position markers
		m_sectionStart = NULL;
		m_sectionEnd   = NULL;
	}

	// If write, then commit (write) cached settings file to disk
	if(m_readWrite == INIFILE_WRITE)
	{
		if(m_path && m_cache)
		{
			m_file = CreateFile(m_path,
								GENERIC_WRITE,
								0,
								NULL,
								CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL,
								NULL);

			if(m_file != INVALID_HANDLE_VALUE)
			{
				DWORD written;
				WriteFile(m_file, m_cache, m_cacheWritePos, &written, NULL);

				CloseHandle(m_file);
				m_file = NULL;

				if(written == m_cacheWritePos)
					success = TRUE;
			}

			delete [] m_cache;
			m_cache	= NULL;
		}
	}

	return success;
}


int INIFile::GetPrivateProfileString(const char *section, const char *key, const char *def, char *dest, DWORD size)
{
	DWORD copied = 0;

	if((m_cache) && (section && key && dest && size))
	{
		if(_stricmp(section, m_currentSection) != 0)
		{
			strncpy(m_currentSection, section, MAX_SECTIONNAME_LENGTH);

			char headerString[MAX_SECTIONNAME_LENGTH];
			ZeroMemory((void *)headerString, sizeof(headerString));
//			_snprintf(headerString, MAX_SECTIONNAME_LENGTH, "[%s]\r\n", section);
			_snprintf(headerString, MAX_SECTIONNAME_LENGTH, "[%s]", section);

			// Locate section start in buffer
			m_sectionStart = strstri(m_cache, headerString);

			// We found the start section
			if(m_sectionStart)
			{
				// Keys start after the "[" + section + "]" header
				m_sectionStart += strlen(section) + 2;

				// Locate section end in buffer
				m_sectionEnd = strstr(m_sectionStart, "\r\n[");

				// If no next section is found, then use end of buffer
				if(m_sectionEnd == NULL)
					m_sectionEnd = m_cache + strlen(m_cache);
			}
		}

		if(m_sectionStart && m_sectionEnd)
		{
			char keyString[MAX_KEYNAME_LENGTH];
			ZeroMemory((void *)keyString, sizeof(keyString));
			_snprintf(keyString, MAX_KEYNAME_LENGTH, "\r\n%s=", key);

			if(char *s = strstri(m_sectionStart, keyString))
			{
				if(s < m_sectionEnd)
				{
					s += strlen(keyString);

					while(*s && copied < size && *s != '\n' && *s != '\r' && *s != NULL)
					{
						*dest++ = *s++;
						copied++;
					}

					*dest = 0;
					copied++;
				}
			}
		}
	}

	if (!copied && def)
	{
		strncpy(dest, def, size);
		return strlen(dest);
	}

	return copied; // Do not include the terminating null character
}


BOOL INIFile::WritePrivateProfileString(char *section, char *key, char *string)
{
	if(!section || !key || !string) return false;

	if(_stricmp(section, m_currentSection) != 0)
	{
		if(m_cacheWritePos == 0)
			m_cacheWritePos += sprintf((m_cache + m_cacheWritePos), "[%s]\r\n", section);
		else
			m_cacheWritePos += sprintf((m_cache + m_cacheWritePos), "\r\n[%s]\r\n", section);

		strncpy(m_currentSection, section, MAX_SECTIONNAME_LENGTH);
	}
	m_cacheWritePos += sprintf((m_cache + m_cacheWritePos), "%s=%s\r\n", key, string);
	return true;
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

INIFileW::INIFileW()
{
	m_readWrite = 0;
	m_file = NULL;
	m_cacheWritePos = 0;
	m_cache = NULL;
	m_sectionStart = NULL;
	m_sectionEnd = NULL;

	ZeroMemory((void *)m_path, sizeof(m_path));
	ZeroMemory((void *)m_currentSection, sizeof(m_currentSection));
}

INIFileW::~INIFileW()
{
	if (m_file)
		CloseHandle(m_file);

	if (m_cache)
		delete[] m_cache;
}

BOOL INIFileW::InitReadWrite(const char *path, int readWrite, DWORD writeCacheSize)
{
	if (!path)
		return false;

	m_readWrite = readWrite;
	m_cacheWritePos = 0;
	m_sectionStart = NULL;
	m_sectionEnd = NULL;

	ZeroMemory((void *)m_path, sizeof(m_path));
	ZeroMemory((void *)m_currentSection, sizeof(m_currentSection));

	strncpy(m_path, path, sizeof(m_path));

	if (m_file)
	{
		CloseHandle(m_file);
		m_file = NULL;
	}

	if (m_cache)
	{
		delete[] m_cache;
		m_cache = NULL;
	}

	// If read, the open file and cache content in memory
	if (m_readWrite == INIFILE_MPQREAD)
	{
		void* refFile;
		if (D2MPQOpenFile(m_path, &refFile))
		{
			DWORD fileSize = D2MPQGetSizeFile(refFile, NULL);
			m_cache = new WCHAR[fileSize + 1];
			if (m_cache)
			{
				DWORD read;
				if (D2MPQReadFile(refFile, (BYTE*)m_cache, fileSize, &read, NULL, NULL, NULL) == 0)
				{
					delete[] m_cache;
					m_cache = NULL;
				}
				else
					m_cache[fileSize] = 0;
			}
			D2MPQCloseFile(refFile);
			if (m_cache)
				return TRUE;
		}
	}

	//		if((m_readWrite == INIFILE_MPQREAD) || (m_readWrite == INIFILE_READ))
	if (m_readWrite == INIFILE_READ)
	{
		if (FileExists(m_path))
		{
			m_file = CreateFile(m_path,
				GENERIC_READ,
				0,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

			if (m_file != INVALID_HANDLE_VALUE)
			{
				DWORD fileSize = GetFileSize(m_file, NULL);

				m_cache = new WCHAR[fileSize + 1];
				if (m_cache)
				{
					DWORD read;

					if (ReadFile(m_file, m_cache, fileSize, &read, NULL) == 0)
					{
						delete[] m_cache;
						m_cache = NULL;
					}
					else
						m_cache[fileSize] = 0;
				}

				CloseHandle(m_file);
				m_file = NULL;

				if (m_cache)
					return TRUE;
			}
		}

		// If write, then create write cache memory buffer
		if (m_readWrite == INIFILE_WRITE)
		{
			m_cache = new WCHAR[writeCacheSize];

			if (m_cache)
				return TRUE;
		}
	}

	return FALSE;
}

BOOL INIFileW::close()
{
	BOOL success = FALSE;

	// If read, then kill the memory cached settings
	if (m_readWrite == INIFILE_READ)
	{
		// Just to be sure, close the settings file again if it's still open for some reason
		if (m_file)
		{
			CloseHandle(m_file);
			m_file = NULL;
		}

		if (m_cache)
		{
			delete[] m_cache;
			m_cache = NULL;

			success = TRUE;
		}

		// Just to be sure, reset section position markers
		m_sectionStart = NULL;
		m_sectionEnd = NULL;
	}

	// If write, then commit (write) cached settings file to disk
	if (m_readWrite == INIFILE_WRITE)
	{
		if (m_path && m_cache)
		{
			m_file = CreateFile(m_path,
				GENERIC_WRITE,
				0,
				NULL,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

			if (m_file != INVALID_HANDLE_VALUE)
			{
				DWORD written;
				WriteFile(m_file, m_cache, m_cacheWritePos, &written, NULL);

				CloseHandle(m_file);
				m_file = NULL;

				if (written == m_cacheWritePos)
					success = TRUE;
			}

			delete[] m_cache;
			m_cache = NULL;
		}
	}

	return success;
}


int INIFileW::GetPrivateProfileString(LPCWSTR section, LPCWSTR key, LPCWSTR def, LPWSTR dest, DWORD size)
{
	DWORD copied = 0;

	if ((m_cache) && (section && key && dest && size))
	{
		if (_wcsicmp(section, m_currentSection) != 0)
		{
			wcsncpy(m_currentSection, section, MAX_SECTIONNAME_LENGTH);

			WCHAR headerString[MAX_SECTIONNAME_LENGTH];
			ZeroMemory((void *)headerString, sizeof(headerString));
			//			_snprintf(headerString, MAX_SECTIONNAME_LENGTH, "[%s]\r\n", section);
			_snwprintf(headerString, MAX_SECTIONNAME_LENGTH, L"[%s]", section);

			// Locate section start in buffer
			m_sectionStart = strstri(m_cache, headerString);

			// We found the start section
			if (m_sectionStart)
			{
				// Keys start after the "[" + section + "]" header
				m_sectionStart += wcslen(section) + 2;
				// Locate section end in buffer
				m_sectionEnd = wcsstr(m_sectionStart, L"\r\n[");
				// If no next section is found, then use end of buffer
				if (m_sectionEnd == NULL)
					m_sectionEnd = m_cache + wcslen(m_cache);
			}
		}

		if (m_sectionStart && m_sectionEnd)
		{
			WCHAR keyString[MAX_KEYNAME_LENGTH];
			ZeroMemory((void *)keyString, sizeof(keyString));
			_snwprintf(keyString, MAX_KEYNAME_LENGTH, L"\r\n%s=", key);

			if (LPCWSTR s = strstri(m_sectionStart, keyString))
			{
				if (s < m_sectionEnd)
				{
					s += wcslen(keyString);

					while (*s && copied < size && *s != '\n' && *s != '\r' && *s != NULL)
					{
						*dest++ = *s++;
						copied++;
					}

					*dest = 0;
					copied++;
				}
			}
		}
	}

	if (!copied && def)
	{
		wcsncpy(dest, def, size);
		return wcslen(dest);
	}

	return copied; // Do not include the terminating null character
}


