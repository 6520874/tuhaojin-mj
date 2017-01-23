#include "HNMJPlayer.h"
#include "HNMJSoundFun.h"


HNMJPlayer::HNMJPlayer(int iIdex,cocos2d::Node* pSeatNode )
	:GamePlayer(NULL)
	,m_iIdex(iIdex)
	,m_pSeatNode(pSeatNode)
	,m_kHandCardCout(0)
	,m_kWeaveCount(0)
	,m_pHandOutCard(NULL)
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
	m_pHandOutCard = NULL;
	setActOutCard(-1);
	m_kOutCardList.clear();
	setChiHuCard(0);
	
	WidgetFun::setVisible(m_pSeatNode,"Zhuang",false);
	WidgetFun::setVisible(m_pSeatNode,"BigOutNode",false);
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);
	WidgetFun::getChildWidget(m_pSeatNode,"CardNode1")->removeAllChildren();
	WidgetFun::getChildWidget(m_pSeatNode,"CardNode2")->removeAllChildren();
	WidgetFun::setVisible(m_pSeatNode,"AnimateNode",false);
	upPlayerState();
}

void HNMJPlayer::startGame()
{
	WidgetFun::runWidgetAction(m_pSeatNode,"Head","Start");
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);

	if (m_iIdex == 0)
	{
		WidgetFun::setVisible(m_pSeatNode,"Name",true);
//		WidgetFun::runWidgetAction(m_pSeatNode,"GoldImagic","Start");
		WidgetFun::setVisible(m_pSeatNode,"GoldImagic",true);
	}
	else
	{
		WidgetFun::setVisible(m_pSeatNode,"Name",true);
		WidgetFun::setVisible(m_pSeatNode,"GoldImagic",true);
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

	if (WidgetFun::getChildWidget(m_pSeatNode,"ButtonPlayerHeadClick"))
	{
		WidgetFun::setWidgetUserInfo(m_pSeatNode,"ButtonPlayerHeadClick","UserID",utility::toString((int)GetUserID()));
	}
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

cocos2d::Node* HNMJPlayer::getPlayerNode()
{
	return m_pSeatNode;
}
void HNMJPlayer::upPlayerInfo()
{
	WidgetFun::setText(m_pSeatNode,"Name",GetNickName());
	WidgetFun::setText(m_pSeatNode,"GoldTxt",(int)GetUserScore());
	if (GetHeadHttp() != "")
	{
		ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(m_pSeatNode,"HeadImagic"),
			GetHeadHttp(),GetUserID());
	}
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
	WidgetFun::setVisible(m_pSeatNode,"Offline",GetUserStatus() == US_OFFLINE);
}


void HNMJPlayer::seatDownCard()
{
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		pSprite->setPositionY(CARD_DWON_POSY);
	}
}

void HNMJPlayer::ClickCard(cocos2d::Node* pCard)
{
	if (pCard == NULL )return;

	int nPosY = pCard->getPositionY();
	if (nPosY == CARD_DWON_POSY)
	{
		pCard->setPositionY(CARD_UP_POSY);
	}
	else
	{
		pCard->setPositionY(CARD_DWON_POSY);
	}
}

void HNMJPlayer::runAniDianPao()
{
	HNMJSoundFun::playEffectByGender(GetGender(),HNMJSoundFun::EFFECT_HU);
//	WidgetFun::setPlaceImagicKey(m_pSeatNode,"AnimateNode","HuAni");
//	WidgetFun::runWidgetAction(m_pSeatNode,"AnimateNode","Start");
}
void HNMJPlayer::runAniZiMo()
{
	HNMJSoundFun::playEffectByGender(GetGender(),HNMJSoundFun::EFFECT_ZIMO);
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"AnimateNode","HuAni");
	WidgetFun::runWidgetAction(m_pSeatNode,"AnimateNode","Start");
}
void HNMJPlayer::runAniPeng()
{
	HNMJSoundFun::playEffectByGender(GetGender(),HNMJSoundFun::EFFECT_PENG);
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"AnimateNode","PengAni");
	WidgetFun::runWidgetAction(m_pSeatNode,"AnimateNode","Start");
}
void HNMJPlayer::runAniMingGang()
{
	HNMJSoundFun::playEffectByGender(GetGender(),HNMJSoundFun::EFFECT_GANG);
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"AnimateNode","GangAni");
	WidgetFun::runWidgetAction(m_pSeatNode,"AnimateNode","Start");
}
void HNMJPlayer::runAniAnGang()
{
	HNMJSoundFun::playEffectByGender(GetGender(),HNMJSoundFun::EFFECT_GANG);
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"AnimateNode","GangAni");
	WidgetFun::runWidgetAction(m_pSeatNode,"AnimateNode","Start");
}

void HNMJPlayer::runAniChi()
{
	HNMJSoundFun::playEffectByGender(GetGender(),HNMJSoundFun::EFFECT_CHI);
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"AnimateNode","ChiAni");
	WidgetFun::runWidgetAction(m_pSeatNode,"AnimateNode","Start");
}
