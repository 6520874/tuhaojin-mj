#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Kernel/kernel/server/IServerPrivateSink.h"

class HNPrivateScenceHN
	:public cocos2d::Node
	,public IServerPrivateSink
	,public FvSingleton<HNPrivateScenceHN> 
{
public:
	HNPrivateScenceHN();
	~HNPrivateScenceHN();
public:
	void hideAll();
	void setPlayerCoutIdex(int iGameType,int iIdex);
	void setGameRuleIdex(int iGameType,int iIdex);
	void upJoinNumTxt();
public:
	virtual bool init();
public:
	virtual void OnSocketSubPrivateInfo(CMD_GR_Private_Info* pNetInfo);
	virtual void OnSocketSubPrivateCreateSuceess(CMD_GR_Create_Private_Sucess* pNetInfo);
	virtual void OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo);
	virtual void OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo);
	virtual void OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo);
public:
	void Button_JoinNum(cocos2d::Ref*,WidgetUserInfo*);
	void Button_JoinNumDel(cocos2d::Ref*,WidgetUserInfo*);
	void Button_JoinNumReset(cocos2d::Ref*,WidgetUserInfo*);

	void Button_Show_Create_Private(cocos2d::Ref*,WidgetUserInfo*);
	void Button_Show_Join_Private(cocos2d::Ref*,WidgetUserInfo*);
	
	void Button_ZZCreateRoom(cocos2d::Ref*,WidgetUserInfo*);
	void Button_CSCreateRoom(cocos2d::Ref*,WidgetUserInfo*);
	void Button_XZCreateRoom(cocos2d::Ref*,WidgetUserInfo*);
	void Button_SureeDismissPrivate(cocos2d::Ref*,WidgetUserInfo*);
	void Button_DismissPrivate(cocos2d::Ref*,WidgetUserInfo*);
	void Button_DismissPrivateNot(cocos2d::Ref*,WidgetUserInfo*);
	void Button_PrivateAgagin(cocos2d::Ref*,WidgetUserInfo*);
	void Button_SetGameType(cocos2d::Ref*,WidgetUserInfo*);
	
	void Button_PrivatePlayCout(cocos2d::Ref*,WidgetUserInfo*);
	void Button_ZZGameRule(cocos2d::Ref*,WidgetUserInfo*);
	
private:
	int m_iGameType;

	int m_iPlayCoutIdex[2];
	dword m_dwPlayRule[2];

	int	 m_iJoinCout;
	char m_kJoinNumTxt[7];

	CMD_GF_Private_Room_Info m_kPrivateInfo;
};