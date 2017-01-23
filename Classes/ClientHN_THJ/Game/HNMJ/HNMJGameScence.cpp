#include "HNMJGameScence.h"
#include "HNMJPlayer.h"
#include "Game/GameLib.h"
#include "CMD_HNMJ.h"
#include "HNMJPlayer.h"
#include "HNMJGameLogic.h"
#include "HNMJSoundFun.h"

FV_SINGLETON_STORAGE(HNMJGameScence);

HNMJGameScence::HNMJGameScence()
	:m_iBankerUser(0)
	,m_iCurrentUser(0)
	,m_iUserAction(0)
	,m_pLocal(NULL)
	,m_pTouchCardNode(NULL)
	,m_nPlayCount(0)
{

	init();
}
HNMJGameScence::~HNMJGameScence()
{

}
bool HNMJGameScence::init()
{
	if (!GameBase::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/HNMJ/GameScence.xml",this);

	initPrivate();

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i] = new HNMJPlayer(i,WidgetFun::getChildWidget(this,utility::toString("Player",i)));
	}
	m_pLocal = m_pPlayer[0];

	WidgetManager::addButtonCB("HNMJButton_GameExit",this,button_selector(GameBase::Button_ExitGameBase));

	initButton();
	initNet();
	initTouch();
	return true;
}
void HNMJGameScence::EnterScence()
{
	WidgetFun::setVisible(this,"GameHNMJPlane",true);
	if (WidgetFun::getChildWidget(this,"PrivateEndPlane"))
	{
		WidgetFun::setVisible(this,"PrivateEndPlane",false);
	}
	defaultState();
	HNMJSoundFun::playBackMusic(HNMJSoundFun::EX_BACKGRUAND);
}
void HNMJGameScence::HideAll()
{
	WidgetFun::setVisible(this,"GameHNMJPlane",false);
	defaultPrivateState();
}
void HNMJGameScence::defaultState()
{
	m_pTouchCardNode = NULL;

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->defaultState();
		m_pPlayer[i]->EndGame();
	}

	WidgetFun::setVisible(this,"TimeNode",false);
	WidgetFun::setVisible(this,"LastCardNode",false);
	WidgetFun::setVisible(this,"SaiZiNode",false);
	WidgetFun::setVisible(this,"SelfActionNode",false);
	WidgetFun::setVisible(this,"GameJieSuanNode",false);

	defaultPrivateState();
	defaultPlayerActionState();
}
void HNMJGameScence::defaultPlayerActionState()
{
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"TimeNode");
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		WidgetFun::setVisible(pRootNode,utility::toString("TimePoint",i),false);
	}
	WidgetFun::getChildWidget(pRootNode,"ActPlayerLastTime")->stopAllActions();
	WidgetFun::setText(pRootNode,"ActPlayerLastTime","0");
}
std::string HNMJGameScence::getStringHuRight(dword kValue)
{
	dword dwRight[] = {
		CHR_PENGPENG_HU		,							//碰碰胡
		CHR_JIANGJIANG_HU		,							//将将胡
		CHR_QING_YI_SE		,							//清一色
		CHR_HAI_DI_LAO		,							//海底捞
		CHR_HAI_DI_PAO		,							//海底捞
		CHR_QI_XIAO_DUI		,							//七小对
		CHR_HAOHUA_QI_XIAO_DUI	,							//豪华七小对
		CHR_GANG_KAI			,							//杠上开花
		CHR_QIANG_GANG_HU		,							//抢杠胡
		CHR_GANG_SHANG_PAO	,							//杠上跑
		CHR_QUAN_QIU_REN		,							//全求人
		CHR_XIAO_DA_SI_XI	,								//大四喜
		CHR_XIAO_BAN_BAN_HU		,						//板板胡
		CHR_XIAO_QUE_YI_SE		,						//缺一色
		CHR_XIAO_LIU_LIU_SHUN								//六六顺
	};
	std::string pszRight[] = {
		ScriptData<std::string>("HuRight0").Value(),
		ScriptData<std::string>("HuRight1").Value(),
		ScriptData<std::string>("HuRight2").Value(),
		ScriptData<std::string>("HuRight3").Value(),
		ScriptData<std::string>("HuRight4").Value(),
		ScriptData<std::string>("HuRight5").Value(),
		ScriptData<std::string>("HuRight6").Value(),
		ScriptData<std::string>("HuRight7").Value(),
		ScriptData<std::string>("HuRight8").Value(),
		ScriptData<std::string>("HuRight9").Value(),
		ScriptData<std::string>("HuRight10").Value(),
		ScriptData<std::string>("HuRight11").Value(),
		ScriptData<std::string>("HuRight12").Value(),
		ScriptData<std::string>("HuRight13").Value(),
		ScriptData<std::string>("HuRight14").Value(),
	};
	std::string kTxt;
	HNMJ::CChiHuRight	kChiHuRight;	
	kChiHuRight.SetRightData(&kValue,MAX_RIGHT_COUNT );
	for( BYTE j = 0; j < CountArray(pszRight); j++ )
	{
		if( !(kChiHuRight&dwRight[j]).IsEmpty() )
		{
			kTxt+=pszRight[j];
		}
	}
	return kTxt;
}
void HNMJGameScence::showSaiZi(unsigned int iValue)
{
	word wSice1 = word(iValue>>16);
	word wSice2 = word(iValue);
	if (wSice1 > 0)
	{
		BYTE SiceFirst = (wSice1 >> 8);
		BYTE SiceSecond = (wSice1);
		std::string kImagic = WidgetFun::getWidgetUserInfo(this,"SaiZiNode","Imagic");
		WidgetFun::setImagic(this,"SaiZi0",utility::toString(kImagic,(int)SiceFirst,".png"));
		WidgetFun::setImagic(this,"SaiZi1",utility::toString(kImagic,(int)SiceSecond,".png"));
		WidgetFun::setWidgetUserInfo(this,"SaiZiNode","NextValue",utility::toString((int)wSice2));
		WidgetFun::runWidgetAction(this,"SaiZiNode","ActionStart1");
	}
	else if (wSice2 > 0)
	{
		BYTE SiceFirst = (wSice2 >> 8);
		BYTE SiceSecond = (wSice2);
		std::string kImagic = WidgetFun::getWidgetUserInfo(this,"SaiZiNode","Imagic");
		WidgetFun::setImagic(this,"SaiZi0",utility::toString(kImagic,(int)SiceFirst,".png"));
		WidgetFun::setImagic(this,"SaiZi1",utility::toString(kImagic,(int)SiceSecond,".png"));
		WidgetFun::runWidgetAction(this,"SaiZiNode","ActionStart2");
		getPlayerByChairID(m_iBankerUser)->setZhuang();
	}
}
void HNMJGameScence::setCurrentPlayer(int iCurrentPlayer,int iUserAction)
{
	if (iCurrentPlayer < 0 || iCurrentPlayer > MAX_PLAYER)
	{
		CCASSERT(false,"");
		return;
	}
	defaultPlayerActionState();

	m_iCurrentUser = iCurrentPlayer;

	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"TimeNode");

	HNMJPlayer* pPlyer = getPlayerByChairID(m_iCurrentUser);
	if (!pPlyer)
	{
		return;
	}
	WidgetFun::getChildWidget(pRootNode,"ActPlayerLastTime")->runAction(
		cocos2d::MoveExTxtTime::create(TIME_OPERATE_CARD));

	if (iUserAction == WIK_NULL)
	{
		WidgetFun::setVisible(pRootNode,utility::toString("TimePoint",pPlyer->getIdex()),true);
	}

	if(pPlyer == m_pLocal && iUserAction != WIK_NULL)
	{
		int iCout = 0;
		int iStartPos = 0;
		int iNextPos = 0;
		if (BYTE(iUserAction)&WIK_GANG)
		{
			iCout++;
		}
		if (BYTE(iUserAction)&WIK_PENG)
		{
			iCout++;
		}
		if (BYTE(iUserAction)&WIK_LEFT || BYTE(iUserAction)&WIK_CENTER || BYTE(iUserAction)&WIK_RIGHT)
		{
			iCout++;
		}
		if (BYTE(iUserAction)&WIK_CHI_HU || BYTE(iUserAction)&WIK_ZI_MO)
		{
			iCout++;
		}
		if (BYTE(iUserAction)&WIK_XIAO_HU)
		{
			iCout++;
		}
		if (iCout == 1)
		{
			iStartPos = -140;
			iNextPos = 250;
		}
		if (iCout == 2)
		{
			iStartPos = -200;
			iNextPos = 280;
		}
		if (iCout == 3)
		{
			iStartPos = -300;
			iNextPos = 280;
		}
		if (iCout == 4)
		{
			iStartPos = -300;
			iNextPos = 250;
		}
		if (BYTE(iUserAction)&WIK_CHI_HU || BYTE(iUserAction)&WIK_ZI_MO)
		{
			WidgetFun::setPos(this,"HNMJButton_HuAction",cocos2d::Vec2(iStartPos,0));
			iStartPos+=iNextPos;
		}
		if (BYTE(iUserAction)&WIK_XIAO_HU )
		{
			WidgetFun::setPos(this,"HNMJButton_XiaoHuAction",cocos2d::Vec2(iStartPos,0));
			iStartPos+=iNextPos;
		}
		if (BYTE(iUserAction)&WIK_GANG)
		{
			WidgetFun::setPos(this,"HNMJButton_GangAction",cocos2d::Vec2(iStartPos,0));
			iStartPos+=iNextPos;
		}
		if (BYTE(iUserAction)&WIK_PENG)
		{
			WidgetFun::setPos(this,"HNMJButton_PengAction",cocos2d::Vec2(iStartPos,0));
			iStartPos+=iNextPos;
		}
		if (BYTE(iUserAction)&WIK_LEFT || BYTE(iUserAction)&WIK_CENTER || BYTE(iUserAction)&WIK_RIGHT)
		{
			WidgetFun::setPos(this,"HNMJButton_PengAction",cocos2d::Vec2(iStartPos,0));
			iStartPos+=iNextPos;
		}
		WidgetFun::setPos(this,"HNMJButton_GuoAction",cocos2d::Vec2(iStartPos,0));

		WidgetFun::setVisible(this,"SelfActionNode",true);
		WidgetFun::setVisible(this,"HNMJButton_GangAction",(bool)(BYTE(iUserAction)&WIK_GANG));
		WidgetFun::setVisible(this,"HNMJButton_PengAction",(bool)(BYTE(iUserAction)&WIK_PENG));
		WidgetFun::setVisible(this,"HNMJButton_HuAction",(bool)(BYTE(iUserAction)&WIK_CHI_HU) || (bool)(BYTE(iUserAction)&WIK_ZI_MO));
		WidgetFun::setVisible(this,"HNMJButton_XiaoHuAction",(bool)(BYTE(iUserAction)&WIK_XIAO_HU));
		WidgetFun::setVisible(this,"HNMJButton_ChiAction",(bool)(BYTE(iUserAction)&WIK_LEFT) 
			|| (bool)(BYTE(iUserAction)&WIK_CENTER)
			|| (bool)(BYTE(iUserAction)&WIK_RIGHT));

		if (WidgetFun::isWidgetVisble(this,"HNMJButton_ChiAction"))
		{
			WidgetFun::setWidgetUserInfo(this,"HNMJButton_ChiAction","Chi_Type",utility::toString(iUserAction));
		}
	}
}

