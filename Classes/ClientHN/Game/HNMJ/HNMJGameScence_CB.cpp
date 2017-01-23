#include "HNMJGameScence.h"
#include "Game/GameLib.h"

#include "CMD_HNMJ.h"
#include "HNMJPlayer.h"

bool HNMJGameScence::OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize)
{
	if (cbGameStatus == GS_MJ_FREE)
	{
		OnFreeScence(data,dataSize);
	}

	if (cbGameStatus == GS_MJ_PLAY)
	{
		OnPlayScence(data,dataSize);
	}
	return true;
}
void HNMJGameScence::OnFreeScence(void* data, int wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
	if (wDataSize!=sizeof(CMD_S_StatusFree)) return;

	CMD_S_StatusFree* pNetInfo = (CMD_S_StatusFree*)data;
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() == US_FREE);
}
void HNMJGameScence::OnPlayScence(void* data, int wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
	if (wDataSize!=sizeof(CMD_S_StatusPlay)) return;

	CMD_S_StatusPlay* pNetInfo = (CMD_S_StatusPlay*)data;

	defaultState();

	m_iBankerUser=pNetInfo->wBankerUser;

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		HNMJPlayer* pPlayer = getPlayerByChairID(i);
		pPlayer->setWeaveItem(pNetInfo->WeaveItemArray[i],pNetInfo->cbWeaveCount[i]);
		pPlayer->setHandCard(pNetInfo->cbCardData,13 - pNetInfo->cbWeaveCount[i]*3);

	}

	m_pLocal->setHandCard(pNetInfo->cbCardData,pNetInfo->cbCardCount);

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->showHandCard();
		m_pPlayer[i]->startGame();
	}
	setCurrentPlayer(pNetInfo->wCurrentUser,pNetInfo->cbActionMask);

	HNMJPlayer* pPlayer = getPlayerByChairID(m_iCurrentUser);
	if (pPlayer != m_pLocal)
	{
		pPlayer->addNewInCard(0);
	}

	WidgetFun::setVisible(this,"TimeNode",true);
	WidgetFun::setVisible(this,"LastCardNode",true);
}
void HNMJGameScence::OnGFGameClose(int iExitCode)
{
	ExitGame();
}