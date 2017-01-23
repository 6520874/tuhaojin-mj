
#pragma once

#include "Game/Game/GamePlayer.h"
#include "GameLib/Game/GameLib.h"
#include "cocos2d.h"
#include "CMD_HNMJ.h"

//游戏逻辑
class HNMJPlayer:public GamePlayer
{
public:
	HNMJPlayer(int iIdex,cocos2d::Node* pSeatNode);
	~HNMJPlayer();

	void init();
	int  getIdex();
	void defaultState();
	void startGame();
	void EndGame();
	void setZhuang();
	void showAddGold(int iGold);
	void showEffect(std::string kKey);
	void showStatusImagic(std::string kKey);
public:
	virtual void PlayerEnter();
	virtual void PlayerLeave();
	virtual void upPlayerInfo();
	virtual void upPlayerState();
public:
	void setHandCard(BYTE* pCardData,int iCout);
	void setWeaveItem(CMD_WeaveItem* pWeave,int iCout);
	void setOperateResoult(CMD_S_OperateResult* pNetInfo);
	void addNewInCard(int iCard);
	void setActOutCard(int iCard);
	void addHandOutCard(int iCard);
	void showHandCard();
	void showHandCard(CMD_WeaveItem* pWeave,int iWeaveCout,BYTE* pHandCard,int iHandCout);
	void setCardImagic(cocos2d::Node* pNode,int kValue,std::string kImagicFront);
	cocos2d::Node* getTouchCardNode(cocos2d::Vec2 kTouchPos);
	BYTE getTouchCardVlaue(cocos2d::Node* pNode);
	void showHuPai(bool bZiMo);
private:
	int						m_iIdex;
	cocos2d::Node*			m_pSeatNode;

	BYTE					m_kHandCardData[MAX_COUNT];
	BYTE					m_kHandCardCout;

	CMD_WeaveItem			m_kWeaveItemArray[MAX_WEAVE];		//组合扑克
	BYTE					m_kWeaveCount;					//组合数目

	int						m_iOutCardCout;
};

