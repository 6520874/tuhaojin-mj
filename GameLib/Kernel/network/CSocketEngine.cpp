#include <sstream>
#include "QPCipher.h"
#include "CSocketEngine.h"
#include <platform/CCCommon.h>
#include "Platform/PlatformHeader.h"
#include "Json/json.hpp"

using json = nlohmann::json;
using namespace std;
//////////////////////////////////////////////////////////////////////////

ISocketEngine* ISocketEngine::create()
{
	return new CSocketEngine();
}

void ISocketEngine::destory(ISocketEngine* pISocketEngine)
{
	if (pISocketEngine)
	{
		delete pISocketEngine;
	}
}

//////////////////////////////////////////////////////////////////////////
CSocketEngine::CSocketEngine()
{
	mISocketEngineSink = 0;
	mIsSendTCPValidate = false;
	mSocket.setSocketSink(this);

	mBufPack = new byte[SIZE_TCP_BUFFER];
	mBufUnPack = new byte[SIZE_TCP_BUFFER];
	// 接收缓冲
	mBufRecieve = new byte[SIZE_TCP_BUFFER];
	mTempBuf = new byte[SIZE_TCP_BUFFER];
	//加密数据
	m_cbSendRound = 0;
	m_cbRecvRound = 0;
	m_dwSendXorKey = 0;
	m_dwRecvXorKey = 0;

	m_dwSendPacketCount = 0;
	m_dwRecvPacketCount = 0;

	// add by lesten
	temp_size_ = 0;
}

CSocketEngine::~CSocketEngine()
{
	delete [] mBufPack;
	delete [] mBufUnPack;
	delete [] mBufRecieve;
	delete [] mTempBuf;

	mISocketEngineSink=0;
	mSocket.setSocketSink(0);
	disconnect();
}

//////////////////////////////////////////////////////////////////////////
// ISocketEngine 接口实现

/** 设置Socket接收器 */
void CSocketEngine::setSocketEngineSink(ISocketEngineSink* pISocketEngineSink)
{
	mISocketEngineSink = pISocketEngineSink;
}

void CSocketEngine::initValue()
{
	m_cbSendRound = 0;
	m_cbRecvRound = 0;
	m_dwSendXorKey = 0;
	m_dwRecvXorKey = 0;
	m_dwSendPacketCount = 0;
	m_dwRecvPacketCount = 0;
	mBufRevLength = 0;
	temp_size_ = 0;
	memset(mTempBuf, SIZE_TCP_BUFFER, 0);
}
/** 链接网络 **/
bool CSocketEngine::connect(const char* url, int port)
{
	initValue();

	cocos2d::log("CSocketEngine Connect %s",url);

	return mSocket.connect(url, port) == 0;
}

/** 关闭网络 **/
bool CSocketEngine::disconnect()
{
	//恢复数据
	initValue();

	mSocket.disconnect();
	return true;
}

struct MessageHead {
    int size;
    word main;
    word sub;
};

/** 发送数据 **/
bool CSocketEngine::send(int main, int sub, void* data, int dataSize)
{
	if (!isAlive())
		return false;
	//构造数据
	MessageHead pHead;
    pHead.size = dataSize + sizeof(MessageHead);
	pHead.main = main;
	pHead.sub = sub;
    
    cocos2d::log("CSocketEngine send size:%d, main:%d, sub:%d, data:%s", pHead.size, pHead.main, pHead.sub, data);
    
    mSocket.send((const char*)&pHead, sizeof(MessageHead));
    if(dataSize > 0)
        mSocket.send((const char*)data, dataSize);
	return true;
}

/** 状态判断 **/
bool CSocketEngine::isAlive()
{
	return mSocket.isAlive();
}

/** 发送校验 **/
void CSocketEngine::setTCPValidate(bool send)
{
	mIsSendTCPValidate = send;
}

//////////////////////////////////////////////////////////////////////////
// 接口ISocketSink
void CSocketEngine::onSocketLink()
{
	sendTCPValidate();
	if (mISocketEngineSink != 0)
		mISocketEngineSink->onEventTCPSocketLink();
}

