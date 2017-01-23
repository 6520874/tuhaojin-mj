#pragma once
#include "cocos2d.h"
#include "Kernel/kernel/user/IClientUserItem.h"

class GamePlayer
{
public:
	GamePlayer(IClientUserItem* pUserItem);
	~GamePlayer();
public:
	void setUserItem(IClientUserItem* pItem);
	IClientUserItem* getUserItem(bool bAssert = true);
public:
	word GetTableID();
	word GetChairID();
	byte GetUserStatus();
	SCORE GetUserScore();
	word GetFaceID();
	byte GetGender();
	dword GetUserID();
	dword GetGameID();
	std::string GetHeadHttp();
	//�û��ǳ�
	std::string GetNickName();
	//�û���Ϣ
	tagUserInfo * GetUserInfo();
public:
	virtual void PlayerEnter(){};
	virtual void PlayerLeave(){};
	virtual void upPlayerInfo(){};
	virtual void upPlayerState(){};
protected:
	IClientUserItem* m_pUserItem;
};