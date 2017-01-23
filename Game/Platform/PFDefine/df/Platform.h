#ifndef PLATFORM_HEAD_FILE
#define PLATFORM_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//包含文件

//定义文件
#include "Macro.h"
#include "Platform/PFDefine/df/Define.h"

//结构文件
#include "Platform/PFDefine/df/Struct.h"
#include "Platform/PFDefine/df/Packet.h"
#include "Platform/PFDefine/df/Property.h"

//模板文件
//#include "Array.h"
#include "Platform/PFDefine/df/Module.h"
#include "Platform/PFDefine/df/ServerRule.h"
#include "Platform/PFDefine/df/RightDefine.h"

//程序版本
#define VERSION_FRAME				PROCESS_VERSION(6,0,3)				//框架版本
#define VERSION_PLAZA				PROCESS_VERSION(9,0,3)				//大厅版本
#define VERSION_MOBILE_ANDROID		PROCESS_VERSION(6,0,3)				//手机版本
#define VERSION_MOBILE_IOS			PROCESS_VERSION(6,0,3)				//手机版本

//版本定义
#define VERSION_EFFICACY			0									//效验版本
#define VERSION_FRAME_SDK			INTERFACE_VERSION(6,3)				//框架版本

#endif