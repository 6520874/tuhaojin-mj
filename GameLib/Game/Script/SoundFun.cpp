#include "SoundFun.h"
#include "SimpleAudioEngine.h"
#include "Game/Script/utility.h"

namespace SoundFun
{

	static const std::string s_soundPath = "raw/";
	static bool s_bMute = false;
	static float s_fSound1 = 0;
	static float s_fSound2 = 0;

	void SetSoundBackground(float fValue)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume( fValue*(!s_bMute));
		s_fSound1 = fValue;
	}
	float GetSoundBackground()
	{
		return CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
	}
	void SetSoundEffect(float fValue)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume( fValue * fValue);
		s_fSound2 = fValue;
	}
	void setUseSoudType(int iType)
	{
	}
	float GetSoundEffect()
	{
		return CocosDenshion::SimpleAudioEngine::getInstance()->getEffectsVolume();
	}
	void playBackMusic(std::string kName)
	{
		kName = s_soundPath + kName;
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(kName.c_str(),true);	
	}
	void stopBackMusic()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();	
	}

	void playEffect(std::string kName,bool bPreload)
	{
		if (s_bMute)
		{
			return;
		}
		kName = s_soundPath + kName;
		if (bPreload)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(kName.c_str());
		}
		else
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(kName.c_str());
		}
	}
	void playEffectDirect(std::string kName,bool bPreload )
	{

		if (s_bMute)
		{
			return;
		}
		if (bPreload)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(kName.c_str());
		}
		else
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(kName.c_str());
		}
	}
	void playCardByGender(int iGender,std::string kName,bool bPreload)
	{
		if (iGender)
		{
			kName = "F_" + kName;
		}
		else
		{
			kName = "M_" + kName;
		}
		playEffect(kName,bPreload);
	}
	void PreloadEffectByGender(std::string kName)
	{
		playCardByGender(0,kName,true);
		playCardByGender(1,kName,true);
	}
	void setMute(bool bMute)
	{
		if (s_bMute == bMute)
		{
			return;
		}
		SetSoundBackground(s_fSound1);
		SetSoundEffect(s_fSound2);
		s_bMute = bMute;
		cocos2d::UserDefault::getInstance()->setBoolForKey("Mute",bMute);
	}

	void playCustomEffect(std::string kName)
	{
		if (s_bMute)
		{
			return;
		}
		
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(kName.c_str());
	}

} 
