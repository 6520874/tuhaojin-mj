#include "HNMJSoundFun.h"
#include "SimpleAudioEngine.h"
#include "Game/Script/utility.h"
#include "Game/Script/SoundFun.h"
#include "HNMJGameLogic.h"
namespace HNMJSoundFun
{

	static const std::string LG_FLAG_HN = "putong/";

	static bool s_bMute = false;
	static float s_fSound1 = 0;
	static float s_fSound2 = 0;

	
	void playBackMusic(std::string kName)
	{
		SoundFun::playBackMusic(utility::a_u8(kName));
		cocos2d::log("HNMJSoundFun playBackMusic :%s",utility::a_u8(kName).c_str());
	}

	void stopBackMusic()
	{
		SoundFun::stopBackMusic();
	}

	void playEffect(std::string kName,bool bPreload /*= false*/)
	{
		kName = utility::a_u8(LG_FLAG_HN + kName + RAW_KIND);

		SoundFun::playEffect(kName,bPreload);
		cocos2d::log("HNMJSoundFun:%s",kName.c_str());
	}

	void playCardByGender(int iGender,int nCard)
	{
		int nColor = HNMJ::CGameLogic::Instance().GetCardColor(nCard);
		int nValue = HNMJ::CGameLogic::Instance().GetCardValue(nCard);

		std::string strValue = utility::toString(nValue);
		std::string strColor;
		if (nColor == CARD_COLOR_WAN)
		{
			strColor="wan";
		}
		else if (nColor == CARD_COLOR_TONG)
		{
			strColor="tong";
		}
		else if (nColor == CARD_COLOR_TIAO)
		{
			strColor="tiao";
		}
		else
		{
			strValue = "";
			strColor = "zhong";
		}
		std::string kName = utility::toString(strValue,strColor);
		if (iGender)
		{

			kName = utility::toString("g_",kName);
		}
		playEffect(kName);
	}

	void playEffectByGender(int iGender,std::string kName)
	{
		if (iGender)
		{
			kName =utility::toString("g_",kName);
		}
		else
		{
			kName =utility::toString("",kName);
		}

		playEffect(kName);
	}

	void playEffectEX(std::string kName)
	{
		SoundFun::playEffect(kName);
	}

} 