void CSocketEngine::onSocketShut()
{
	if (mISocketEngineSink != nullptr)
		mISocketEngineSink->onEventTCPSocketShut();
}
void CSocketEngine::onSocketTimeOut()
{
	if (mISocketEngineSink != nullptr)
		mISocketEngineSink->onEventTCPSocketError(0);
}

void CSocketEngine::onSocketError(int errorCode)
{
	if (mISocketEngineSink != 0)
		mISocketEngineSink->onEventTCPSocketError(errorCode);

}

void CSocketEngine::onSocketData(void* data, int dataSize)
{
    
	static byte cbBufRecieve[SIZE_TCP_BUFFER];
	int nRecvSize = 0;

	//  add by lesten
	if (temp_size_!=0){
			
		nRecvSize += temp_size_;
		memcpy( &cbBufRecieve[0], mTempBuf, temp_size_ );

		temp_size_ = 0;
		memset(mTempBuf, SIZE_TCP_BUFFER, 0);
	}
	if (nRecvSize + dataSize >= SIZE_TCP_BUFFER)
	{
		cocos2d::log("nRecvSize + dataSize >= SIZE_TCP_BUFFER");
		disconnect();
		return;
	}
	memcpy(&cbBufRecieve[nRecvSize], data, dataSize);

	nRecvSize += dataSize;

	//变量定义
	unsigned short wPacketSize = 0;
	byte cbDataBuffer[SOCKET_TCP_PACKET + sizeof(TCP_Head)];
	TCP_Head * pHead = (TCP_Head *)cbBufRecieve;

	while (nRecvSize >= sizeof(TCP_Head))
	{
		wPacketSize = pHead->TCPInfo.wPacketSize;
				
		if (wPacketSize > (SOCKET_TCP_PACKET + sizeof(TCP_Head)))
		{
			disconnect();
			return;
		}
				
		if (nRecvSize < wPacketSize){
			
			temp_size_ = nRecvSize;
			memcpy(mTempBuf, cbBufRecieve, temp_size_);
				
			return;
		}

		//拷贝数据
		m_dwRecvPacketCount++;
		memcpy(cbDataBuffer, cbBufRecieve, wPacketSize);
		nRecvSize -= wPacketSize;
		memmove(cbBufRecieve, cbBufRecieve + wPacketSize, nRecvSize);

		//解释数据
		void * pDataBuffer = cbDataBuffer + sizeof(TCP_Head);
		TCP_Command Command = ((TCP_Head *)cbDataBuffer)->CommandInfo;

        CCLOG("onSocketData wPacketSize:%d, wMainCmdID:%d, wSubCmdID:%d, json:%s", wPacketSize,Command.wMainCmdID, Command.wSubCmdID,pDataBuffer);
        
        if(Command.wMainCmdID == 2000){
            send(2001, 0, NULL, 0);
        }else if (mISocketEngineSink != 0)
		{
            try {
                
                std::vector<char> vec1((char*)pDataBuffer, (char*)pDataBuffer + wPacketSize - 8);
                auto pData = json::parse(vec1);
                bool bHandle = mISocketEngineSink->onEventTCPSocketRead(Command.wMainCmdID, Command.wSubCmdID, (void*)&pData, 0);
                
            } catch (std::exception& e) {
                CCLOG("json parse error:%s", e.what());
            }
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 辅助函数

void CSocketEngine::sendTCPValidate()
{
	if (mIsSendTCPValidate == false)
		return;
	// 获取验证信息
	QPCipher::tcpValidate(&mBufPack[0], 0);
	// 发送验证
	ostringstream ss;
	ss.str("");
	for (int i = 0; i < SIZE_VALIDATE; i++)
	{
		ss << mBufPack[i] << ",";
	}
	mSocket.send(ss.str().c_str(), (int)ss.str().size());
}

bool CSocketEngine::unpack(byte* data, int start, int length)
{
	// 解密
	if ((data[start] & QPCipher::getCipherMode()) > 0)
	{
		QPCipher::decryptBuffer(data, start, length);
	}
	// 主命令码
	int main = QPCipher::getMainCommand(data, start);
	// 次命令码
	int sub = QPCipher::getSubConmmand(data, start);
	
	// 附加数据
	if (length > 8) 
	{
		memcpy(mBufUnPack, &data[start+8], length - 8);
	}
	
	length -= 8;

	

	//if (SOCKET_CHECK)
	//	PLAZZ_PRINTF("Main:%d Sub:%d Size:%d\n", main, sub, length);

	
	
	if (main == 0 && sub == 1)
	{
		//PLAZZ_PRINTF("REV-HEART\n");
	}
	else
	{
		if (mISocketEngineSink != 0)
		{
			bool bHandle = mISocketEngineSink->onEventTCPSocketRead(main, sub, mBufUnPack, length);

			//if (!bHandle)
			//	PLAZZ_PRINTF("no match: main:%d sub:%d size:%d\n", main, sub, length);
			return bHandle;
		}
	}
	
	return true;
}

//加密数据
unsigned short CSocketEngine::EncryptBuffer(unsigned char * pcbDataBuffer, unsigned short wDataSize, unsigned short wBufferSize)
{
	//调整长度
	unsigned short wEncryptSize = wDataSize - sizeof(TCP_Command), wSnapCount = 0;
	if ((wEncryptSize%sizeof(unsigned int)) != 0)
	{
		wSnapCount = sizeof(unsigned int)-wEncryptSize%sizeof(unsigned int);
		memset(pcbDataBuffer + sizeof(TCP_Info)+wEncryptSize, 0, wSnapCount);
	}

	//效验码与字节映射
	unsigned char cbCheckCode = 0;
	unsigned short i = 0;
	for (i = sizeof(TCP_Info); i < wDataSize; i++)
	{
		cbCheckCode += pcbDataBuffer[i];
		//cocos2d::log("CSocketEngine::EncryptBuffer MapSendByte -- 1 -- %d  ", pcbDataBuffer[i]);
		
		pcbDataBuffer[i] = MapSendByte(pcbDataBuffer[i]);

		//cocos2d::log("CSocketEngine::EncryptBuffer MapSendByte -- 2 -- %d  ", pcbDataBuffer[i]);
	}

	//填写信息头
	TCP_Head * pHead = (TCP_Head *)pcbDataBuffer;
	pHead->TCPInfo.cbCheckCode = ~cbCheckCode + 1;
	pHead->TCPInfo.wPacketSize = wDataSize;
	pHead->TCPInfo.cbDataKind = DK_ENCRYPT;

	//创建密钥
	unsigned int dwXorKey = m_dwSendXorKey;
	if (m_dwSendPacketCount == 0)
	{
		//随机映射种子
		dwXorKey = ::time(NULL);
		dwXorKey = SeedRandMap((unsigned short)dwXorKey);
		dwXorKey |= ((unsigned int)SeedRandMap((unsigned int)(dwXorKey >> 16))) << 16;
		dwXorKey ^= g_dwPacketKey;
		m_dwSendXorKey = dwXorKey;
		m_dwRecvXorKey = dwXorKey;
	}

	//加密数据
	unsigned short * pwSeed = (unsigned short *)(pcbDataBuffer + sizeof(TCP_Info));
	unsigned int * pdwXor = (unsigned int *)(pcbDataBuffer + sizeof(TCP_Info));
	unsigned short wEncrypCount = (wEncryptSize + wSnapCount) / sizeof(unsigned int);
	for (unsigned short i = 0; i < wEncrypCount; i++)
	{
		*pdwXor++ ^= dwXorKey;
		dwXorKey = SeedRandMap(*pwSeed++);
		dwXorKey |= ((unsigned int)SeedRandMap(*pwSeed++)) << 16;
		dwXorKey ^= g_dwPacketKey;
	}

	//插入密钥
	if (m_dwSendPacketCount == 0)
	{
		memmove(pcbDataBuffer + sizeof(TCP_Head)+sizeof(unsigned int), pcbDataBuffer + sizeof(TCP_Head), wDataSize);
		*((unsigned int *)(pcbDataBuffer + sizeof(TCP_Head))) = m_dwSendXorKey;
		pHead->TCPInfo.wPacketSize += sizeof(unsigned int);
		wDataSize += sizeof(unsigned int);
	}

	//cocos2d::log("CSocketEngine::EncryptBuffer cbCheckCode -- %d   m_dwSendXorKey --- %x ----   wDataSize ---- %d", pHead->TCPInfo.cbCheckCode, m_dwSendXorKey, wDataSize);

	//设置变量
	m_dwSendPacketCount++;
	m_dwSendXorKey = dwXorKey;

	return wDataSize;
}
//解密数据
unsigned short CSocketEngine::CrevasseBuffer(unsigned char pcbDataBuffer[], unsigned short wDataSize)
{
	//ASSERT(m_dwSendPacketCount > 0);
	ASSERT(wDataSize >= sizeof(TCP_Head));
	ASSERT(((TCP_Head *)pcbDataBuffer)->TCPInfo.wPacketSize == wDataSize);

	//调整长度
	unsigned short wSnapCount = 0;
	if ((wDataSize%sizeof(unsigned int)) != 0)
	{
		wSnapCount = sizeof(unsigned int)-wDataSize%sizeof(unsigned int);
		memset(pcbDataBuffer + wDataSize, 0, wSnapCount);
	}

	if (m_dwRecvPacketCount == 0)
	{
		//数据包长度错误
		if (wDataSize < (sizeof(TCP_Head)+sizeof(unsigned int)))
			return 0;

		m_dwRecvXorKey = *(unsigned int *)(pcbDataBuffer + sizeof(TCP_Head));
		m_dwSendXorKey = m_dwRecvXorKey;
		memmove(pcbDataBuffer + sizeof(TCP_Head), pcbDataBuffer + sizeof(TCP_Head)+sizeof(unsigned int),
			wDataSize - sizeof(TCP_Head)-sizeof(unsigned int));
		wDataSize -= sizeof(unsigned int);
		((TCP_Head *)pcbDataBuffer)->TCPInfo.wPacketSize -= sizeof(unsigned int);
	}

	//解密数据
	unsigned int dwXorKey = m_dwRecvXorKey;
	unsigned int * pdwXor = (unsigned int *)(pcbDataBuffer + sizeof(TCP_Info));
	unsigned short  * pwSeed = (unsigned short *)(pcbDataBuffer + sizeof(TCP_Info));
	unsigned short wEncrypCount = (wDataSize + wSnapCount - sizeof(TCP_Info)) / 4;
	for (unsigned short i = 0; i<wEncrypCount; i++)
	{
		if ((i == (wEncrypCount - 1)) && (wSnapCount>0))
		{
			unsigned char * pcbKey = ((unsigned char *)&m_dwRecvXorKey) + sizeof(unsigned int)-wSnapCount;
			memcpy(pcbDataBuffer + wDataSize, pcbKey, wSnapCount);
		}
		dwXorKey = SeedRandMap(*pwSeed++);
		dwXorKey |= ((unsigned int)SeedRandMap(*pwSeed++)) << 16;
		dwXorKey ^= g_dwPacketKey;
		*pdwXor++ ^= m_dwRecvXorKey;
		m_dwRecvXorKey = dwXorKey;
	}

	//效验码与字节映射
	TCP_Head * pHead = (TCP_Head *)pcbDataBuffer;
	unsigned char cbCheckCode = pHead->TCPInfo.cbCheckCode;
	for (int i = sizeof(TCP_Info); i < wDataSize; i++)
	{
		pcbDataBuffer[i] = MapRecvByte(pcbDataBuffer[i]);
		cbCheckCode += pcbDataBuffer[i];
	}
	if (cbCheckCode != 0)
	{
		return 0;
	}

	return wDataSize;
}

//随机映射
unsigned short CSocketEngine::SeedRandMap(unsigned short wSeed)
{
	unsigned int dwHold = wSeed;
	return (unsigned short)((dwHold = dwHold * 241103L + 2533101L) >> 16);
}

//映射发送数据
unsigned char CSocketEngine::MapSendByte(unsigned char const cbData)
{
	unsigned char cbMap = g_SendByteMap[(unsigned char)(cbData + m_cbSendRound)];
	
	// cocos2d::log("CSocketEngine::EncryptBuffer MapSendByte -- g_SendByteMap->cbMap -- %d-----%d---%d  ", cbMap, m_cbSendRound, cbData);
	
	m_cbSendRound += 3;
	return cbMap;
}

//映射接收数据
unsigned char CSocketEngine::MapRecvByte(unsigned char const cbData)
{
	unsigned char cbMap = g_RecvByteMap[cbData] - m_cbRecvRound;
	m_cbRecvRound += 3;
	return cbMap;
}
