#include "CGPIndividualMission.h"
#include "Kernel/kernel/server/IServerItem.h"



//////////////////////////////////////////////////////////////////////////

CGPIndividualMission::CGPIndividualMission(const char* url, int port)
: CSocketMission(url, port)
, m_bRevStop(true)
{
	mMissionType = MISSION_INDIVIDUAL_NULL;
	mIGPIndividualMissionSink = 0;
}

// 设置回调接口
void CGPIndividualMission::setMissionSink(IGPIndividualMissionSink* pIGPIndividualMissionSink)
{
	mIGPIndividualMissionSink = pIGPIndividualMissionSink;
}

// 查询个人资料
void CGPIndividualMission::query(int iAccountID,bool bRecStop)
{
	m_bRevStop = bRecStop;

	mAccountInfoID = iAccountID;

	mMissionType = MISSION_INDIVIDUAL_QUERY;

	start();
}
void CGPIndividualMission::queryAccountInfo(int iAccountID)
{
	if (iAccountID >= 0)
	{
		mAccountInfoID = iAccountID;
	}
	else
	{
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		mAccountInfoID =  pGlobalUserData->dwUserID;
	}

	mMissionType = MISSION_INDIVIDUAL_Account;
	start();
}

void CGPIndividualMission::modifyName(std::string kName)
{
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	memset(&mModifyIndividual, 0, sizeof(mModifyIndividual));
	mModifyIndividual.cbGender = pGlobalUserData->cbGender;
	strncpy(mModifyIndividual.szNickName,kName.c_str(),kName.size());
	mMissionType = MISSION_INDIVIDUAL_MODIFY;
	start();
}

void CGPIndividualMission::modifyGender(byte gender)
{
	memset(&mModifyIndividual, 0, sizeof(mModifyIndividual));
	mModifyIndividual.cbGender = gender;
	mMissionType = MISSION_INDIVIDUAL_MODIFY;
	start();
}
void CGPIndividualMission::modify(std::string kNickName,byte gender)
{
	memset(&mModifyIndividual, 0, sizeof(mModifyIndividual));
	mModifyIndividual.cbGender = gender;
	strncpy(mModifyIndividual.szNickName,kNickName.c_str(),kNickName.size());
	mMissionType = MISSION_INDIVIDUAL_MODIFY;
	start();
}
// 修改个人资料
void CGPIndividualMission::modify(const tagModifyIndividual& individual)
{
	memcpy(&mModifyIndividual, &individual, sizeof(mModifyIndividual));
	mMissionType = MISSION_INDIVIDUAL_MODIFY;
	start();
}
void CGPIndividualMission::modifySpreader(std::string kSpreaderID)
{
	m_kSpreaderID = kSpreaderID;
	mMissionType = MISSION_INDIVIDUAL_SPREADER;
	start();
}
void CGPIndividualMission::modifyHeadHttp(std::string kHttp)
{
	memset(&mModifyIndividual, 0, sizeof(mModifyIndividual));
	strncpy(mModifyIndividual.szHeadHttp,kHttp.c_str(),kHttp.size());
	mMissionType = MISSION_INDIVIDUAL_MODIFY;
	start();
}
void CGPIndividualMission::modifyUserChannel(std::string szUserChannel)
{
	memset(&mModifyIndividual, 0, sizeof(mModifyIndividual));
	strncpy(mModifyIndividual.szUserChannel,szUserChannel.c_str(),szUserChannel.size());
	mMissionType = MISSION_INDIVIDUAL_MODIFY;
	start();
}

