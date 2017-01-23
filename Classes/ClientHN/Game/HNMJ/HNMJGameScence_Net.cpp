#include "HNMJGameScence.h"
#include "Game/GameLib.h"

#include "CMD_HNMJ.h"
#include "HNMJPlayer.h"
#include "HNMJGameLogic.h"

void HNMJGameScence::initNet()
{
	addNetCBDefine(SUB_S_GAME_START,this,HNMJGameScence::OnSubGameStart);
	addNetCBDefine(SUB_S_OUT_CARD,this,HNMJGameScence::OnSubOutCard);
	addNetCBDefine(SUB_S_SEND_CARD,this,HNMJGameScence::OnSubSendCard);
	addNetCBDefine(SUB_S_OPERATE_NOTIFY,this,HNMJGameScence::OnSubOperateNotify);
	addNetCBDefine(SUB_S_OPERATE_RESULT,this,HNMJGameScence::OnSubOperateResult);
	addNetCBDefine(SUB_S_GAME_END,this,HNMJGameScence::OnSubGameEnd);
	addNetCBDefine(SUB_S_TRUSTEE,this,HNMJGameScence::OnSubTrustee);
	addNetCBDefine(SUB_S_CHI_HU,this,HNMJGameScence::OnSubUserChiHu);
	addNetCBDefine(SUB_S_GANG_SCORE,this,HNMJGameScence::OnSubGangScore);
}
//游戏开始
void HNMJGameScence::OnSubGameStart(const void * pBuffer, word wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) 
		return;

	//变量定义
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	defaultState();

	m_iBankerUser=pGameStart->wBankerUser;
	m_iCurrentUser=pGameStart->wCurrentUser;
	m_iUserAction = pGameStart->cbUserAction;

	WidgetFun::setText(this,"LastCardCout",(int)pGameStart->cbLeftCardCount);

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->setHandCard(pGameStart->cbCardData,MAX_COUNT-1);
		m_pPlayer[i]->startGame();
	}

	HNMJPlayer* pPlayer = getPlayerByChairID(m_iCurrentUser);
	if (pPlayer == m_pLocal)
	{
		pPlayer->addNewInCard(pGameStart->cbCardData[MAX_COUNT-1]);
	}
	else
	{
		pPlayer->addNewInCard(0);
	}
	showSaiZi(pGameStart->lSiceCount);

}
//用户出牌
void HNMJGameScence::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) 
		return;

	//消息处理
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->setActOutCard(-1);
	}

	HNMJPlayer* pPlayer = getPlayerByChairID(pOutCard->wOutCardUser);
	pPlayer->addHandOutCard(pOutCard->cbOutCardData);
	pPlayer->setActOutCard(pOutCard->cbOutCardData);

	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"TimeNode");
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		WidgetFun::setVisible(pRootNode,utility::toString("TimePoint",i),false);
	}
	WidgetFun::getChildWidget(pRootNode,"ActPlayerLastTime")->stopAllActions();
	WidgetFun::setText(pRootNode,"ActPlayerLastTime","0");
}
//发牌消息
void HNMJGameScence::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) 
		return;

	//变量定义
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;
	setCurrentPlayer(pSendCard->wCurrentUser,pSendCard->cbActionMask);
	WidgetFun::setWidgetUserInfo(this,"NotifyCard",utility::toString((int)pSendCard->cbCardData));
	HNMJPlayer* pPlayer = getPlayerByChairID(pSendCard->wCurrentUser);
	if (pSendCard->cbCardData != 0)
	{
		pPlayer->addNewInCard(pSendCard->cbCardData);
		pPlayer->showHandCard();
		int iLastCardCout = utility::parseInt(WidgetFun::getText(this,"LastCardCout")) - 1;
		WidgetFun::setText(this,"LastCardCout",iLastCardCout);
	}
}
//操作提示
void HNMJGameScence::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) 
		return;

	//变量定义
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

	setCurrentPlayer(m_pLocal->GetChairID(),pOperateNotify->cbActionMask);
	if (pOperateNotify->cbActionCard != 0)
	{
		WidgetFun::setWidgetUserInfo(this,"NotifyCard",utility::toString((int)pOperateNotify->cbActionCard));
	}
}
//操作结果
void HNMJGameScence::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) 
		return ;

	//消息处理
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

	HNMJPlayer* pPlayer = getPlayerByChairID(pOperateResult->wOperateUser);
	pPlayer->setOperateResoult(pOperateResult);

	setCurrentPlayer(pOperateResult->wOperateUser,WIK_NULL);
	WidgetFun::setVisible(this,"SelfActionNode",false);
}
//游戏结束
void HNMJGameScence::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) 
		return;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	WidgetFun::setVisible(this,"FreeStateNode",true);
	WidgetFun::setVisible(this,"GameResoult2",true);
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"GameResoult2");
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		int iChirID = m_pPlayer[i]->GetChairID();
		WidgetFun::setButtonTxt(pRootNode,utility::toString("Button_ResoultPlayer",i),utility::a_u8(m_pPlayer[i]->GetNickName()));
		WidgetFun::setText(pRootNode,utility::toString("GoldTxt",i),pGameEnd->lGameScore[iChirID]);
		setGameResoultPlayerInfo(pGameEnd,m_pPlayer[i],WidgetFun::getChildWidget(pRootNode,utility::toString("ResoultPlayer",i)));
	}
	WidgetManager::Instance().ButtonRelease(WidgetFun::getChildWidget(this,"Button_ResoultPlayer0"));
}
//用户托管
void HNMJGameScence::OnSubTrustee(const void * pBuffer,WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_Trustee));
	if (wDataSize!=sizeof(CMD_S_Trustee)) 
		return;

	//消息处理
	CMD_S_Trustee * pTrustee=(CMD_S_Trustee *)pBuffer;
}
//
void HNMJGameScence::OnSubUserChiHu( const void *pBuffer,WORD wDataSize )
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_ChiHu));
	if (wDataSize!=sizeof(CMD_S_ChiHu))
		return;

	//消息处理
	CMD_S_ChiHu * pChiHu=(CMD_S_ChiHu *)pBuffer;

	HNMJPlayer* pPlayer = getPlayerByChairID(pChiHu->wChiHuUser);
	pPlayer->showAddGold(pChiHu->lGameScore);
	pPlayer->showEffect("Hu");
	if (pChiHu->wChiHuUser != pChiHu->wProviderUser)
	{
		HNMJPlayer* pDestPlayer = getPlayerByChairID(pChiHu->wProviderUser);
		pPlayer->addHandOutCard(pChiHu->cbChiHuCard);
		pPlayer->showHuPai(false);
		pPlayer->showStatusImagic("Hu");
		pDestPlayer->showAddGold(-pChiHu->lGameScore);
	}
	else
	{
		for (int i = 0;i<MAX_PLAYER;i++)
		{
			if (i == pChiHu->wChiHuUser)
			{
				continue;
			}
			m_pPlayer[i]->showAddGold(-pChiHu->lGameScore/3);
		}
		pPlayer->showHuPai(true);
		pPlayer->showStatusImagic("ZiMo");
	}
}
//
void HNMJGameScence::OnSubGangScore( const void *pBuffer, WORD wDataSize )
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_GangScore));
	if (wDataSize!=sizeof(CMD_S_GangScore)) 
		return;

	//消息处理
	CMD_S_GangScore * pGangScore=(CMD_S_GangScore *)pBuffer;

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		HNMJPlayer* pPlayer = getPlayerByChairID(i);
		pPlayer->showAddGold(pGangScore->lGangScore[i]);
	}
}