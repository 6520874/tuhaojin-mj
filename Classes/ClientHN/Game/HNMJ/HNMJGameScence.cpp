#include "HNMJGameScence.h"
#include "HNMJPlayer.h"
#include "Game/GameLib.h"
#include "CMD_HNMJ.h"
#include "HNMJPlayer.h"
#include "HNMJGameLogic.h"

FV_SINGLETON_STORAGE(HNMJGameScence);

HNMJGameScence::HNMJGameScence()
	:m_iBankerUser(0)
	,m_iCurrentUser(0)
	,m_iUserAction(0)
	,m_pLocal(NULL)
	,m_pTouchCardNode(NULL)
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
	setVisible(true);
	defaultState();
}
void HNMJGameScence::HideAll()
{
	setVisible(false);
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
	WidgetFun::setVisible(this,"FreeStateNode",false);
	WidgetFun::setVisible(this,"SaiZiNode",false);
	WidgetFun::setVisible(this,"SelfActionNode",false);
	WidgetFun::setVisible(this,"GameResoult1",false);
	WidgetFun::setVisible(this,"GameResoult2",false);

	defaultPrivateState();
}
std::string HNMJGameScence::getStringHuRight(dword kValue)
{
	dword dwRight[] = {
		CHR_SHU_FAN,CHR_TIAN_HU,CHR_DI_HU,
		CHR_DA_DUI_ZI,CHR_QING_YI_SE,CHR_QI_XIAO_DUI,CHR_DAI_YAO,CHR_JIANG_DUI,CHR_QING_DUI,
		CHR_LONG_QI_DUI,CHR_QING_QI_DUI,CHR_QING_YAO_JIU,CHR_QING_LONG_QI_DUI,
		CHR_GANG_SHANG_PAO,CHR_GANG_KAI
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
	utility::log(utility::toString(m_pLocal->GetChairID()," ",iCurrentPlayer," ",iUserAction).c_str());
	m_iCurrentUser = iCurrentPlayer;

	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"TimeNode");

	HNMJPlayer* pPlyer = getPlayerByChairID(m_iCurrentUser);
	WidgetFun::getChildWidget(pRootNode,"ActPlayerLastTime")->runAction(
		cocos2d::MoveExTxtTime::create(TIME_OPERATE_CARD));
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		WidgetFun::setVisible(pRootNode,utility::toString("TimePoint",i),false);
	}

	if (iUserAction == WIK_NULL)
	{
		WidgetFun::setVisible(pRootNode,utility::toString("TimePoint",pPlyer->getIdex()),true);
	}

	if(pPlyer == m_pLocal && iUserAction != WIK_NULL)
	{
		WidgetFun::setVisible(this,"SelfActionNode",true);
		WidgetFun::setVisible(this,"HNMJButton_GangAction",(bool)(BYTE(iUserAction)&WIK_GANG));
		WidgetFun::setVisible(this,"HNMJButton_PengAction",(bool)(BYTE(iUserAction)&WIK_PENG));
		WidgetFun::setVisible(this,"HNMJButton_HuAction",(bool)(BYTE(iUserAction)&WIK_CHI_HU) 
			|| (bool)(BYTE(iUserAction)&WIK_ZI_MO)
			|| (bool)(BYTE(iUserAction)&WIK_XIAO_HU));
	}
}
void HNMJGameScence::setGameResoultStateInfo(cocos2d::Node* pNode,int iIdex,std::string kName,std::string kDes,int lSocre)
{
	cocos2d::Node* pStatusNode = WidgetFun::getChildWidget(pNode,utility::toString("GameResoultInfo",iIdex));
	WidgetFun::setText(pStatusNode,"Name",kName);
	WidgetFun::setText(pStatusNode,"Des",kDes);
	WidgetFun::setText(pStatusNode,"GoldTxt",utility::toString(lSocre));
	pStatusNode->setVisible(true);
}
void HNMJGameScence::setGameResoultPlayerInfo(CMD_S_GameEnd* pGameEnd,HNMJPlayer* pPlayer,cocos2d::Node* pNode)
{
	for (int i = 0;i<6;i++)
	{
		WidgetFun::setVisible(pNode,utility::toString("GameResoultInfo",i),false);
	}
	int iChirID = pPlayer->GetChairID();
	int iIdex = 0;
	if (pGameEnd->lGangScoreInfo[iChirID] != 0)
	{
		setGameResoultStateInfo(pNode,iIdex,m_pPlayer[iChirID]->GetNickName(),utility::getScriptString("GFXY"),pGameEnd->lGangScoreInfo[iChirID]);
		iIdex++;
	}
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		if (pGameEnd->wLostFanShu[i][iChirID] != 0)
		{
			setGameResoultStateInfo(pNode,iIdex,m_pPlayer[i]->GetNickName(),getStringHuRight(pGameEnd->dwChiHuRight[i]),
				-pGameEnd->wLostFanShu[i][iChirID]);
			iIdex++;
		}
	}
	int lSelfSocre = 0;
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		lSelfSocre += pGameEnd->wLostFanShu[iChirID][i] != 0;
	}
	if (lSelfSocre != 0)
	{
		setGameResoultStateInfo(pNode,iIdex,m_pPlayer[iChirID]->GetNickName(),getStringHuRight(pGameEnd->dwChiHuRight[iChirID]),
			lSelfSocre);
		iIdex++;
	}
}