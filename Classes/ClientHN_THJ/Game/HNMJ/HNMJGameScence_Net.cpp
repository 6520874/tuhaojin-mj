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

	HNMJButtonAction_ShowCard(NULL,NULL);
	getPlayerByChairID(m_iBankerUser)->setZhuang();
	if (pGameStart->cbXiaoHuTag == 1)
	{
		setGameState(HNMJ_STATE_XIAO_HU);
	}
	else
	{
		setGameState(HNMJ_STATE_PLAYING);
	}

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
	pPlayer->sendOutCard(pOutCard->cbOutCardData);

	defaultPlayerActionState();
}
//发牌消息
void HNMJGameScence::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) 
		return;

	WidgetFun::setVisible(this,"SelfActionNode",false);
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
	setGameState(HNMJ_STATE_PLAYING);
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

	HNMJPlayer* pProvidePlayer = getPlayerByChairID(pOperateResult->wProvideUser);
	if (pProvidePlayer &&(pOperateResult->cbOperateCode == WIK_PENG 
		|| pOperateResult->cbOperateCode == WIK_LEFT
		|| pOperateResult->cbOperateCode == WIK_CENTER
		|| pOperateResult->cbOperateCode == WIK_RIGHT
		|| (pOperateResult->cbOperateCode == WIK_GANG && pOperateResult->wOperateUser != pOperateResult->wProvideUser )))
	{
		pProvidePlayer->removeHandOutCard(pOperateResult->cbOperateCard);
		pProvidePlayer->setActOutCard(-1);
	}

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

	showJieSuanInfo(pGameEnd);
	setGameState(HNMJ_STATE_NULL);
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		int iChirID = m_pPlayer[i]->GetChairID();
		m_pPlayer[i]->showCard(pGameEnd->cbCardData[iChirID],pGameEnd->cbCardCount[iChirID]);	
	}
// 	WidgetManager::Instance().ButtonRelease(WidgetFun::getChildWidget(this,"Button_ResoultPlayer0"));
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
	pPlayer->setChiHuCard(pChiHu->cbChiHuCard);
	if (pChiHu->wChiHuUser != pChiHu->wProviderUser)
	{
		HNMJPlayer* pDestPlayer = getPlayerByChairID(pChiHu->wProviderUser);
		pPlayer->addNewInCard(pChiHu->cbChiHuCard);
		pPlayer->showHuPai(false);
		pPlayer->showStatusImagic("Hu");
		pDestPlayer->showAddGold(-pChiHu->lGameScore);
		pDestPlayer->removeHandOutCard(pChiHu->cbChiHuCard);
		pDestPlayer->setActOutCard(-1);
		pDestPlayer->runAniDianPao();
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
		pPlayer->runAniZiMo();
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