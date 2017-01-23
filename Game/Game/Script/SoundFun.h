
#ifndef __SoundFun_FUN_H__
#define __SoundFun_FUN_H__

#include "cocos2d.h"

namespace SoundFun
{
	void playBackMusic(std::string kName);
	void stopBackMusic();
	void playEffect(std::string kName,bool bPreload = false);
	void playEffectDirect(std::string kName,bool bPreload = false);
	void playCardByGender(int iGender,std::string kName,bool bPreload = false);
	void PreloadEffectByGender(std::string kName);
	void upDatePlayerEffect();
	void SetSoundBackground(float fValue);
	void SetSoundEffect(float fValue);
	void setMute(bool bMute);
	void playCustomEffect(std::string kName);
} 

#endif // 
