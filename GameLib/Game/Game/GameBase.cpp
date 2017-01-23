#include "GameBase.h"

#include "Game/GameLib.h"
#include "Game/Sound/ArmFun.h"

#include "ScenceManagerBase.h"
#include "GameManagerBase.h"

GameBase::GameBase()
	:m_pSelfPlayer(NULL)
	,m_bEnterGetUserInfo(false)
	,m_kIndividualMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	m_kIndividualMission.setMissionSink(this);
}
GameBase::~GameBase()
{

}
bool GameBase::init()
{
	if (!Node::init())
	{
		return false;
	};
	return true;
}
void GameBase::Button_ExitGameBase(cocos2d::Ref*,WidgetUserInfo*)
{
	if (IServerItem::get())
	{
		IServerItem::get()->PerformStandUpAction();
	}
	else
	{
		ExitGame();
	}
}
void GameBase::clearInfo()
{
	while (m_kPlayers.size())
	{
		GamePlayer* pTempPlayer = m_kPlayers[0];
		removeGamePlayerToList(pTempPlayer);
		DeletePlayer(pTempPlayer);
	}
}
void GameBase::ExitGame()
{
	clearInfo();
	m_kIndividualMission.stop();
	ScenceManagerBase::InstanceBase().GameBackScence();
	UserInfo::Instance().reqAccountInfo();
	GameManagerBase::InstanceBase().disconnectServer();
}

void GameBase::OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo)
{

}
void GameBase::OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo)
{

}
void GameBase::OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo)
{

}
void GameBase::sendTalkFile(int iChair,std::string kFile)
{
	ssize_t iSize = 0;
	std::string kDestFile = cocos2d::CCFileUtils::getInstance()->getWritablePath()+"talk.arm";
	ArmFun::WavToArm(kFile.c_str(),kDestFile.c_str());
	CMD_GR_C_TableTalk kNetInfo;
	unsigned char* pData = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(kDestFile,"rb",&iSize);
	if (!pData)
	{
		return;
	}
	kNetInfo.cbChairID = iChair;
	kNetInfo.kDataStream.pushValue((char*)pData, iSize);
	free(pData);
	datastream kDataStream;
	kNetInfo.StreamValue(kDataStream,true);
	IServerItem::get()->SendSocketData(MDM_GF_FRAME,SUB_GR_TABLE_TALK,&kDataStream[0],kDataStream.size());
}
bool GameBase::RevTalkFile(void* data,int dataSize)
{
	datastream kDataStream(data,dataSize);
	CMD_GR_C_TableTalk kTalkNet;
	kTalkNet.StreamValue(kDataStream,false);
	if (kTalkNet.kDataStream.size() == 0)
	{
		return true;
	}
	static int iIdex = 0;
	iIdex ++;
	std::string kFile = utility::toString(cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath(),"TableTalk",iIdex,".arm");
	FILE *fp = fopen(kFile.c_str(), "wb");

	fseek(fp,0,SEEK_END);
	fseek(fp,0,SEEK_SET);
	fwrite(&kTalkNet.kDataStream[0],sizeof(unsigned char), kTalkNet.kDataStream.size(),fp);
	fclose(fp);

	std::string kDestFile = kFile;
	utility::StringReplace(kDestFile,"arm","wav");
	ArmFun::ArmToWav(kFile.c_str(),kDestFile.c_str());

	SoundFun::playEffectDirect(kDestFile);
	return true;
}
