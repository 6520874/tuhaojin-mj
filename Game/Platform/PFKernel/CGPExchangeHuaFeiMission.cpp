#include "CGPExchangeHuaFeiMission.h"


CGPExchangeHuaFeiMission::CGPExchangeHuaFeiMission(const char* url, int port)
: CCallMission("CGPExchangeHuaFeiMission",url, port)
{
	mIGPExchangeHuaFeiMissionSink = 0;

	addNetCall(CC_CALLBACK_2(CGPExchangeHuaFeiMission::Net_ExchangeHuaFeiList,this),SUB_GP_EXCHANGEHUAFEI_BACK);
}

// 设置回调接口
void CGPExchangeHuaFeiMission::setMissionSink(CGExchangeHuaFeiMissionSink* pIGPKefuMissionSink)
{
	mIGPExchangeHuaFeiMissionSink = pIGPKefuMissionSink;
}

void CGPExchangeHuaFeiMission::GetExchangeList()
{
	addLinkCallFun(CC_CALLBACK_0(CGPExchangeHuaFeiMission::CB_ExchangeList,this));
	start();
}

void CGPExchangeHuaFeiMission::CB_ExchangeList()
{
	CMD_GP_GetExchangeHuaFei kNetInfo;
	kNetInfo.dwUserID = CGlobalUserInfo::getUserID();
	strncpy(kNetInfo.szPassword,CGlobalUserInfo::getPassword(),countarray(kNetInfo.szPassword));
	send(MDM_GP_USER_SERVICE, SUB_GP_EXCHANGEHUAFEI_GET_LIST_INFO,&kNetInfo,sizeof(CMD_GP_GetExchangeHuaFei));
}

void CGPExchangeHuaFeiMission::Net_ExchangeHuaFeiList( void* data, int dataSize )
{
	PLAZZ_PRINTF("CGPExchangeHuaFeiMission::Net_ExchangeHuaFeiList\n");

	datastream kStream(data,dataSize);
	CMD_GP_BackExchangeHuaFei kNetInfo;
	kNetInfo.StreamValue(kStream,false);

	stop();

	if (mIGPExchangeHuaFeiMissionSink)
	{
		mIGPExchangeHuaFeiMissionSink->onGPExchangeHuaFeiListResult(&kNetInfo);
	}
}
