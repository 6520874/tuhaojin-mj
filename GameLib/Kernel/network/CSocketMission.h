#ifndef _CSocketMission_H_
#define _CSocketMission_H_
#include "Kernel/network/ISocketEngine.h"

class CSocketMission
	: public ISocketEngineSink
{
public:
	CSocketMission(const char* url=0, int port=0);
	virtual ~CSocketMission();

	void setUrl(const char* url, int port);
	bool start();
	void stop();
	bool isAlive() const;
	bool send(int main, int sub);
	bool send(int main, int sub, void* data, int size);
private:
	ISocketEngine* mSocketEngine;
	char mUrl[260];
	int  mPort;
}; // CSocketMission

#endif // _CSocketMission_H_
