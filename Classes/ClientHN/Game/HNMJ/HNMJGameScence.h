#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Game/Game/GameBase.h"
#include "Game/Game/GameDefine.h"


class HNMJPlayer;
struct CMD_S_GameEnd;

class HNMJGameScence
	:public GameBase
	,public FvSingleton<HNMJGameScence> 
{
public:
	const static int KIND_ID = 310;
	const static int VERSION_SERVER	= PROCESS_VERSION(6,0,3);				//����汾
	const static int VERSION_CLIENT	= PROCESS_VERSION(6,0,3);	
	const static int MAX_PLAYER = 4;
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
	void showSaiZi(unsigned int iValue);
	std::string getStringHuRight(dword kValue);
	void setCurrentPlayer(int iCurrentPlayer,int iUserAction);
	void setGameResoultPlayerInfo(CMD_S_GameEnd* pGameEnd,HNMJPlayer* pPlayer,cocos2d::Node* pNode);
	void setGameResoultStateInfo(cocos2d::Node* pNode,int iIdex,std::string kName,std::string kDes,int lSocre);
public:
	void initButton();
	void HNMJButton_Ready(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_GuoAction(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_GangAction(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_PengAction(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_HuAction(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButtonAction_ShowNext(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButtonAction_ShowCard(cocos2d::Ref*,WidgetUserInfo*);
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

	//˽�˳�
public:
	void defaultPrivateState();
	virtual void OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo);
	virtual void OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo);
	virtual void OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo);

	void XZDDButton_WeiXinFriend(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_WeiXinImagic(cocos2d::Ref*,WidgetUserInfo*);
public:
	void initNet();

	void OnSubGameStart(const void * pBuffer, word wDataSize);
	//�û�����
	void OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//������Ϣ
	void OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//������ʾ
	void OnSubOperateNotify(const void * pBuffer, WORD wDataSize);
	//�������
	void OnSubOperateResult(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	void OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//�û��й�
	void OnSubTrustee(const void * pBuffer,WORD wDataSize);
	//�Ժ���Ϣ
	void OnSubUserChiHu( const void *pBuffer,WORD wDataSize );
	//�ܵ÷�
	void OnSubGangScore( const void *pBuffer, WORD wDataSize );
public:
	void Command_PlaceBet(int iArea,int iBetScore);
public:
	void initTouch();
	bool ccTouchBegan(cocos2d::Vec2 kPos);
	void ccTouchMoved(cocos2d::Vec2 kPos);
	void ccTouchEnded(cocos2d::Vec2 kPos);
protected:
	int							m_iBankerUser;						//ׯ���û�
	int							m_iCurrentUser;						//��ǰ�û�
	int							m_iUserAction;						//��Ҷ���

	HNMJPlayer*					m_pLocal;
	HNMJPlayer*					m_pPlayer[MAX_PLAYER];

	cocos2d::Node*				m_pTouchCardNode;
	cocos2d::Vec2				m_kTouchSrcPos;
};