#pragma once
#include "cocos2d.h"
#include "Platform/PlatformHeader.h"
#include "Kernel/network/CCallMission.h"

//////////////////////////////////////////////////////////////////////////
// 机器绑定修改提示
//////////////////////////////////////////////////////////////////////////
class CGExchangeHuaFeiMissionSink
{
public:
	virtual ~CGExchangeHuaFeiMissionSink(){}
	virtual void onGPExchangeHuaFeiListResult(CMD_GP_BackExchangeHuaFei* pNetInfo){}
};

//////////////////////////////////////////////////////////////////////////
// 兑换道具
//////////////////////////////////////////////////////////////////////////
class CGPExchangeHuaFeiMission
	: public CCallMission
{
public:
	CGPExchangeHuaFeiMission(const char* url, int port);
	// 设置回调接口
	void setMissionSink(CGExchangeHuaFeiMissionSink* pIGPKefuMissionSink);

	void GetExchangeList();
	void CB_ExchangeList();

	void Net_ExchangeHuaFeiList(void* data, int dataSize);
private:
	// 回调
	CGExchangeHuaFeiMissionSink* mIGPExchangeHuaFeiMissionSink;
}; 
