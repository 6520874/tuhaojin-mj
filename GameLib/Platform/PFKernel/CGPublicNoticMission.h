#pragma once
#include "cocos2d.h"
#include "Platform/PlatformHeader.h"
#include "Kernel/network/CSocketMission.h"

//////////////////////////////////////////////////////////////////////////
// 机器绑定修改提示
//////////////////////////////////////////////////////////////////////////
class CGPublicNoticMissionSink
{
public:
	virtual ~CGPublicNoticMissionSink(){}
	virtual void onGPPublicNoticeResult(std::string kKey,int iError,const char* szDescription){}
};

//////////////////////////////////////////////////////////////////////////
// 兑换道具
//////////////////////////////////////////////////////////////////////////
class CGPublicNoticMission
	: public CSocketMission
{
public:
	CGPublicNoticMission(const char* url, int port);
	// 设置回调接口
	void setMissionSink(CGPublicNoticMissionSink* pIGPKefuMissionSink);

	void PublicNotice(std::string kKey);
public:
	virtual void onEventTCPSocketLink();
	virtual void onEventTCPSocketShut();
	virtual void onEventTCPSocketError(int errorCode);
	virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);
private:
	// 回调
	std::string m_kKeyName;
	CGPublicNoticMissionSink* mIGPKefuMissionSink;
}; 
