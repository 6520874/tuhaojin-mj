#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Game/Game/GameBase.h"
#include "Game/Game/GameDefine.h"
#include "Game/Script/TimeNode.h"


class HNMJPlayer;
struct CMD_S_GameEnd;

class HNMJGameScence
	:public GameBase
	,public TimeNode
	,public FvSingleton<HNMJGameScence> 
{
public:
	const static int KIND_ID = 310;
	const static int VERSION_SERVER	= PROCESS_VERSION(6,0,3);				//程序版本
	const static int VERSION_CLIENT	= PROCESS_VERSION(6,0,3);	
	const static int MAX_PLAYER = 4;

	enum 
	{
		HNMJ_STATE_NULL,
		HNMJ_STATE_READY,
		HNMJ_STATE_XIAO_HU,
		HNMJ_STATE_PLAYING,
	};
public:
	HNMJGameScence();
	~HNMJGameScence();
public:
	virtual bool init();

	void initPrivate();
public:
	void EnterScence();
	void HideAll();
	void defaultState();
	void defaultPlayerActionState();
	void showSaiZi(unsigned int iValue);
	std::string getStringHuRight(dword kValue);
	void setCurrentPlayer(int iCurrentPlayer,int iUserAction);
	void showJieSuanInfo(CMD_S_GameEnd* pGameEnd);
	void setJieSuanNiaoCard(cocos2d::Node* pNode,BYTE* pNiaoCard,BYTE cbCardNum);
public:
	void initButton();
	void HNMJButton_Ready(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_GuoAction(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_GangAction(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_PengAction(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_ChiAction(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_HuAction(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_XiaoHuAction(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButtonAction_ShowNext(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButtonAction_ShowCard(cocos2d::Ref*,WidgetUserInfo*);

	void HNMJButton_JieSuanShare(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_JieSuanStart(cocos2d::Ref*,WidgetUserInfo*);

	void HNMJButton_TalkBegin(cocos2d::Ref*,WidgetUserInfo*);
	void endButtonTalk();
	void HNMJButton_TalkEnd(cocos2d::Ref*,WidgetUserInfo*);
private:
	virtual bool OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize);
	void OnFreeScence(void* data, int dataSize);
	void OnPlayScence(void* data, int dataSize);
public:
	HNMJPlayer* getPlayerByChairID(int iChairID);
	virtual GamePlayer* CreatePlayer(IClientUserItem * pIClientUserItem);
	virtual void DeletePlayer(GamePlayer* pPlayer);
	virtual void upSelfPlayerInfo();
	virtual void OnGFGameClose(int iExitCode);
	virtual void OnEventUserStatus(GamePlayer * pPlayer);

	//私人场
public:
	void defaultPrivateState();
	virtual void OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo);
	virtual void OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo);
	virtual void OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo);
	void XZDDButton_WeiXinFriend(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_WeiXinImagic(cocos2d::Ref*,WidgetUserInfo*);
	void ButtonPlayerHeadClick(cocos2d::Ref*,WidgetUserInfo* pUserInfo);
public:
	void initNet();

	void OnSubGameStart(const void * pBuffer, word wDataSize);
	//用户出牌
	void OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//发牌消息
	void OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//操作提示
	void OnSubOperateNotify(const void * pBuffer, WORD wDataSize);
	//操作结果
	void OnSubOperateResult(const void * pBuffer, WORD wDataSize);
	//游戏结束
	void OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//用户托管
	void OnSubTrustee(const void * pBuffer,WORD wDataSize);
	//吃胡消息
	void OnSubUserChiHu( const void *pBuffer,WORD wDataSize );
	//杠得分
	void OnSubGangScore( const void *pBuffer, WORD wDataSize );
public:
	void Command_PlaceBet(int iArea,int iBetScore);
	void SendOutCard(cocos2d::Node* pCard);
public:
	void initTouch();
	bool ccTouchBegan(cocos2d::Vec2 kPos);
	void ccTouchMoved(cocos2d::Vec2 kPos);
	void ccTouchEnded(cocos2d::Vec2 kPos);

public:
	void setGameState(int nState);
	int getGameState();
	void setPlayCount(int nCount);
	int getPlayCount();
protected:
	int							m_iBankerUser;						//庄家用户
	int							m_iCurrentUser;						//当前用户
	int							m_iUserAction;						//玩家动作

	HNMJPlayer*					m_pLocal;
	HNMJPlayer*					m_pPlayer[MAX_PLAYER];

	cocos2d::Node*				m_pTouchCardNode;
	cocos2d::Vec2				    m_kTouchSrcPos;

	int                         m_nGameState;
	int                         m_nPlayCount;
};