void HNMJGameScence::showJieSuanInfo(CMD_S_GameEnd* pGameEnd)
{
	cocos2d::Node* pJieSuanNode = WidgetFun::getChildWidget(this,"GameJieSuanNode");
	pJieSuanNode->runAction(cocos2d::CCVisibleAction::create(3.0f,true));

	 WidgetFun::setPlaceText(pJieSuanNode,"JieSuan_JuShu",utility::toString(getPlayCount()));
	 setJieSuanNiaoCard(pJieSuanNode,pGameEnd->cbCardDataNiao,pGameEnd->cbNiaoCount);
	 
	 bool isLiuJu = true;
	 bool isZimo = false;

	 for (int i=0;i<MAX_PLAYER;i++)
	 {
		int iChirID = m_pPlayer[i]->GetChairID();
		int nHuScore = pGameEnd->lGameScore[iChirID] - pGameEnd->lStartHuScore[iChirID]-pGameEnd->lGangScoreInfo[iChirID];

		cocos2d::Node* pJieSuanInfo = WidgetFun::getChildWidget(pJieSuanNode,utility::toString("JieSuanInfo",i));
		WidgetFun::setText(pJieSuanInfo,"Jiesuan_Nicke",m_pPlayer[i]->GetNickName());
		WidgetFun::setText(pJieSuanInfo,"Jiesuan_GoldNum",pGameEnd->lGameScore[iChirID]);

		WidgetFun::setVisible(pJieSuanInfo,"JieSuan_Zhuang",false);
		WidgetFun::setVisible(pJieSuanInfo,"JieSuan_Hu",false);
		WidgetFun::setVisible(pJieSuanInfo,"Jiesuan_HuType",false);
		if (nHuScore>0 )
		{
			WidgetFun::setVisible(pJieSuanInfo,"JieSuan_Hu",true);
			if (pGameEnd->wProvideUser[iChirID] == iChirID )
			{
				isZimo = true;
			}
		}
		if (iChirID == m_iBankerUser)
		{
			WidgetFun::setVisible(pJieSuanInfo,"JieSuan_Zhuang",true);
		}
		cocos2d::Node* pCardNode = WidgetFun::getChildWidget(pJieSuanInfo,"JieSuanCardNode");
		m_pPlayer[i]->showJieSuanCard(pCardNode,pGameEnd->WeaveItemArray[iChirID],pGameEnd->cbWeaveCount[iChirID],
			pGameEnd->cbCardData[iChirID],pGameEnd->cbCardCount[iChirID]);
		
		if(pGameEnd->lGameScore[iChirID] != 0)
		{
			isLiuJu = false;
		}
	 }

	 for (int i=0;i<MAX_PLAYER;i++)
	 {
		 int iChirID = m_pPlayer[i]->GetChairID();
		 cocos2d::Node* pJieSuanInfo = WidgetFun::getChildWidget(pJieSuanNode,utility::toString("JieSuanInfo",i));
		 WidgetFun::setVisible(pJieSuanInfo,"Jiesuan_HuType",true);

		 WidgetFun::setNodeKeepAfter(pJieSuanInfo,WidgetFun::getChildWidget(pJieSuanInfo,"Jiesuan_Nicke"),"Jiesuan_HuType");
		 std::string kHuType;
		 if(pGameEnd->lGameScore[iChirID] >0 && isZimo)
		 {
			kHuType = ScriptData<std::string>("HU_TYPE_ZIMO").Value();
			if (pGameEnd->cbNiaoCount>0)
			{
				kHuType += utility::toString(ScriptData<std::string>("ZHONG_NIAO").Value(),(int)pGameEnd->cbNiaoPick,ScriptData<std::string>("ZHONG_GE").Value());
			}
		 }
		 else if(pGameEnd->lGameScore[iChirID] >0 && !isZimo && pGameEnd->lStartHuScore[iChirID]  != pGameEnd->lGameScore[iChirID])
		 {
			 kHuType = ScriptData<std::string>("HU_TYPE_JIEPAO").Value();
			 if (pGameEnd->cbNiaoCount>0)
			 {
				 kHuType += utility::toString(ScriptData<std::string>("ZHONG_NIAO").Value(),(int)pGameEnd->cbNiaoPick,ScriptData<std::string>("ZHONG_GE").Value());
			 }
		 }
		 else if (pGameEnd->lGameScore[iChirID] <0 && !isZimo 
			 && pGameEnd->lStartHuScore[iChirID]  != pGameEnd->lGameScore[iChirID] 
			 && pGameEnd->lGangScoreInfo[iChirID]  != pGameEnd->lGameScore[iChirID])
		 {
			kHuType = ScriptData<std::string>("HU_TYPE_FANGPAO").Value();
		 }
		 if (pGameEnd->lStartHuScore[iChirID] >0)
		 {
			 kHuType += getStringHuRight(pGameEnd->dwStartHuRight[iChirID]);
		 }
		 if (pGameEnd->lGameScore[iChirID] >0)
		 {
			 kHuType += getStringHuRight(pGameEnd->dwChiHuRight[iChirID]);
		 }

		 WidgetFun::setText(pJieSuanInfo,"Jiesuan_HuType",kHuType);
	 }

	 int nChairID = m_pLocal->GetChairID();
	 cocos2d::Node* pJieSuanTitle = WidgetFun::getChildWidget(pJieSuanNode,"JieSuan_Title");
	 cocos2d::Node* pJieSuanNPC = WidgetFun::getChildWidget(pJieSuanNode,"JieSuan_NPC");
	 if (pGameEnd->lGameScore[nChairID]>0)
	 {
		 std::string kTitleWin = WidgetFun::getWidgetUserInfo(pJieSuanTitle,"Title_Win");
		 std::string kNPCXiao = WidgetFun::getWidgetUserInfo(pJieSuanNPC,"NPC_Smile");

		 WidgetFun::setImagic(pJieSuanTitle,kTitleWin,false);
		 WidgetFun::setImagic(pJieSuanNPC,kNPCXiao,false);
	 }
	 else if (pGameEnd->lGameScore[nChairID]== 0)
	 {
		 std::string kTitlePing = WidgetFun::getWidgetUserInfo(pJieSuanTitle,"Title_Ping");
		 if (isLiuJu == true)
		 {
			 kTitlePing =  WidgetFun::getWidgetUserInfo(pJieSuanTitle,"Title_LiuJu");
		 }
		 std::string kNPCXiao = WidgetFun::getWidgetUserInfo(pJieSuanNPC,"NPC_Smile");

		 WidgetFun::setImagic(pJieSuanTitle,kTitlePing,false);
		 WidgetFun::setImagic(pJieSuanNPC,kNPCXiao,false);
	 }
	 else if (pGameEnd->lGameScore[nChairID] < 0)
	 {
		 std::string kTitleLose = WidgetFun::getWidgetUserInfo(pJieSuanTitle,"Title_Lose");
		 std::string kNPCKu = WidgetFun::getWidgetUserInfo(pJieSuanNPC,"NPC_Cry");

		 WidgetFun::setImagic(pJieSuanTitle,kTitleLose,false);
		 WidgetFun::setImagic(pJieSuanNPC,kNPCKu,false);
	 }

}
void HNMJGameScence::setJieSuanNiaoCard( cocos2d::Node* pNode,BYTE* pNiaoCard,BYTE cbCardNum )
{
	ASSERT(pNode);

	cocos2d::Node* pNiaoNode = WidgetFun::getChildWidget(pNode,"JieSuan_CardNiao");
 	pNiaoNode->removeAllChildren();

	int iIdex = 0;
	int iOder = 0;
	cocos2d::Vec2 kStartPos = cocos2d::Vec2(0,0);
	std::string kHandSkin = utility::toString("HNMJ_HAND_",iIdex);
	cocos2d::Vec2 kHandAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pNiaoNode,"JieSuanAddPos"));
	for (int i=0;i<cbCardNum;i++)
	{
		ASSERT(HNMJ::CGameLogic::Instance().IsValidCard(pNiaoCard[i]));

		cocos2d::Node* pCard = WidgetManager::Instance().createWidget(kHandSkin,pNiaoNode);
		pCard->setTag(1);
		pCard->setZOrder(iOder);
		pCard->setPosition(kStartPos);
		{
			kStartPos += kHandAddPos;
		}
		if (iIdex == 0)
		{
			std::string kImagic = WidgetFun::getWidgetUserInfo(pCard,"MingImagic");
			m_pLocal->setCardImagic(pCard,pNiaoCard[i],kImagic);
		}
		iOder++;
	}

}


void HNMJGameScence::setGameState(int nState)
{
	m_nGameState = nState;
}
int HNMJGameScence::getGameState()
{
	return m_nGameState;
}

void HNMJGameScence::setPlayCount(int nCount)
{
	m_nPlayCount = nCount;
}
int HNMJGameScence::getPlayCount()
{
	return m_nPlayCount;
}
