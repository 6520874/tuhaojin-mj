#include "CGPExchangeMission.h"

CGPExchangeMission::CGPExchangeMission(const char* url, int port)
: CSocketMission(url, port)
{
	mIGPExchangeMissionSink = 0;
}

// 设置回调接口
void CGPExchangeMission::setMissionSink(IGPExchangeMissionSink* pIGPExchangeMissionSink)
{
	mIGPExchangeMissionSink = pIGPExchangeMissionSink;
}

void CGPExchangeMission::exchange(uint16 item)
{
	mItem = item;
	start();
}

void CGPExchangeMission::onEventTCPSocketLink()
{
	PLAZZ_PRINTF("CGPExchangeMission::onEventTCPSocketLink\n");

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

}

void CGPExchangeMission::onEventTCPSocketShut()
{
	PLAZZ_PRINTF("CGPExchangeMission::onEventTCPSocketShut\n");
}

void CGPExchangeMission::onEventTCPSocketError(int errorCode)
{
	PLAZZ_PRINTF("CGPExchangeMission::onEventTCPSocketError code[%d]\n", errorCode);
}

bool CGPExchangeMission::onEventTCPSocketRead(int main, int sub, void* data, int size) 
{
	PLAZZ_PRINTF("CGPExchangeMission::onEventTCPSocketRead main:%d sub:%d size:%d\n", main, sub, size);
	if (main != MDM_GP_USER_SERVICE)
	{
		return false;
	}

	return false;
}

// 兑换结果
bool CGPExchangeMission::onSubExchangeSuccess(void* data, int size)
{
	
	return true;
}