void CGPIndividualMission::onEventTCPSocketLink()
{
	PLAZZ_PRINTF("CGPIndividualMission::onEventTCPSocketLink\n");
	
	switch (mMissionType)
	{
		// 查询个人资料
	case MISSION_INDIVIDUAL_Account:
		{
			//变量定义
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//变量定义
			CMD_GP_QueryAccountInfo QueryIndividual;
			zeromemory(&QueryIndividual,sizeof(QueryIndividual));

			QueryIndividual.dwUserID=mAccountInfoID;
			send(MDM_GP_USER_SERVICE,SUB_GP_QUERY_ACCOUNTINFO,&QueryIndividual,sizeof(QueryIndividual));
			break;
		}
		// 查询个人资料
	case MISSION_INDIVIDUAL_QUERY:
		{
			//变量定义
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//变量定义
			CMD_GP_QueryIndividual QueryIndividual;
			zeromemory(&QueryIndividual,sizeof(QueryIndividual));

			send(MDM_GP_USER_SERVICE,SUB_GP_QUERY_INDIVIDUAL,&QueryIndividual,sizeof(QueryIndividual));
			break;
		}
	case MISSION_INDIVIDUAL_SPREADER:
		{
			//变量定义
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//变量定义
			CMD_GP_ModifySpreader kNetInfo;
			zeromemory(&kNetInfo,sizeof(kNetInfo));

			//设置变量
			kNetInfo.dwUserID=pGlobalUserData->dwUserID;
			strncpy(kNetInfo.szPassword,pGlobalUserData->szPassword,countarray(kNetInfo.szPassword));
			strncpy(kNetInfo.szSpreader,m_kSpreaderID.c_str(),countarray(kNetInfo.szSpreader));

			//发送数据
			send(MDM_GP_USER_SERVICE,SUB_GP_MODIFY_SPREADER,&kNetInfo,sizeof(kNetInfo));
			break;
		}
		// 修改个人资料
	case MISSION_INDIVIDUAL_MODIFY:
		{
			//变量定义
			byte cbBuffer[SIZE_PACK_DATA];
			zeromemory(cbBuffer,sizeof(cbBuffer));

			//变量定义
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//变量定义
			CMD_GP_ModifyIndividual * pModifyIndividual=(CMD_GP_ModifyIndividual *)cbBuffer;
			CSendPacketHelper SendPacket(cbBuffer+sizeof(CMD_GP_ModifyIndividual),sizeof(cbBuffer)-sizeof(CMD_GP_ModifyIndividual));

			//设置变量
			pModifyIndividual->cbGender=mModifyIndividual.cbGender;
			pModifyIndividual->dwUserID=pGlobalUserData->dwUserID;
			strncpy(pModifyIndividual->szPassword,pGlobalUserData->szPassword,countarray(pModifyIndividual->szPassword));

			//用户昵称
			if (mModifyIndividual.szNickName[0]!=0 && countarray(mModifyIndividual.szNickName) >= 6)
			{
				PLAZZ_PRINTF(("szNickName:%s \n"), mModifyIndividual.szNickName);
				SendPacket.AddPacket(mModifyIndividual.szNickName,DTP_GP_UI_NICKNAME);
			}

			//个性签名
			if (mModifyIndividual.szUnderWrite[0]!=0)
			{
				PLAZZ_PRINTF(("szUnderWrite:%s \n"), mModifyIndividual.szUnderWrite);
				SendPacket.AddPacket(mModifyIndividual.szUnderWrite,DTP_GP_UI_UNDER_WRITE);
			}

			//用户备注
			if (mModifyIndividual.szUserNote[0]!=0)
			{
				SendPacket.AddPacket(mModifyIndividual.szUserNote,DTP_GP_UI_USER_NOTE);
			}

			//真实名字
			if (mModifyIndividual.szCompellation[0]!=0) 
			{
				SendPacket.AddPacket(mModifyIndividual.szCompellation,DTP_GP_UI_COMPELLATION);
			}

			//固定号码
			if (mModifyIndividual.szSeatPhone[0]!=0) 
			{
				SendPacket.AddPacket(mModifyIndividual.szSeatPhone,DTP_GP_UI_SEAT_PHONE);
			}

			//手机号码
			if (mModifyIndividual.szMobilePhone[0]!=0)
			{
				SendPacket.AddPacket(mModifyIndividual.szMobilePhone,DTP_GP_UI_MOBILE_PHONE);
			}

			//Q Q 号码
			if (mModifyIndividual.szQQ[0]!=0) 
			{
				SendPacket.AddPacket(mModifyIndividual.szQQ,DTP_GP_UI_QQ);
			}

			//电子邮件
			if (mModifyIndividual.szEMail[0]!=0) 
			{
				SendPacket.AddPacket(mModifyIndividual.szEMail,DTP_GP_UI_EMAIL);
			}

			//详细地址
			if (mModifyIndividual.szDwellingPlace[0]!=0) 
			{
				SendPacket.AddPacket(mModifyIndividual.szDwellingPlace,DTP_GP_UI_DWELLING_PLACE);
			}

			//详细地址
			if (mModifyIndividual.szHeadHttp[0]!=0) 
			{
				SendPacket.AddPacket(mModifyIndividual.szHeadHttp,DTP_GP_UI_HEAD_HTTP);
			}

			//详细地址
			if (mModifyIndividual.szUserChannel[0]!=0) 
			{
				SendPacket.AddPacket(mModifyIndividual.szUserChannel,DTP_GP_UI_CHANNEL);
			}

			//发送数据
			word wSendSize=sizeof(CMD_GP_ModifyIndividual)+SendPacket.GetDataSize();
			send(MDM_GP_USER_SERVICE,SUB_GP_MODIFY_INDIVIDUAL,cbBuffer,wSendSize);
			break;
		}
	}

}

