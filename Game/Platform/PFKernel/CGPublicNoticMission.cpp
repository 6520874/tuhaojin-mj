#include "CGPublicNoticMission.h"


CGPublicNoticMission::CGPublicNoticMission(const char* url, int port)
: CSocketMission(url, port)
{
	mIGPKefuMissionSink = 0;
}

// 设置回调接口
void CGPublicNoticMission::setMissionSink(CGPublicNoticMissionSink* pIGPKefuMissionSink)
{
	mIGPKefuMissionSink = pIGPKefuMissionSink;
}

void CGPublicNoticMission::PublicNotice(std::string kKey)
{
	if (isAlive())
	{
		PLAZZ_PRINTF("CGPublicNoticMission::PublicNotice isAlive");
		
		return;
	}
	m_kKeyName = kKey;
	start();
}

void CGPublicNoticMission::onEventTCPSocketLink()
{
	CMD_GP_QueryNotice kNetInfo;
	strncpy(kNetInfo.szKeyName,m_kKeyName.c_str(),countarray(kNetInfo.szKeyName));
	//发送数据
	send(MDM_GP_USER_SERVICE,SUB_GP_QUERY_PUBLIC_NOTICE, &kNetInfo,sizeof(kNetInfo));
}

void CGPublicNoticMission::onEventTCPSocketShut()
{
	PLAZZ_PRINTF("CGPKefuMission::onEventTCPSocketShut\n");
}

void CGPublicNoticMission::onEventTCPSocketError(int errorCode)
{
	PLAZZ_PRINTF("CGPKefuMission::onEventTCPSocketError code[%d]\n", errorCode);
}

bool CGPublicNoticMission::onEventTCPSocketRead(int main, int sub, void* data, int size) 
{
	PLAZZ_PRINTF("CGPKefuMission::onEventTCPSocketRead main:%d sub:%d size:%d\n", main, sub, size);
	stop();
	if (SUB_GP_PUBLIC_NOTICE == sub)
	{
		CMD_GP_PublicNotice* pNetInfo = (CMD_GP_PublicNotice*)data;
		if (mIGPKefuMissionSink)
		{
			mIGPKefuMissionSink->onGPPublicNoticeResult(m_kKeyName,pNetInfo->lResultCode,pNetInfo->szDescribeString);
		}
		return true;
	}
	if (SUB_GP_OPERATE_FAILURE == sub)
	{
		CMD_GP_OperateFailure* pNetInfo = (CMD_GP_OperateFailure*)data;
		if (mIGPKefuMissionSink)
		{
			mIGPKefuMissionSink->onGPPublicNoticeResult(m_kKeyName,pNetInfo->lResultCode,pNetInfo->szDescribeString);
		}
		return true;
	}

	return true;
}

