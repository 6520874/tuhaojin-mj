#include "HNMJPlayer.h"


HNMJPlayer::HNMJPlayer(int iIdex,cocos2d::Node* pSeatNode )
	:GamePlayer(NULL)
	,m_iIdex(iIdex)
	,m_pSeatNode(pSeatNode)
	,m_kHandCardCout(0)
	,m_kWeaveCount(0)
{
	init();
}

HNMJPlayer::~HNMJPlayer()
{

}

void HNMJPlayer::init()
{
	PlayerLeave();
}
int  HNMJPlayer::getIdex()
{
	return m_iIdex;
}
void HNMJPlayer::defaultState()
{
	m_kHandCardCout = 0;
	m_kWeaveCount = 0;
	m_iOutCardCout = 0;
	setActOutCard(-1);
	
	WidgetFun::setVisible(m_pSeatNode,"Zhuang",false);
	WidgetFun::setVisible(m_pSeatNode,"BigOutNode",false);
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);
	WidgetFun::getChildWidget(m_pSeatNode,"CardNode1")->removeAllChildren();
	WidgetFun::getChildWidget(m_pSeatNode,"CardNode2")->removeAllChildren();

	upPlayerState();
}

void HNMJPlayer::startGame()
{
	WidgetFun::runWidgetAction(m_pSeatNode,"Head","Start");
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);

	if (m_iIdex == 0)
	{
		WidgetFun::setVisible(m_pSeatNode,"Name",false);
		WidgetFun::runWidgetAction(m_pSeatNode,"GoldImagic","Start");
	}
	else
	{
		WidgetFun::setVisible(m_pSeatNode,"Name",false);
		WidgetFun::setVisible(m_pSeatNode,"GoldImagic",false);
	}
}
void HNMJPlayer::EndGame()
{
	if (m_pUserItem == NULL )
	{
		return;
	}
	WidgetFun::runWidgetAction(m_pSeatNode,"Head","End");
	if (m_iIdex == 0)
	{
		WidgetFun::setVisible(m_pSeatNode,"Name",true);
		WidgetFun::runWidgetAction(m_pSeatNode,"GoldImagic","End");
	}
	else
	{
		WidgetFun::setVisible(m_pSeatNode,"Name",true);
		WidgetFun::setVisible(m_pSeatNode,"GoldImagic",true);
	}
}

void HNMJPlayer::PlayerEnter()
{
	WidgetFun::setVisible(m_pSeatNode,"Name",true);
	WidgetFun::setVisible(m_pSeatNode,"GoldImagic",true);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagicEmpty",false);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagic",true);
}

void HNMJPlayer::PlayerLeave()
{
	defaultState();
	WidgetFun::setVisible(m_pSeatNode,"HeadImagicEmpty",true);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagic",false);
	WidgetFun::setVisible(m_pSeatNode,"Name",false);
	WidgetFun::setVisible(m_pSeatNode,"GoldImagic",false);
	WidgetFun::setText(m_pSeatNode,"Name","");
	WidgetFun::setText(m_pSeatNode,"GoldTxt","");
}

void HNMJPlayer::upPlayerInfo()
{
	WidgetFun::setText(m_pSeatNode,"Name",GetNickName());
	WidgetFun::setText(m_pSeatNode,"GoldTxt",(int)GetUserScore());
}
void HNMJPlayer::setZhuang()
{
	WidgetFun::setVisible(m_pSeatNode,"Zhuang",true);
}
void HNMJPlayer::showAddGold(int iGold)
{
	WidgetFun::setText(m_pSeatNode,"AddGoldTxt",iGold);
	WidgetFun::runWidgetAction(m_pSeatNode,"AddGoldTxt","Start");
}
void HNMJPlayer::showEffect(std::string kKey)
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"EffectImagic");
	WidgetFun::setPlaceImagicKey(pNode,kKey);
	pNode->stopAllActions();
	WidgetFun::runWidgetAction(pNode,"Start");
}
void HNMJPlayer::showStatusImagic(std::string kKey)
{
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"ReadyState",kKey);
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",true);
}
void HNMJPlayer::upPlayerState()
{
	if (GetUserStatus() == US_READY)
	{
		showStatusImagic("Ready");
	}
}