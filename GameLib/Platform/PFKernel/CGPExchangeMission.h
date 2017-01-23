#pragma once

#include "cocos2d.h"
#include "Platform/PlatformHeader.h"
#include "Kernel/network/CSocketMission.h"

//////////////////////////////////////////////////////////////////////////
// 机器绑定修改提示
//////////////////////////////////////////////////////////////////////////
class IGPExchangeMissionSink
{
public:
	virtual ~IGPExchangeMissionSink(){}
	virtual void onGPExchangeResult(int lResultCode, const char* szDescription){}
};

//////////////////////////////////////////////////////////////////////////
// 兑换道具
//////////////////////////////////////////////////////////////////////////
class CGPExchangeMission
	: public CSocketMission
{
public:
	CGPExchangeMission(const char* url, int port);
	// 设置回调接口
	void setMissionSink(IGPExchangeMissionSink* pIGPExchangeMissionSink);
	// 兑换道具
	void exchange(uint16 item);
	//////////////////////////////////////////////////////////////////////////
	// ISocketEngineSink
public:
	virtual void onEventTCPSocketLink();
	virtual void onEventTCPSocketShut();
	virtual void onEventTCPSocketError(int errorCode);
	virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);
private:
	// 兑换结果
	bool onSubExchangeSuccess(void* data, int size);
	
private:
	// 回调
	IGPExchangeMissionSink* mIGPExchangeMissionSink;

	uint8 mItem;
}; // CGPExchangeMission