void CGPIndividualMission::onEventTCPSocketShut()
{
	PLAZZ_PRINTF("CGPIndividualMission::onEventTCPSocketShut\n");
}

void CGPIndividualMission::onEventTCPSocketError(int errorCode)
{
	PLAZZ_PRINTF("CGPIndividualMission::onEventTCPSocketError code[%d]\n", errorCode);
}

bool CGPIndividualMission::onEventTCPSocketRead(int main, int sub, void* data, int size) 
{
	PLAZZ_PRINTF("CGPIndividualMission::onEventTCPSocketRead main:%d sub:%d size:%d\n", main, sub, size);
	if (main != MDM_GP_USER_SERVICE)
	{
		return false;
	}

	switch (sub)
	{
		//个人信息
	case SUB_GP_QUERY_ACCOUNTINFO:	return onSubUserAccountInfo(data, size);
		//个人信息
	case SUB_GP_USER_INDIVIDUAL:	return onSubUserIndividual(data, size);
		//设置推荐人结果
	case SUB_GP_SPREADER_RESOULT:	return onSubSpreaderResoult(data, size);
		//操作成功
	case SUB_GP_OPERATE_SUCCESS:	return onSubOperateSuccess(data, size);
		//操作失败
	case SUB_GP_OPERATE_FAILURE:	return onSubOperateFailure(data, size);
	}

	return false;
}

