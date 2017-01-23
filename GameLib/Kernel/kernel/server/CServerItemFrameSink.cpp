#include "Kernel/kernel/server/CServerItem.h"
IRoomFrameSink* IRoomFrameSink::getInterface()
{
	return (IRoomFrameSink*)IServerItem::get();
}

//////////////////////////////////////////////////////////////////////////
// 框架消息

//游戏已准备好
void CServerItem::OnGFGameReady()
{
	PLAZZ_PRINTF("CServerItem::OnGFGameReady\n");
	//变量定义
	word wTableID=m_pMeUserItem->GetTableID();
	word wChairID=m_pMeUserItem->GetChairID();

	mUserAttribute.wChairID = wChairID;
	mUserAttribute.wTableID = wTableID;

	//配置完成
	if (mIClientKernelSink )
	{
		mIClientKernelSink->SetupGameClient();
	}
	CMD_GF_GameOption GameOption;
	zeromemory(&GameOption,sizeof(GameOption));

	//构造数据
	GameOption.dwFrameVersion=VERSION_FRAME;
	GameOption.cbAllowLookon=gGlobalUnits.m_bAllowLookon;
	GameOption.dwClientVersion=DF::shared()->GetGameVersion();

	//发送数据
	SendSocketData(MDM_GF_FRAME,SUB_GF_GAME_OPTION,&GameOption,sizeof(GameOption));
	
	mIsGameReady = true;
}

//游戏关闭
void CServerItem::OnGFGameClose(int iExitCode)
{
	PLAZZ_PRINTF("flow->OnGFGameClose %d\n", iExitCode);
	if(!m_pMeUserItem)
	{
		return;
	}
	//效验状态
	ASSERT(m_pMeUserItem!=0);
	if (m_pMeUserItem==0) return ;

	mIsGameReady = false;

	//变量定义
	word wTableID=m_pMeUserItem->GetTableID();
	word wChairID=m_pMeUserItem->GetChairID();
	byte cbUserStatus=m_pMeUserItem->GetUserStatus();
	mUserAttribute.wChairID = INVALID_CHAIR;
	mUserAttribute.wTableID = INVALID_TABLE;


	if (iExitCode == ServiceStatus_NetworkDown)
	{
		mServiceStatus = ServiceStatus_NetworkDown;
	}

	m_TableFrame.SetTableStatus(false);

	if (mIClientKernelSink)
	{
		mIClientKernelSink->CloseGameClient();
		mIClientKernelSink = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// Socket消息

// 发送数据
bool CServerItem::GFSendData(int main, int sub, void* data, int size)
{
	return SendSocketData(main, sub, data, size);
}