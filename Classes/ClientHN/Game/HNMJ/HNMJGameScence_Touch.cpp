#include "HNMJGameScence.h"
#include "HNMJPlayer.h"
#include "Game/GameLib.h"
#include "CMD_HNMJ.h"
#include "HNMJPlayer.h"

class HNMJTouchScence
	:public cocos2d::Layer
{
public:
	virtual bool init()
	{
		if ( !cocos2d::Layer::init() )
		{
			return false;
		}
		setTouchEnabled(true);
		setTouchMode(cocos2d::Touch::DispatchMode::ONE_BY_ONE);
		return true;
	}
public:
	CREATE_FUNC(HNMJTouchScence);
public:
	virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
	{
		if (!utility::isAllVisible(this))
		{
			return false;
		}
		if (pTouch->getID() != 0)
		{
			return false;
		}
		return HNMJGameScence::Instance().ccTouchBegan(pTouch->getLocation());
	}
	virtual void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
	{
		if (!utility::isAllVisible(this))
		{
			return;
		}
		if (pTouch->getID() != 0)
		{
			return;
		}
		return HNMJGameScence::Instance().ccTouchMoved(pTouch->getLocation());
	}
	virtual void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
	{
		if (!utility::isAllVisible(this))
		{
			return;
		}
		if (pTouch->getID() != 0)
		{
			return;
		}
		return HNMJGameScence::Instance().ccTouchEnded(pTouch->getLocation());
	}
};

void HNMJGameScence::initTouch()
{
	HNMJTouchScence* pTouchScence = new HNMJTouchScence;
	pTouchScence->init();
	addChild(pTouchScence);
}

bool HNMJGameScence::ccTouchBegan(cocos2d::Vec2 kPos)
{
	if (!isVisible())
	{
		return false;
	}
	if(m_pLocal->GetChairID() != m_iCurrentUser)
	{
		return false;
	}
	m_pTouchCardNode = m_pLocal->getTouchCardNode(kPos);
	if (!m_pTouchCardNode)
	{
		return false;
	}
	m_kTouchSrcPos = m_pTouchCardNode->getPosition();
	ccTouchMoved(kPos);
	return true;
}
void HNMJGameScence::ccTouchMoved(cocos2d::Vec2 kPos)
{
	cocos2d::Vec2 kTempPos = m_pTouchCardNode->getParent()->convertToNodeSpace(kPos);
	if (kTempPos.y < m_kTouchSrcPos.y)
	{
		kTempPos.y = m_kTouchSrcPos.y;
	}
	kTempPos.x += m_pTouchCardNode->getContentSize().width*(m_pTouchCardNode->getAnchorPoint().x-0.5f);
	kTempPos.y += m_pTouchCardNode->getContentSize().height*0.5f;
	m_pTouchCardNode->setPosition(kTempPos);
}
void HNMJGameScence::ccTouchEnded(cocos2d::Vec2 kPos)
{
	cocos2d::Vec2 kTempPos = m_pTouchCardNode->getParent()->convertToNodeSpace(kPos);
	m_pTouchCardNode->setPosition(m_kTouchSrcPos);
	if (kTempPos.y > m_kTouchSrcPos.y)
	{
		CMD_C_OutCard OutCard;
		OutCard.cbCardData=m_pLocal->getTouchCardVlaue(m_pTouchCardNode);
		SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));
	}
}