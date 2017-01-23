#include "CGPGameRecord.h"


CGPGameRecordMission::CGPGameRecordMission(const char* url, int port)
: CCallMission("CGPGameRecordMission",url, port)
{
	mIGPGameRecordSink = 0;

	addNetCall(CC_CALLBACK_2(CGPGameRecordMission::Net_GetGameRecordList,this),SUB_GP_GAME_RECORD_LIST);
	addNetCall(CC_CALLBACK_2(CGPGameRecordMission::Net_PrivateRandTotalRecord,this),SUB_GP_GAME_RECORD_TOTAL);
}

// 设置回调接口
void CGPGameRecordMission::setMissionSink(CGGameRecordSink* pIGPKefuMissionSink)
{
	mIGPGameRecordSink = pIGPKefuMissionSink;
}

void CGPGameRecordMission::GetGameRecordList(int iUserID)
{
	PLAZZ_PRINTF("CGPGameRecordMission::GetGameRecordList\n");

	addLinkCallFun(CC_CALLBACK_0(CGPGameRecordMission::CB_GetGameRecordList,this,iUserID));

	start();
}
void CGPGameRecordMission::CB_GetGameRecordList(int iUserID)
{
	PLAZZ_PRINTF("CGPGameRecordMission::CB_GetGameRecordList\n");
	CMD_GP_GetGameRecordList kNetInfo;
	kNetInfo.dwUserID = iUserID;

	send(MDM_GP_USER_SERVICE,SUB_GP_GAME_RECORD_LIST,&kNetInfo,sizeof(kNetInfo));
}
void CGPGameRecordMission::Net_GetGameRecordList(void* data, int dataSize)
{
	PLAZZ_PRINTF("CGPGameRecordMission::Net_GetGameRecordList\n");

	datastream kDataStream(data,dataSize);
	tagPrivateRandTotalRecordList kNetInfo;
	kNetInfo.StreamValue(kDataStream,false);

	stop();

	if (mIGPGameRecordSink)
	{
		mIGPGameRecordSink->onGPBackGameRecordList(&kNetInfo);
	}
}
void CGPGameRecordMission::GetPrivateRandTotalRecord(int iRecordID)
{
	PLAZZ_PRINTF("CGPGameRecordMission::GetGameRecordVideo\n");

	addLinkCallFun(CC_CALLBACK_0(CGPGameRecordMission::CB_PrivateRandTotalRecord,this,iRecordID));

	start();
}
void CGPGameRecordMission::CB_PrivateRandTotalRecord(int iRecordID)
{
	CMD_GP_GetGameTotalRecord kNetInfo;
	kNetInfo.dwUserID = 0;
	kNetInfo.dwRecordID = iRecordID;
	send(MDM_GP_USER_SERVICE, SUB_GP_GAME_RECORD_TOTAL,&kNetInfo,sizeof(CMD_GP_GetGameTotalRecord));

}
void CGPGameRecordMission::Net_PrivateRandTotalRecord(void* data, int dataSize)
{
	PLAZZ_PRINTF("CGPAddRankMission::Net_AddBank\n");

	datastream kStream(data,dataSize);
	tagPrivateRandTotalRecord kNetInfo;
	kNetInfo.StreamValue(kStream,false);

	stop();

	if (mIGPGameRecordSink)
	{
		mIGPGameRecordSink->onGPBackGameTotalRecord(&kNetInfo);
	}
}
