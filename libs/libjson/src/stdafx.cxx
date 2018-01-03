/*

Copyright (C) 2012-18 Miranda NG team (https://miranda-ng.org)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////////////////

LIBJSON_DLL(JSONNode&) operator<<(JSONNode &json, const INT_PARAM &param)
{
	json.push_back(JSONNode(param.szName, param.iValue));
	return json;
}

LIBJSON_DLL(JSONNode&) operator<<(JSONNode &json, const INT64_PARAM &param)
{
	char tmp[100];
	_i64toa_s(param.iValue, tmp, _countof(tmp), 10);
	json.push_back(JSONNode(param.szName, tmp));
	return json;
}

LIBJSON_DLL(JSONNode&) operator<<(JSONNode &json, const BOOL_PARAM &param)
{
	json.push_back(JSONNode(param.szName, param.bValue));
	return json;
}

LIBJSON_DLL(JSONNode&) operator<<(JSONNode &json, const CHAR_PARAM &param)
{
	json.push_back(JSONNode(param.szName, param.szValue));
	return json;
}

LIBJSON_DLL(JSONNode&) operator<<(JSONNode &json, const WCHAR_PARAM &param)
{
	json.push_back(JSONNode(param.szName, ptrA(mir_utf8encodeW(param.wszValue)).get()));
	return json;
}

LIBJSON_DLL(JSONNode&) operator<<(JSONNode &json, const NULL_PARAM &param)
{
	JSONNode newOne(JSON_NULL);
	newOne.set_name(param.szName);
	json.push_back(newOne);
	return json;
}

LIBJSON_DLL(JSONNode&) operator<<(JSONNode &json, const JSON_PARAM &param)
{
	JSONNode newOne(param.node);
	newOne.set_name(param.szName);
	json.push_back(newOne);
	return json;
}