// 个人信息
bool CGPIndividualMission::onSubUserAccountInfo(void* data, int size)
{
	PLAZZ_PRINTF("CGPIndividualMission::onSubUserAccountInfo\n");


	//变量定义
	CMD_GP_UserAccountInfo * pAccountInfo=(CMD_GP_UserAccountInfo *)data;
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	if (pAccountInfo->dwUserID == pGlobalUserInfo->getUserID())
	{
		pGlobalUserData->lUserScore = pAccountInfo->lUserScore;
		pGlobalUserData->lUserInsure = pAccountInfo->lUserInsure;
		//保存信息
		pGlobalUserData->wFaceID = pAccountInfo->wFaceID;
		pGlobalUserData->cbGender = pAccountInfo->cbGender;
		pGlobalUserData->dwUserID =pAccountInfo->dwUserID;
		pGlobalUserData->dwGameID = pAccountInfo->dwGameID;
		pGlobalUserData->dwSpreaderID = pAccountInfo->dwSpreaderID;
		pGlobalUserData->dwExperience =pAccountInfo->dwExperience;
		strcpy(pGlobalUserData->szNickName, utility::a_u8((char*)pAccountInfo->szNickName).c_str());
		strncpy(pGlobalUserData->szAccounts, ((char*)pAccountInfo->szAccounts), countarray(pGlobalUserData->szAccounts));
		strncpy(pGlobalUserData->szLogonIP, ((char*)pAccountInfo->szLogonIp), countarray(pGlobalUserData->szLogonIP));
		//金币信息
		pGlobalUserInfo->upPlayerInfo();
	}
	if (mIGPIndividualMissionSink)
	{
		mIGPIndividualMissionSink->onGPAccountInfo(pAccountInfo);
	}

	if (m_bRevStop)
	{
		stop();
	}

	return true;
}
// 个人信息
bool CGPIndividualMission::onSubUserIndividual(void* data, int size)
{
	PLAZZ_PRINTF("CGPIndividualMission::onSubUserIndividual\n");

	CMD_GP_UserIndividual* pModifyIndividual = (CMD_GP_UserIndividual*)data;


	void * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pModifyIndividual+1,size-sizeof(CMD_GP_UserIndividual));

	bool bUpdate = false;
	std::string kIP,kHttp,kChannel;
	//扩展信息
	while (true)
	{
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) break;
		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GP_UI_HEAD_HTTP:	//联系地址
			{
				if (DataDescribe.wDataSize<=LEN_USER_NOTE)
				{
					bUpdate = true;
					kHttp.assign((char*)pDataBuffer,DataDescribe.wDataSize);
					
				}
				break;
			}
		case DTP_GP_UI_IP:	//联系地址
			{
				if (DataDescribe.wDataSize<=LEN_NICKNAME)
				{
					bUpdate = true;
					kIP.assign((char*)pDataBuffer,DataDescribe.wDataSize);

				}
				break;
			}
		case DTP_GP_UI_CHANNEL:	//联系地址
			{
				if (DataDescribe.wDataSize<=LEN_NICKNAME)
				{
					bUpdate = true;
					kChannel.assign((char*)pDataBuffer,DataDescribe.wDataSize);

				}
				break;
			}
		}
	}
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	if (pModifyIndividual->dwUserID == pGlobalUserInfo->getUserID())
	{
		if (kIP != "")
		{
			strncpy(pGlobalUserData->szLogonIP, (kIP.c_str()), kIP.size());
		}
		if (kHttp != "")
		{
			strncpy(pGlobalUserData->szHeadHttp, (kHttp.c_str()), kHttp.size());
		}
		if (kChannel != "")
		{
			strncpy(pGlobalUserData->szUserChannel, (kChannel.c_str()), kChannel.size());
		}
		//金币信息
		pGlobalUserInfo->upPlayerInfo();
	}

	if (bUpdate && mIGPIndividualMissionSink)
		mIGPIndividualMissionSink->onGPAccountInfoHttpIP(pModifyIndividual->dwUserID,kIP,kHttp);

	if (m_bRevStop)
	{
		stop();
	}

	//通知
	if (mIGPIndividualMissionSink)
		mIGPIndividualMissionSink->onGPIndividualInfo(mMissionType);
	return true;

}
bool CGPIndividualMission::onSubSpreaderResoult(void* data, int size)
{
	PLAZZ_PRINTF("CGPIndividualMission::onSubSpreaderResoult\n");
	//变量定义
	CMD_GP_SpreaderResoult * pOperateSuccess=(CMD_GP_SpreaderResoult *)data;

	//效验数据
	ASSERT(size>=(sizeof(CMD_GP_SpreaderResoult)-sizeof(pOperateSuccess->szDescribeString)));
	if (size<(sizeof(CMD_GP_SpreaderResoult)-sizeof(pOperateSuccess->szDescribeString))) return false;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	if (pOperateSuccess->lResultCode == 0)
	{
		pGlobalUserData->lUserScore = pOperateSuccess->lScore;

		pGlobalUserInfo->upPlayerInfo();
	}
	//关闭连接
	if (m_bRevStop)
	{
		stop();
	}

	//显示消息
	if (mIGPIndividualMissionSink)
		mIGPIndividualMissionSink->onGPIndividualSuccess(mMissionType, pOperateSuccess->szDescribeString);
	return true;
}

