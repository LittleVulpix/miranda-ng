/*

Facebook plugin for Miranda NG
Copyright © 2019 Miranda NG team

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "stdafx.h"

FacebookProto::FacebookProto(const char *proto_name, const wchar_t *username) :
	PROTO<FacebookProto>(proto_name, username)
{
	szDeviceID = getMStringA(DBKEY_DEVICE_ID);
	if (szDeviceID.IsEmpty()) {
		UUID deviceId;
		UuidCreate(&deviceId);
		RPC_CSTR szId;
		UuidToStringA(&deviceId, &szId);
		szDeviceID = szId;
		setString(DBKEY_DEVICE_ID, szDeviceID);
		RpcStringFreeA(&szId);
	}

}

FacebookProto::~FacebookProto()
{
}

void FacebookProto::OnModulesLoaded()
{
}
