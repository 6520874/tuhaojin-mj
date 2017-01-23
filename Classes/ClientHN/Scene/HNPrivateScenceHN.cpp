#include "HNPrivateScenceHN.h"
#include "HNScenceManager.h"
#include "Game/GameLib.h"
//#include "ClientSC/SCGameManager.h"
#include "kernel/kernel/server/CServerItem.h"
#include "ClientHN/HNGameManager.h"

FV_SINGLETON_STORAGE(HNPrivateScenceHN);

#define GAME_TYPE_ZZ				0
#define GAME_TYPE_CS				1

#define GAME_TYPE_ZZ_258			0		//只能258做将
#define GAME_TYPE_ZZ_ZIMOHU			1		//只能自模胡
#define GAME_TYPE_ZZ_QIDUI			2		//可胡七对
#define GAME_TYPE_ZZ_QIANGGANGHU	3		//可抢杠胡

#define GAME_TYPE_CS_ZHUANGXIANFEN	0		//可抢杠胡

HNPrivateScenceHN::HNPrivateScenceHN()
	:m_iJoinCout(0)
	,m_iGameType(0)
{
	memset(m_iPlayCoutIdex,0,sizeof(m_iPlayCoutIdex));
	memset(m_kJoinNumTxt,0,sizeof(m_kJoinNumTxt));
	memset(m_dwPlayRule,0,sizeof(m_dwPlayRule));
	init();
	CServerItem::get()->SetServerPrivateSink(this);
}
HNPrivateScenceHN::~HNPrivateScenceHN()
{

}
bool HNPrivateScenceHN::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};

	WidgetScenceXMLparse kScence1("Script/HNPrivateScenceHN.xml",this);

	WidgetManager::addButtonCB("Button_JoinRoom",this,button_selector(HNPrivateScenceHN::Button_JoinNumReset));
	WidgetManager::addButtonCB("Button_JoinNum",this,button_selector(HNPrivateScenceHN::Button_JoinNum));
	WidgetManager::addButtonCB("Button_JoinNumDel",this,button_selector(HNPrivateScenceHN::Button_JoinNumDel));
	WidgetManager::addButtonCB("Button_JoinNumReset",this,button_selector(HNPrivateScenceHN::Button_JoinNumReset));

	WidgetManager::addButtonCB("Button_Show_Create_Private",this,button_selector(HNPrivateScenceHN::Button_Show_Create_Private));
	WidgetManager::addButtonCB("Button_Show_Join_Private",this,button_selector(HNPrivateScenceHN::Button_Show_Join_Private));
	WidgetManager::addButtonCB("Button_ZZCreateRoom",this,button_selector(HNPrivateScenceHN::Button_ZZCreateRoom));
	WidgetManager::addButtonCB("Button_CSCreateRoom",this,button_selector(HNPrivateScenceHN::Button_CSCreateRoom));
	WidgetManager::addButtonCB("Button_XZCreateRoom",this,button_selector(HNPrivateScenceHN::Button_XZCreateRoom));
	WidgetManager::addButtonCB("Button_DismissPrivate",this,button_selector(HNPrivateScenceHN::Button_DismissPrivate));
	WidgetManager::addButtonCB("Button_DismissPrivateNot",this,button_selector(HNPrivateScenceHN::Button_DismissPrivateNot));
	WidgetManager::addButtonCB("Button_PrivateAgagin",this,button_selector(HNPrivateScenceHN::Button_PrivateAgagin));
	WidgetManager::addButtonCB("Button_ZhuanZhuan",this,button_selector(HNPrivateScenceHN::Button_SetGameType));
	WidgetManager::addButtonCB("Button_ChangSha",this,button_selector(HNPrivateScenceHN::Button_SetGameType));

	WidgetManager::addButtonCB("Button_ZZGameCout0",this,button_selector(HNPrivateScenceHN::Button_PrivatePlayCout));
	WidgetManager::addButtonCB("Button_ZZGameCout1",this,button_selector(HNPrivateScenceHN::Button_PrivatePlayCout));

	WidgetManager::addButtonCB("Button_CSGameCout0",this,button_selector(HNPrivateScenceHN::Button_PrivatePlayCout));
	WidgetManager::addButtonCB("Button_CSGameCout1",this,button_selector(HNPrivateScenceHN::Button_PrivatePlayCout));

	WidgetManager::addButtonCB("Button_ZZGameRule0",this,button_selector(HNPrivateScenceHN::Button_ZZGameRule));
	WidgetManager::addButtonCB("Button_ZZGameRule1",this,button_selector(HNPrivateScenceHN::Button_ZZGameRule));
	WidgetManager::addButtonCB("Button_ZZGameRule2",this,button_selector(HNPrivateScenceHN::Button_ZZGameRule));
	
	setPlayerCoutIdex(GAME_TYPE_ZZ,0);
	setPlayerCoutIdex(GAME_TYPE_CS,0);
	return true;
}
void HNPrivateScenceHN::hideAll()
{
	WidgetFun::setVisible(this,"CreateRoomPlane",false);
	WidgetFun::setVisible(this,"JoinRoomPlane",false);
}
void HNPrivateScenceHN::setPlayerCoutIdex(int iGameType,int iIdex)
{
	for (int i = 0;i<2;i++)
	{
		if (iGameType == GAME_TYPE_ZZ)
		{
			WidgetFun::setChecked(this,utility::toString("Button_ZZGameCout",i),iIdex == i);
		}
		else
		{
			WidgetFun::setChecked(this,utility::toString("Button_CSGameCout",i),iIdex == i);
		}
	}
	m_iPlayCoutIdex[iGameType] = iIdex;
}
void HNPrivateScenceHN::setGameRuleIdex(int iGameType,int iIdex)
{
	if (iGameType == GAME_TYPE_ZZ)
	{
		if (FvMask::HasAny(m_dwPlayRule[0],_MASK_((dword)iIdex)))
		{
			FvMask::Del(m_dwPlayRule[0],_MASK_((dword)iIdex));
		}
		else
		{
			FvMask::Add(m_dwPlayRule[0],_MASK_((dword)iIdex));
		}
		WidgetFun::setChecked(this,"Button_ZZGameRule0",FvMask::HasAny(m_dwPlayRule[0],_MASK_((dword)GAME_TYPE_ZZ_ZIMOHU)));
		WidgetFun::setChecked(this,"Button_ZZGameRule1",FvMask::HasAny(m_dwPlayRule[0],_MASK_((dword)GAME_TYPE_ZZ_QIANGGANGHU)));
		WidgetFun::setChecked(this,"Button_ZZGameRule2",FvMask::HasAny(m_dwPlayRule[0],_MASK_((dword)GAME_TYPE_ZZ_QIDUI)));
	}
}
void HNPrivateScenceHN::OnSocketSubPrivateInfo(CMD_GR_Private_Info* pNetInfo)
{
}
void HNPrivateScenceHN::upJoinNumTxt()
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"JoinRoomPlane");
	for (int i = 0;i<m_iJoinCout;i++)
	{
		WidgetFun::setText(pNode,utility::toString("JoinTxt",i),utility::toString(m_kJoinNumTxt[i]));
	}
	for (int i = m_iJoinCout;i<6;i++)
	{
		WidgetFun::setText(pNode,utility::toString("JoinTxt",i),"");
	}
}
void HNPrivateScenceHN::OnSocketSubPrivateCreateSuceess(CMD_GR_Create_Private_Sucess* pNetInfo)
{
	//NoticeMsg::Instance().ShowTopMsg(utility::toString((int)pNetInfo->dwRoomNum));
}
void HNPrivateScenceHN::OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo)
{
	m_kPrivateInfo = *pNetInfo;
}
void HNPrivateScenceHN::OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo)
{

}
void HNPrivateScenceHN::OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo)
{

}
void HNPrivateScenceHN::Button_Show_Create_Private(cocos2d::Ref*,WidgetUserInfo* pInfo)
{
	WidgetFun::setVisible(this,"CreateRoomPlane",true);
	HNGameManager::Instance().connectGameServerByKindID(310);
}
void HNPrivateScenceHN::Button_Show_Join_Private(cocos2d::Ref*,WidgetUserInfo* pInfo)
{
	WidgetFun::setVisible(this,"JoinRoomPlane",true);
	Button_JoinNumReset(NULL,NULL);
	HNGameManager::Instance().connectGameServerByKindID(310);
}

