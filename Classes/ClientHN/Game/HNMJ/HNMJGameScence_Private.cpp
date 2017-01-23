#include "HNMJGameScence.h"
#include "Game/GameLib.h"
#include "HNMJPlayer.h"
#include "Game/Game/MissionWeiXin.h"

void HNMJGameScence::initPrivate()
{
	WidgetScenceXMLparse kScence("Script/HNGamePrivateScence.xml",this);

	WidgetManager::addButtonCB("XZDDButton_WeiXinFriend",this,button_selector(HNMJGameScence::XZDDButton_WeiXinFriend));
	WidgetManager::addButtonCB("XZDDButton_WeiXinImagic",this,button_selector(HNMJGameScence::XZDDButton_WeiXinImagic));
}
void HNMJGameScence::defaultPrivateState()
{
	WidgetFun::setVisible(this,"PrivateDismissPlane",false);
	WidgetFun::setVisible(this,"PrivateEndPlane",false);
}
void HNMJGameScence::OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo)
{
	WidgetFun::setVisible(this,"PrivateInfo",true);
	WidgetFun::setText(this,"RoomID",utility::toString(pNetInfo->dwRoomNum));

	bool bShowDismiss = false;
	if (pNetInfo->dwCreateUserID == m_pLocal->GetUserID())
	{
		bShowDismiss = true;
	}
	if (pNetInfo->bStartGame)
	{
		bShowDismiss = true;
	}
	WidgetFun::setVisible(this,"XZDDButton_WeiXinFriend",!pNetInfo->bStartGame);
	WidgetFun::setVisible(this,"Button_DismissPrivate",bShowDismiss);
}
void HNMJGameScence::OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo)
{
	for (int i  = 0;i<MAX_PLAYER;i++)
	{
		GamePlayer* pPlayer = m_pPlayer[i];
		if (!pPlayer)
		{
			continue;
		}
		int iChairID = pPlayer->GetChairID();
		cocos2d::Node* pNode = WidgetFun::getChildWidget(this,utility::toString(utility::toString("PrivateEndPlayer",i)));
		WidgetFun::setText(pNode,"Name",pPlayer->GetNickName());
		WidgetFun::setText(pNode,"ID",(int)pPlayer->GetUserID());
		WidgetFun::setText(pNode,"ActionTxt0",utility::toString("x",(int)pNetInfo->lPlayerAction[iChairID][0]));
		WidgetFun::setText(pNode,"ActionTxt1",utility::toString("x",(int)pNetInfo->lPlayerAction[iChairID][1]));
		WidgetFun::setText(pNode,"ActionTxt2",utility::toString("x",(int)pNetInfo->lPlayerAction[iChairID][2]));
		WidgetFun::setText(pNode,"ActionTxt3",utility::toString("x",(int)pNetInfo->lPlayerAction[iChairID][3]));
		WidgetFun::setText(pNode,"TotalScore",utility::toString((int)pNetInfo->lPlayerWinLose[iChairID]));
	}
	WidgetFun::setVisible(this,"PrivateEndPlane",true);
}
void HNMJGameScence::OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo)
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"PrivateDismissPlane");
	pNode->setVisible(true);
	if (pNetInfo->dwDissUserCout == 0)
	{
		pNode->runAction(cocos2d::CCVisibleAction::create(3.0f,false));
		return;
	}
	int kChairID[4] = {1,1,1,1};

	bool bShowSelfAction = true;
	int iIdex = 0;
	for (int i = 0;i<(int)pNetInfo->dwDissUserCout;i++)
	{
		std::string kName = "";
		GamePlayer* pPlayer = getPlayerByChairID(pNetInfo->dwDissChairID[i]);
		kChairID[pNetInfo->dwDissChairID[i]] = 0;
		if (pPlayer)
		{
			kName = pPlayer->GetNickName();
		}
		if (pPlayer == m_pLocal)
		{
			bShowSelfAction = false;
		}
		WidgetFun::setText(pNode,utility::toString("DissPrivateName",iIdex),kName);
		WidgetFun::setPlaceTextKey(pNode,utility::toString("DissPrivateState",iIdex),"Text0",kName);
		iIdex++;
	}
	for (int i = 0;i<(int)pNetInfo->dwNotAgreeUserCout;i++)
	{
		std::string kName = "";
		GamePlayer* pPlayer = getPlayerByChairID(pNetInfo->dwNotAgreeChairID[i]);
		kChairID[pNetInfo->dwNotAgreeChairID[i]] = 0;
		if (pPlayer)
		{
			kName = pPlayer->GetNickName();
		}
		if (pPlayer == m_pLocal)
		{
			bShowSelfAction = false;
		}
		WidgetFun::setText(pNode,utility::toString("DissPrivateName",iIdex),kName);
		WidgetFun::setPlaceTextKey(pNode,utility::toString("DissPrivateState",iIdex),"Text1",kName);
		iIdex++;
	}
	for (int i = 0;i<4;i++)
	{
		if (kChairID[i] == 0)
		{
			continue;
		}
		WidgetFun::setText(pNode,utility::toString("DissPrivateName",iIdex),"");
		WidgetFun::setText(pNode,utility::toString("DissPrivateState",iIdex),"");
		std::string kName = "";
		GamePlayer* pPlayer = getPlayerByChairID(i);
		if (!pPlayer)
		{
			continue;
		}
		if (pPlayer)
		{
			kName = pPlayer->GetNickName();
		}
		WidgetFun::setText(pNode,utility::toString("DissPrivateName",iIdex),kName);
		iIdex++;
	}
	WidgetFun::setVisible(pNode,"PrivateDismissAction",bShowSelfAction);
}
void HNMJGameScence::XZDDButton_WeiXinFriend(cocos2d::Ref*,WidgetUserInfo*)
{
	MissionWeiXin::Instance().shareTextWeiXin(utility::getScriptReplaceValue("PrivateTxt1",WidgetFun::getText(this,"RoomID")));
}
void HNMJGameScence::XZDDButton_WeiXinImagic(cocos2d::Ref*,WidgetUserInfo*)
{
	static HNMJGameScence* pTemp = NULL;
	pTemp = this;
	cocos2d::utils::captureScreen([](bool b, std::string name){
		if (b)
		{
			MissionWeiXin::Instance().shareScreenWeiXin(name);
			pTemp->Button_ExitGameBase(NULL,NULL);
			pTemp->ExitGame();
			WidgetFun::setVisible(pTemp,"PrivateEndPlane",false); 
		}
	},"screenshot.png");

}