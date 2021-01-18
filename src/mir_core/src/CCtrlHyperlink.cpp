/*

Object UI extensions
Copyright (c) 2008  Victor Pavlychko, George Hazan
Copyright (C) 2012-21 Miranda NG team

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

#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////////////////
// CCtrlHyperlink

CCtrlHyperlink::CCtrlHyperlink(CDlgBase* wnd, int idCtrl, const char* url)
	: CCtrlBase(wnd, idCtrl),
	m_url(url)
{
	OnClick = Callback(this, &CCtrlHyperlink::Default_OnClick);
}

BOOL CCtrlHyperlink::OnCommand(HWND, WORD, WORD)
{
	OnClick(this);
	return FALSE;
}

void CCtrlHyperlink::Default_OnClick(CCtrlHyperlink*)
{
	ShellExecuteA(m_hwnd, "open", m_url, "", "", SW_SHOW);
}

void CCtrlHyperlink::SetUrl(const char *url)
{
	m_url = url;
}

const char* CCtrlHyperlink::GetUrl()
{
	return m_url;
}