// 操作成功
bool CGPIndividualMission::onSubOperateSuccess(void* data, int size)
{
	PLAZZ_PRINTF("CGPIndividualMission::onSubOperateSuccess\n");
	//变量定义
	CMD_GP_OperateSuccess * pOperateSuccess=(CMD_GP_OperateSuccess *)data;

	//效验数据
	ASSERT(size>=(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString)));
	if (size<(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString))) return false;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	switch (mMissionType)
	{
		// 查询个人资料
	case MISSION_INDIVIDUAL_SPREADER:
		{
			pGlobalUserData->dwSpreaderID = 1;
			pGlobalUserInfo->upPlayerInfo();
			break;
		}
		// 查询个人资料
	case MISSION_INDIVIDUAL_QUERY:
		{
			break;
		}
		// 修改个人资料
	case MISSION_INDIVIDUAL_MODIFY:
		{
			tagIndividualUserData * pIndividualUserData=pGlobalUserInfo->GetIndividualUserData();

			//帐号资料
			pGlobalUserData->cbGender=mModifyIndividual.cbGender;

			//用户昵称
			if (mModifyIndividual.szNickName[0]!=0)
				strcpy(pGlobalUserData->szNickName,utility::a_u8(mModifyIndividual.szNickName).c_str());

			//个性签名
			if (mModifyIndividual.szUnderWrite[0]!=0)
				strncpy(pGlobalUserData->szUnderWrite,mModifyIndividual.szUnderWrite,countarray(pGlobalUserData->szUnderWrite));

			//详细资料

			//用户备注
			if (mModifyIndividual.szUserNote[0]!=0)
				strncpy(pIndividualUserData->szUserNote,mModifyIndividual.szUserNote,countarray(pIndividualUserData->szUserNote));

			//真实名字
			if (mModifyIndividual.szCompellation[0]!=0) 
				strncpy(pIndividualUserData->szCompellation,mModifyIndividual.szCompellation,countarray(pIndividualUserData->szCompellation));

			//固定号码
			if (mModifyIndividual.szSeatPhone[0]!=0) 
				strncpy(pIndividualUserData->szSeatPhone,mModifyIndividual.szSeatPhone,countarray(pIndividualUserData->szSeatPhone));

			//手机号码
			if (mModifyIndividual.szMobilePhone[0]!=0)
				strncpy(pIndividualUserData->szMobilePhone,mModifyIndividual.szMobilePhone,countarray(pIndividualUserData->szMobilePhone));

			//Q Q 号码
			if (mModifyIndividual.szQQ[0]!=0) 
				strncpy(pIndividualUserData->szQQ,mModifyIndividual.szQQ,countarray(pIndividualUserData->szQQ));

			//电子邮件
			if (mModifyIndividual.szEMail[0]!=0) 
				strncpy(pIndividualUserData->szEMail,mModifyIndividual.szEMail,countarray(pIndividualUserData->szEMail));

			//详细地址
			if (mModifyIndividual.szHeadHttp[0]!=0) 
				strncpy(pGlobalUserData->szHeadHttp,mModifyIndividual.szHeadHttp,countarray(pGlobalUserData->szHeadHttp));

			//详细地址
			if (mModifyIndividual.szUserChannel[0]!=0) 
				strncpy(pGlobalUserData->szUserChannel,mModifyIndividual.szUserChannel,countarray(pGlobalUserData->szUserChannel));

			pGlobalUserInfo->upPlayerInfo();
			break;
		}
	}

	//关闭连接
	if (m_bRevStop)
	{
		stop();
	}

	//显示消息
	if (mIGPIndividualMissionSink)
		mIGPIndividualMissionSink->onGPIndividualSuccess(mMissionType, pOperateSuccess->szDescribeString);
	return true;
}

// 操作失败
bool CGPIndividualMission::onSubOperateFailure(void* data, int size)
{
	PLAZZ_PRINTF("CGPIndividualMission::onSubOperateFailure\n");
	//效验参数
	CMD_GP_OperateFailure * pOperateFailure=(CMD_GP_OperateFailure *)data;
	ASSERT(size>=(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString)));
	if (size<(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString))) return false;

	//关闭连接
	if (m_bRevStop)
	{
		stop();
	}

	//显示消息
	if (mIGPIndividualMissionSink)
		mIGPIndividualMissionSink->onGPIndividualFailure(mMissionType, pOperateFailure->szDescribeString);

	return true;
}
void CGPIndividualMission::modifyPhoneNumber( std::string kPhoneNumber )
{
	memset(&mModifyIndividual, 0, sizeof(mModifyIndividual));
	strncpy(mModifyIndividual.szMobilePhone,kPhoneNumber.c_str(),kPhoneNumber.size());
	mMissionType = MISSION_INDIVIDUAL_MODIFY;
	start();
}