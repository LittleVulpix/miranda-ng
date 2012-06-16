/*
Version information plugin for Miranda IM

Copyright � 2002-2006 Luca Santarelli, Cristian Libotean

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/


#ifndef CPLUGIN_H
#define CPLUGIN_H

//#define STRICT
#define WIN32_LEAN_AND_MEAN

#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <string>

#ifndef M_NEWPLUGINAPI_H__
	#include "newpluginapi.h"
#endif

#define DEF_UUID_CHARMARK "�"

extern const int cPLUGIN_UUID_MARK;
extern TCHAR PLUGIN_UUID_MARK[];

//using namespace std;


	#define tstring wstring


class CPlugin {
	private:
		std::tstring lpzFileName;
		std::tstring lpzShortName;
		std::tstring lpzVersion;
		std::tstring lpzUnicodeInfo; //aditional info, Unicode aware ...
		std::tstring lpzTimestamp; //to show the last modified timestamp
		std::tstring lpzLinkedModules; //to show linked modules that aren't found
		MUUID pluginID;

	public:
		CPlugin();
		CPlugin(LPCTSTR fileName, LPCTSTR shortName, MUUID pluginID, LPCTSTR unicodeInfo, DWORD version, LPCTSTR timestamp, LPCTSTR linkedModules);
		CPlugin(const CPlugin&);
		~CPlugin();
		std::tstring getFileName();
		std::tstring getInformations(DWORD, TCHAR *, TCHAR *);
		
		void SetErrorMessage(LPCTSTR error);

		//Operators
		bool operator<(CPlugin&);
		bool operator>(CPlugin&);
		bool operator==(CPlugin&);
};
#endif