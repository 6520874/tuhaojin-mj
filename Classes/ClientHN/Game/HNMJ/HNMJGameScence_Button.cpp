#include "HNMJGameScence.h"
#include "Game/GameLib.h"

#include "CMD_HNMJ.h"
#include "HNMJPlayer.h"
#include "HNMJGameLogic.h"

void HNMJGameScence::initButton()
{
	WidgetManager::addButtonCB("HNMJButton_Ready",this,button_selector(HNMJGameScence::HNMJButton_Ready));
	WidgetManager::addButtonCB("HNMJButton_GuoAction",this,button_selector(HNMJGameScence::HNMJButton_GuoAction));
	WidgetManager::addButtonCB("HNMJButton_GangAction",this,button_selector(HNMJGameScence::HNMJButton_GangAction));
	WidgetManager::addButtonCB("HNMJButton_PengAction",this,button_selector(HNMJGameScence::HNMJButton_PengAction));
	WidgetManager::addButtonCB("HNMJButton_HuAction",this,button_selector(HNMJGameScence::HNMJButton_HuAction));

	WidgetManager::addButtonCB("HNMJButtonAction_ShowCard",this,button_selector(HNMJGameScence::HNMJButtonAction_ShowCard));
	WidgetManager::addButtonCB("HNMJButtonAction_ShowNext",this,button_selector(HNMJGameScence::HNMJButtonAction_ShowNext));
}
void HNMJGameScence::HNMJButton_Ready(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	sendReady();
	defaultState();
}
void HNMJGameScence::HNMJButton_GuoAction(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=WIK_NULL;
	OperateCard.cbOperateCard=0;
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	WidgetFun::setVisible(this,"SelfActionNode",false);
}
void HNMJGameScence::HNMJButton_GangAction(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=WIK_GANG;
	OperateCard.cbOperateCard=utility::parseInt(WidgetFun::getWidgetUserInfo(this,"NotifyCard"));;
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	WidgetFun::setVisible(this,"SelfActionNode",false);
}
void HNMJGameScence::HNMJButton_PengAction(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=WIK_PENG;
	OperateCard.cbOperateCard=utility::parseInt(WidgetFun::getWidgetUserInfo(this,"NotifyCard"));;
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	WidgetFun::setVisible(this,"SelfActionNode",false);
}
void HNMJGameScence::HNMJButton_HuAction(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=WIK_CHI_HU;
	OperateCard.cbOperateCard=0;
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	WidgetFun::setVisible(this,"SelfActionNode",false);
}
void HNMJGameScence::HNMJButtonAction_ShowNext(cocos2d::Ref*,WidgetUserInfo*)
{
	showSaiZi(utility::parseUInt(WidgetFun::getWidgetUserInfo(this,"SaiZiNode","NextValue")));
}
void HNMJGameScence::HNMJButtonAction_ShowCard(cocos2d::Ref*,WidgetUserInfo*)
{
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->showHandCard();
	}
	setCurrentPlayer(m_iCurrentUser,WIK_NULL);
	WidgetFun::setVisible(this,"TimeNode",true);
	WidgetFun::setVisible(this,"LastCardNode",true);
}
