#include "UserInfo.h"
#include "Game/Script/ScriptData.h"
#include "GameManagerBase.h"

FV_SINGLETON_STORAGE(UserInfo);

UserInfo::UserInfo()
	:m_kIndividualMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
	,m_kUserInGameServerInfo(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	CGlobalUserInfo::GetInstance()->setSink(this);
	m_kUserInGameServerInfo.setMissionSink(this);
}
UserInfo::~UserInfo()
{

}
void UserInfo::reqAccountInfo(float fWaiteTime)
{
	if (fWaiteTime > 0.01f)
	{
		TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(UserInfo::on_reqAccountInfo,this),fWaiteTime); 
	}
	else
	{
		on_reqAccountInfo();
	}
}
void UserInfo::on_reqAccountInfo()
{
	m_kIndividualMission.queryAccountInfo();
}
tagGlobalUserData* UserInfo::getUserData()
{
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	return pGlobalUserInfo->GetGlobalUserData();
}
tagUserInsureInfo* UserInfo::GetUserInsureInfo()
{
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	return pGlobalUserInfo->GetUserInsureInfo();
}

dword UserInfo::getUserID()
{
	return CGlobalUserInfo::GetInstance()->getUserID();
}

dword UserInfo::getGameID()
{
	return CGlobalUserInfo::GetInstance()->getGameID();
}

byte UserInfo::getGender()
{
	return CGlobalUserInfo::GetInstance()->getGender();
}
void UserInfo::modeHeadHttp(std::string strHttp)
{
	m_kIndividualMission.modifyHeadHttp(strHttp);
}
std::string UserInfo::getHeadHttp()
{
	return CGlobalUserInfo::GetInstance()->getHeadHttp();
}
std::string UserInfo::getUserChannel()
{
	return CGlobalUserInfo::GetInstance()->getUserChannel();
}
std::string UserInfo::getUserIP()
{
	return CGlobalUserInfo::GetInstance()->GetGlobalUserData()->szLogonIP;
}

std::string UserInfo::getUserNicName()
{
	return CGlobalUserInfo::GetInstance()->getNickName();
}
std::string UserInfo::getAccounts()
{
	return CGlobalUserInfo::GetInstance()->getAccounts();
}
SCORE UserInfo::getUserScore()
{
	return CGlobalUserInfo::GetInstance()->getUserScore();
}
SCORE UserInfo::getUserInsure()
{
	return CGlobalUserInfo::GetInstance()->getUserInsure();
}
void UserInfo::reqIndividual()
{
	m_kIndividualMission.query(getUserID());
}
void UserInfo::upPlayerInfo()
{
	std::vector<UserCallInfo>::iterator itor = m_kUpPlayerInfoCB.begin();
	for (;itor != m_kUpPlayerInfoCB.end();itor++)
	{
		((*itor).pPoint->*(*itor).pFun)();
	}
}
void UserInfo::LoginSucess()
{
	std::vector<UserCallInfo>::iterator itor = m_kLoginSucessCB.begin();
	for (;itor != m_kLoginSucessCB.end();itor++)
	{
		((*itor).pPoint->*(*itor).pFun)();
	}
	checkInGameServer();
}
void UserInfo::onUserInGameServerID(CMD_GP_InGameSeverID* pNetInfo)
{
	if (pNetInfo->LockServerID != 0)
	{
		GameManagerBase::InstanceBase().connectGameServerByServerID(pNetInfo->LockServerID);
	}
}
void UserInfo::addLoginSucessCB(cocos2d::Ref* pPoint,QYSEL_CallFunc pFun)
{
	std::vector<UserCallInfo>::iterator itor = m_kLoginSucessCB.begin();
	for (;itor != m_kLoginSucessCB.end();itor++)
	{
		if ((*itor).pPoint == pPoint)
		{
			CCASSERT(false,"");
			return;
		}
	}
	UserCallInfo kCallInfo;
	kCallInfo.pPoint = pPoint;
	kCallInfo.pFun = pFun;
	m_kLoginSucessCB.push_back(kCallInfo);
}
void UserInfo::addUpPlayerInfoCB(cocos2d::Ref* pPoint,QYSEL_CallFunc pFun)
{
	std::vector<UserCallInfo>::iterator itor = m_kUpPlayerInfoCB.begin();
	for (;itor != m_kUpPlayerInfoCB.end();itor++)
	{
		if ((*itor).pPoint == pPoint)
		{
			CCASSERT(false,"");
			return;
		}
	}
	UserCallInfo kCallInfo;
	kCallInfo.pPoint = pPoint;
	kCallInfo.pFun = pFun;
	m_kUpPlayerInfoCB.push_back(kCallInfo);
}
void UserInfo::delCallByPoint(cocos2d::Ref* pPoint)
{
	std::vector<UserCallInfo>::iterator itor1 = m_kUpPlayerInfoCB.begin();
	while (itor1 != m_kUpPlayerInfoCB.end())
	{
		if ((*itor1).pPoint == pPoint)
		{
			itor1 = m_kUpPlayerInfoCB.erase(itor1);
			return;
		}
		itor1++;
	}
	std::vector<UserCallInfo>::iterator itor2 = m_kLoginSucessCB.begin();
	while (itor2 != m_kLoginSucessCB.end())
	{
		if ((*itor2).pPoint == pPoint)
		{
			itor2 = m_kLoginSucessCB.erase(itor2);
			return;
		}
		itor2++;
	}
}

void UserInfo::setPhoneNumber( std::string strNumber )
{
	m_kIndividualMission.modifyPhoneNumber(strNumber);
}

std::string UserInfo::getPhoneNumber()
{
	return CGlobalUserInfo::GetInstance()->getPhoneNumber();
}
void UserInfo::checkInGameServer()
{
	m_kUserInGameServerInfo.PerformInGameServerID(getUserID());
}