void HNPrivateScenceHN::Button_ZZCreateRoom(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_GR_Create_Private kSendNet;
	zeromemory(&kSendNet,sizeof(kSendNet));
	kSendNet.bGameRuleIdex = m_dwPlayRule[0];
	kSendNet.bGameTypeIdex = m_iGameType;
	kSendNet.bPlayCoutIdex = m_iPlayCoutIdex[0];
	CServerItem::get()->SendSocketData(MDM_GR_PRIVATE,SUB_GR_CREATE_PRIVATE,&kSendNet,sizeof(kSendNet));
}
void HNPrivateScenceHN::Button_CSCreateRoom(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_GR_Create_Private kSendNet;
	zeromemory(&kSendNet,sizeof(kSendNet));
	kSendNet.bGameRuleIdex = m_dwPlayRule[1];
	kSendNet.bGameTypeIdex = m_iGameType;
	kSendNet.bPlayCoutIdex = m_iPlayCoutIdex[1];
	CServerItem::get()->SendSocketData(MDM_GR_PRIVATE,SUB_GR_CREATE_PRIVATE,&kSendNet,sizeof(kSendNet));
}
void HNPrivateScenceHN::Button_XZCreateRoom(cocos2d::Ref*,WidgetUserInfo*)
{
}
void HNPrivateScenceHN::Button_SureeDismissPrivate(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_GR_Dismiss_Private kNetInfo;
	kNetInfo.bDismiss = 1;
	CServerItem::get()->SendSocketData(MDM_GR_PRIVATE,SUB_GR_PRIVATE_DISMISS,&kNetInfo,sizeof(kNetInfo));
}
void HNPrivateScenceHN::Button_DismissPrivate(cocos2d::Ref*,WidgetUserInfo*)
{
	if (m_kPrivateInfo.bStartGame == 0)
	{
		PopScence::Instance().showAccessPlane(utility::getScriptString("PrivateTxt0"),this,
			button_selector(HNPrivateScenceHN::Button_SureeDismissPrivate),NULL);
		return;
	}
	CMD_GR_Dismiss_Private kNetInfo;
	kNetInfo.bDismiss = 1;
	CServerItem::get()->SendSocketData(MDM_GR_PRIVATE,SUB_GR_PRIVATE_DISMISS,&kNetInfo,sizeof(kNetInfo));
}
void HNPrivateScenceHN::Button_DismissPrivateNot(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_GR_Dismiss_Private kNetInfo;
	kNetInfo.bDismiss = 0;
	CServerItem::get()->SendSocketData(MDM_GR_PRIVATE,SUB_GR_PRIVATE_DISMISS,&kNetInfo,sizeof(kNetInfo));
}
void HNPrivateScenceHN::Button_PrivateAgagin(cocos2d::Ref*,WidgetUserInfo*)
{
	CServerItem::get()->SendSocketData(MDM_GR_PRIVATE,SUB_GR_RIVATE_AGAIN,NULL,0);
}
void HNPrivateScenceHN::Button_SetGameType(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"));
	CCAssert(iIdex>=0 && iIdex <2,"");
	m_iGameType = iIdex;
}
void HNPrivateScenceHN::Button_PrivatePlayCout(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"));
	setPlayerCoutIdex(m_iGameType,iIdex);
}
void HNPrivateScenceHN::Button_ZZGameRule(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"));
	setGameRuleIdex(m_iGameType,iIdex);
}
void HNPrivateScenceHN::Button_JoinNum(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	std::string kStr = (WidgetFun::getUserInfoValue(pUserInfo,"Idex"));
	if (!kStr.size())
	{
		CCAssert(false,"");
		return;
	}
	if (m_iJoinCout >=6)
	{
		return;
	}
	m_kJoinNumTxt[m_iJoinCout] = kStr[0];
	m_kJoinNumTxt[m_iJoinCout+1] = 0;
	m_iJoinCout++;
	upJoinNumTxt();

	if(m_iJoinCout == 6)
	{
		CMD_GR_Join_Private kSendNet;
		kSendNet.dwRoomNum = utility::parseInt(m_kJoinNumTxt);
		CServerItem::get()->SendSocketData(MDM_GR_PRIVATE,SUB_GR_JOIN_PRIVATE,&kSendNet,sizeof(kSendNet));
	}
}
void HNPrivateScenceHN::Button_JoinNumDel(cocos2d::Ref*,WidgetUserInfo*)
{
	m_iJoinCout--;
	if (m_iJoinCout < 0)
	{
		m_iJoinCout = 0;
	}
	upJoinNumTxt();
}
void HNPrivateScenceHN::Button_JoinNumReset(cocos2d::Ref*,WidgetUserInfo*)
{
	m_iJoinCout = 0;
	upJoinNumTxt();
}
