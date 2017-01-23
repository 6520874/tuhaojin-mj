#include "HNHomeScence.h"

#include "Game/Widget/WidgetManager.h"

#include "ClientHN/HNGameManager.h"
#include "GameLib/Game/GameLib.h"

#include "HNScenceManager.h"

void HNHomeScence::init_Button()
{
	WidgetManager::addButtonCB("Button_GameRoom",this,button_selector(HNHomeScence::Button_GameRoom));
	WidgetManager::addButtonCB("Button_BackHome",this,button_selector(HNHomeScence::Button_BackHome));
	WidgetManager::addButtonCB("Button_HomeHead",this,button_selector(HNHomeScence::Button_HomeHead));
	
}
void HNHomeScence::Button_GameRoom(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iServerID = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"GameServerID"));
	HNGameManager::Instance().connectGameServerByServerID(iServerID);
}
void HNHomeScence::Button_BackHome(cocos2d::Ref*,WidgetUserInfo*)
{
	HNGameManager::Instance().disconnectServer();
	HNScenceManager::Instance().InHomeScence();
}
void HNHomeScence::Button_HomeHead(cocos2d::Ref*,WidgetUserInfo*)
{
	PopScence::Instance().showIPInfo(
		UserInfo::Instance().getUserNicName(),
		utility::paseInt(UserInfo::Instance().getUserID(),6),
		UserInfo::Instance().getUserIP(),
		UserInfo::Instance().getHeadHttp());
}