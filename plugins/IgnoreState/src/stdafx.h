/*

   IgnoreState plugin for Miranda-IM (www.miranda-im.org)
   (c) 2010 by Kildor

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

   */

#ifndef COMMHEADERS_H
#define COMMHEADERS_H

#include <windows.h>
#include <commctrl.h>

#include <win2k.h>
#include <newpluginapi.h>
#include <m_database.h>
#include <m_ignore.h>
#include <m_skin.h>
#include <m_options.h>
#include <m_langpack.h>
#include <m_extraicons.h>
#include <m_gui.h>

#include "resource.h"
#include "version.h"

#define MODULENAME "IgnoreState"

struct CMPlugin : public PLUGIN<CMPlugin>
{
	CMPlugin();

	int Load() override;
};

struct IGNOREITEMS
{
	wchar_t* name;
	int   type;
	int   icon;
	bool  filtered;
};

extern IGNOREITEMS ii[];
extern int nII;

static byte bUseMirandaSettings;

void applyExtraImage(MCONTACT hContact);

int onOptInitialise(WPARAM wParam, LPARAM);
BOOL checkState(int type);
VOID fill_filter();

extern HANDLE hExtraIcon;

#endif //COMMHEADERS_H
