/*
Copyright (c) 2015-20 Miranda NG team (https://miranda-ng.org)

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

/* HISTORY SYNC */

void CSkypeProto::OnGetServerHistory(NETLIBHTTPREQUEST *response, AsyncHttpRequest *pRequest)
{
	JsonReply reply(response);
	if (reply.error())
		return;

	auto &root = reply.data();
	const JSONNode &metadata = root["_metadata"];
	const JSONNode &conversations = root["messages"].as_array();

	int totalCount = metadata["totalCount"].as_int();
	std::string syncState = metadata["syncState"].as_string();

	bool markAllAsUnread = getBool("MarkMesUnread", true);
	bool bUseLocalTime = pRequest->pUserInfo != 0;
	time_t iLocalTime = time(0);

	if (totalCount >= 99 || conversations.size() >= 99)
		PushRequest(new GetHistoryRequest(syncState.c_str(), pRequest->pUserInfo));

	for (int i = (int)conversations.size(); i >= 0; i--) {
		const JSONNode &message = conversations.at(i);

		CMStringA szMessageId = message["clientmessageid"] ? message["clientmessageid"].as_string().c_str() : message["skypeeditedid"].as_string().c_str();

		int userType;
		CMStringW wszChatId = UrlToSkypeId(message["conversationLink"].as_mstring(), &userType);
		CMStringW wszContent = message["content"].as_mstring();
		CMStringW wszFrom = UrlToSkypeId(message["from"].as_mstring());

		std::string messageType = message["messagetype"].as_string();
		int emoteOffset = message["skypeemoteoffset"].as_int();
		time_t timestamp = IsoToUnixTime(message["composetime"].as_string());

		bool isEdited = message["skypeeditedid"];

		MCONTACT hContact = FindContact(wszChatId);

		if (timestamp > getDword(hContact, "LastMsgTime", 0))
			setDword(hContact, "LastMsgTime", timestamp);
		if (bUseLocalTime)
			timestamp = iLocalTime;

		if (userType == 8 || userType == 2) {
			DWORD iFlags = DBEF_UTF;

			if (!markAllAsUnread)
				iFlags |= DBEF_READ;

			if (IsMe(wszFrom))
				iFlags |= DBEF_SENT;

			if (messageType == "Text" || messageType == "RichText") {
				CMStringW szMessage(messageType == "RichText" ? RemoveHtml(wszContent) : wszContent);
				MEVENT dbevent = GetMessageFromDb(szMessageId);
				if (isEdited && dbevent != NULL)
					EditEvent(hContact, dbevent, szMessage, timestamp);
				else
					AddDbEvent(emoteOffset == 0 ? EVENTTYPE_MESSAGE : SKYPE_DB_EVENT_TYPE_ACTION, hContact, timestamp, iFlags, szMessage.c_str()+emoteOffset, szMessageId);
			}
			else if (messageType == "Event/Call") {
				AddDbEvent(SKYPE_DB_EVENT_TYPE_CALL_INFO, hContact, timestamp, iFlags, wszContent, szMessageId);
			}
			else if (messageType == "RichText/Files") {
				AddDbEvent(SKYPE_DB_EVENT_TYPE_FILETRANSFER_INFO, hContact, timestamp, iFlags, wszContent, szMessageId);
			}
			else if (messageType == "RichText/UriObject") {
				AddDbEvent(SKYPE_DB_EVENT_TYPE_URIOBJ, hContact, timestamp, iFlags, wszContent, szMessageId);
			}
			else if (messageType == "RichText/Contacts") {
				ProcessContactRecv(hContact, timestamp, T2Utf(wszContent), szMessageId);
			}
			else if (messageType == "RichText/Media_Album") {
				// do nothing
			}
			else {
				AddDbEvent(SKYPE_DB_EVENT_TYPE_UNKNOWN, hContact, timestamp, iFlags, wszContent, szMessageId);
			}
		}
		else if (userType == 19) {
			auto *si = g_chatApi.SM_FindSession(wszChatId, m_szModuleName);
			if (si == nullptr)
				return;

			if (messageType == "Text" || messageType == "RichText")
				AddMessageToChat(si, wszFrom, messageType == "RichText" ? RemoveHtml(wszContent) : wszContent, emoteOffset != NULL, emoteOffset, timestamp, true);
		}
	}
}

void CSkypeProto::ReadHistoryRest(const char *szUrl)
{
	auto *p = strstr(szUrl, g_plugin.szDefaultServer);
	if (p)
		PushRequest(new SyncHistoryFirstRequest(p+ g_plugin.szDefaultServer.GetLength()+3));
}

INT_PTR CSkypeProto::GetContactHistory(WPARAM hContact, LPARAM)
{
	PushRequest(new GetHistoryRequest(getId(hContact), 100, 0, false));
	return 0;
}

void CSkypeProto::OnSyncHistory(NETLIBHTTPREQUEST *response, AsyncHttpRequest*)
{
	JsonReply reply(response);
	if (reply.error())
		return;

	auto &root = reply.data();
	const JSONNode &metadata = root["_metadata"];
	const JSONNode &conversations = root["conversations"].as_array();

	int totalCount = metadata["totalCount"].as_int();
	std::string syncState = metadata["syncState"].as_string();

	if (totalCount >= 99 || conversations.size() >= 99)
		ReadHistoryRest(syncState.c_str());

	for (auto &conversation : conversations) {
		const JSONNode &lastMessage = conversation["lastMessage"];
		if (!lastMessage)
			continue;

		int iUserType;
		std::string strConversationLink = lastMessage["conversationLink"].as_string();
		CMStringA szSkypename = UrlToSkypeId(strConversationLink.c_str(), &iUserType);
		if (iUserType == 8 || iUserType == 2) {
			time_t composeTime(IsoToUnixTime(lastMessage["composetime"].as_string()));

			MCONTACT hContact = FindContact(szSkypename);
			if (hContact != NULL)
				if (getDword(hContact, "LastMsgTime", 0) < composeTime)
					PushRequest(new GetHistoryRequest(szSkypename, 100, 0, true));
		}
	}

	m_bHistorySynced = true;
}
