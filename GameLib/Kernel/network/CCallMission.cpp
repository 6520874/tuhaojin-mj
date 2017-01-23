#include "CCallMission.h"
#include "Game/Script/utility.h"

CCallMission::CCallMission(std::string kName,const char* url, int port)
	:CSocketMission(url,port)
	,m_kClassName(kName)
{
}

CCallMission::~CCallMission()
{
	clearCall();
}

void CCallMission::clearCall()
{
	for (int i = 0;i<(int)m_kLinkCallFun.size();i++)
	{
		delete m_kLinkCallFun[i];
	}
	m_kLinkCallFun.clear();
}
void CCallMission::onEventTCPSocketLink()
{
	utility::log(utility::toString(m_kClassName,":onEventTCPSocketLink").c_str());

	for (int i = 0;i<(int)m_kLinkCallFun.size();i++)
	{
		m_kLinkCallFun[i]->CallFun();
	}
	clearCall();
}
void CCallMission::onEventTCPSocketShut()
{
	utility::log(utility::toString(m_kClassName,":onEventTCPSocketShut").c_str());

	clearCall();
}
void CCallMission::onEventTCPSocketError(int errorCode)
{
	utility::log(utility::toString(m_kClassName,":onEventTCPSocketShut").c_str());

	clearCall();
}
bool CCallMission::onEventTCPSocketRead(int main, int sub, void* data, int dataSize)
{
	utility::log(utility::toString(m_kClassName,":onEventTCPSocketRead").c_str());

	for (int i = 0;i<(int)m_kNetCallFun.size();i++)
	{
		NetCall& kNetInfo = m_kNetCallFun[i];
		if (kNetInfo.iSub == sub)
		{
			kNetInfo.pFun(data,dataSize);
			return true;
		}
	}
	CCASSERT(false,"");
	return false;
}