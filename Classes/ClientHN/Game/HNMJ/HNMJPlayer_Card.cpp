#include "HNMJPlayer.h"
#include "HNMJGameLogic.h"

void HNMJPlayer::setHandCard(BYTE* pCardData,int iCout)
{
	memcpy(m_kHandCardData,pCardData,sizeof(m_kHandCardData));
	m_kHandCardCout = iCout;
}
void HNMJPlayer::setWeaveItem(CMD_WeaveItem* pWeave,int iCout)
{
	memcpy(m_kWeaveItemArray,pWeave,sizeof(m_kWeaveItemArray));
	m_kWeaveCount = iCout;
}
void HNMJPlayer::setOperateResoult(CMD_S_OperateResult* pNetInfo)
{
	HNMJ::CGameLogic kGameLogic;
	bool cbPublicCard = false;
	word wOperateUser=pNetInfo->wOperateUser;
	BYTE cbOperateCard=pNetInfo->cbOperateCard;
	if ((pNetInfo->cbOperateCode&WIK_GANG)!=0)
	{
		//组合扑克
		BYTE cbWeaveIndex=0xFF;
		for (BYTE i=0;i<m_kWeaveCount;i++)
		{
			BYTE cbWeaveKind=m_kWeaveItemArray[i].cbWeaveKind;
			BYTE cbCenterCard=m_kWeaveItemArray[i].cbCenterCard;
			if ((cbCenterCard==pNetInfo->cbOperateCard)&&(cbWeaveKind==WIK_PENG))
			{
				cbWeaveIndex=i;
				m_kWeaveItemArray[cbWeaveIndex].cbPublicCard=true;
				m_kWeaveItemArray[cbWeaveIndex].cbWeaveKind=pNetInfo->cbOperateCode;
				m_kWeaveItemArray[cbWeaveIndex].wProvideUser=pNetInfo->wProvideUser;
				break;
			}
		}

		//组合扑克
		if (cbWeaveIndex==0xFF)
		{
			//暗杠判断
			cbPublicCard=(pNetInfo->wProvideUser==wOperateUser)?FALSE:TRUE;

			//设置扑克
			cbWeaveIndex=m_kWeaveCount++;
			m_kWeaveItemArray[cbWeaveIndex].cbPublicCard=cbPublicCard;
			m_kWeaveItemArray[cbWeaveIndex].cbCenterCard=cbOperateCard;
			m_kWeaveItemArray[cbWeaveIndex].cbWeaveKind=pNetInfo->cbOperateCode;
			m_kWeaveItemArray[cbWeaveIndex].wProvideUser=pNetInfo->wProvideUser;
		}

		//扑克设置
		if (m_iIdex == 0)
		{
			m_iOutCardCout = kGameLogic.RemoveValueCardAll(m_kHandCardData,m_iOutCardCout,pNetInfo->cbOperateCard);
		}
		else
		{
			m_kHandCardCout = MAX_COUNT - m_kWeaveCount*3;
		}
	}
	else if (pNetInfo->cbOperateCode!=WIK_NULL)
	{
		//设置组合
		BYTE cbWeaveIndex=m_kWeaveCount++;
		m_kWeaveItemArray[cbWeaveIndex].cbPublicCard=true;
		m_kWeaveItemArray[cbWeaveIndex].cbCenterCard=cbOperateCard;
		m_kWeaveItemArray[cbWeaveIndex].cbWeaveKind=pNetInfo->cbOperateCode;
		m_kWeaveItemArray[cbWeaveIndex].wProvideUser=pNetInfo->wProvideUser;

		//组合界面
		//删除扑克
		if (m_iIdex == 0)
		{
			BYTE cbWeaveCard[4]={cbOperateCard,cbOperateCard,cbOperateCard,cbOperateCard};
			kGameLogic.RemoveValueCard(m_kHandCardData,m_kHandCardCout,cbWeaveCard,2);
			m_kHandCardCout -= 2;
		}
		else
		{
			m_kHandCardCout = MAX_COUNT - m_kWeaveCount*3;
		}
	}
	showHandCard();
}
void HNMJPlayer::addNewInCard(int iCard)
{
	m_kHandCardCout++;
	m_kHandCardData[MAX_COUNT-1-m_kWeaveCount*3] = iCard;
}
void HNMJPlayer::setActOutCard(int iCard)
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"BigOutNode");
	pNode->setVisible(iCard >= 0);

	if (iCard < 0)
	{
		return;
	}
	std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
	setCardImagic(pNode,iCard,kImagic);

	if (m_iIdex == 0)
	{
		HNMJ::CGameLogic kGameLogic;
		kGameLogic.RemoveValueCardOne(m_kHandCardData,m_kHandCardCout,iCard);
		m_kHandCardCout --;
		kGameLogic.SortCardList(m_kHandCardData,m_kHandCardCout);
	}
	else
	{
		m_kHandCardCout -- ;
	}

	showHandCard();

}
void HNMJPlayer::addHandOutCard(int iCard)
{
	int iIdex = m_iIdex;
	cocos2d::Node* pCardNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode2");
	cocos2d::Vec2 kStartPos = WidgetFun::getChildWidget(m_pSeatNode,"OutPosNode")->getPosition();
	int iAddOder = utility::parseInt(WidgetFun::getWidgetUserInfo(
		m_pSeatNode,"OutPosNode","AddOder"));
	cocos2d::Vec2 kOutAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(
		m_pSeatNode,"OutPosNode","OutAddPos"));
	cocos2d::Vec2 kNextAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(
		m_pSeatNode,"OutPosNode","NextAddPos"));
	cocos2d::Vec2 kPos;
	if (m_iOutCardCout >= 10)
	{
		kPos = kOutAddPos*(m_iOutCardCout-10)+kStartPos;
		kPos += kNextAddPos;
	}
	else
	{
		kPos = kOutAddPos*m_iOutCardCout+kStartPos;
	}
	std::string kHandSkin = utility::toString("HNMJ_OUT_",iIdex);

	cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kHandSkin,pCardNode);
	pNode->setZOrder(iAddOder*m_iOutCardCout);
	pNode->setPosition(kPos);
	std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
	setCardImagic(pNode,iCard,kImagic);

	m_iOutCardCout++;
}
void HNMJPlayer::showHandCard()
{
	showHandCard(m_kWeaveItemArray,m_kWeaveCount,m_kHandCardData,m_kHandCardCout);
}
void HNMJPlayer::showHandCard(CMD_WeaveItem* pWeave,int iWeaveCout,BYTE* pHandCard,int iHandCout)
{
	bool bHaveNewIn = (iWeaveCout*3+iHandCout) == MAX_COUNT;
	int iIdex = m_iIdex;
	cocos2d::Node* pCardNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	pCardNode->removeAllChildren();
	cocos2d::Vec2 kStartPos = WidgetFun::getChildWidget(m_pSeatNode,"HandPosNode")->getPosition();
	int iAddOder = utility::parseInt(WidgetFun::getWidgetUserInfo(
		m_pSeatNode,"HandPosNode","AddOder"));
	cocos2d::Vec2 kHandAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(
		m_pSeatNode,"HandPosNode","HandAddPos"));
	cocos2d::Vec2 kNewInAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(
		m_pSeatNode,"HandPosNode","NewInAddPos"));
	std::string kPengSkin = utility::toString("HNMJ_PENG_",iIdex);
	std::string kGangSkin = utility::toString("HNMJ_GANG_",iIdex);
	std::string kHandSkin = utility::toString("HNMJ_HAND_",iIdex);
	int iOder = 0;
	for (int i = 0;i<iWeaveCout;i++)
	{
		CMD_WeaveItem* pTemp = pWeave+i;
		int iCardValue = pTemp->cbCenterCard;
		if (pTemp->cbPublicCard == 0)
		{
			iCardValue = -1;
		}

		if (pTemp->cbWeaveKind == WIK_PENG)
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kPengSkin,pCardNode);
			pNode->setZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kHandAddPos*3;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue,kImagic);
		}
		if (pTemp->cbWeaveKind == WIK_GANG)
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kGangSkin,pCardNode);
			pNode->setZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kHandAddPos*3;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card3"),iCardValue,kImagic);
		}
		iOder+=iAddOder;
	}
	for (int i = 0;i<iHandCout;i++)
	{
		BYTE* pTemp = pHandCard+i;
		cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kHandSkin,pCardNode);
		pNode->setTag(1);
		pNode->setZOrder(iOder);
		pNode->setPosition(kStartPos);
		if (bHaveNewIn && i == (iHandCout - 2))
		{
			kStartPos += kNewInAddPos;
		}
		else
		{
			kStartPos += kHandAddPos;
		}
		int iCardValue = *pTemp;
		if (iIdex == 0)
		{
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(pNode,iCardValue,kImagic);
		}
		iOder+=iAddOder;
	}
}
void HNMJPlayer::setCardImagic(cocos2d::Node* pNode,int kValue,std::string kImagicFront)
{
	if (kValue > 0)
	{
		BYTE cbValue=((BYTE)kValue&MASK_VALUE);
		BYTE cbColor=(((BYTE)kValue&MASK_COLOR)>>4)+1;
		WidgetFun::setImagic(pNode,utility::toString(kImagicFront,(int)cbColor,(int)cbValue,".png"),false);
	}
	else
	{
		WidgetFun::setImagic(pNode,utility::toString(kImagicFront,0,0,".png"),false);
	}
}
cocos2d::Node* HNMJPlayer::getTouchCardNode(cocos2d::Vec2 kTouchPos)
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		if (utility::IsSpriteInTouch(kTouchPos,pSprite))
		{
			return pSprite;
		}
	}
	return NULL;
}

BYTE HNMJPlayer::getTouchCardVlaue(cocos2d::Node* pNode)
{
	int iIdex = 0;
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		if (pSprite == pNode)
		{
			return m_kHandCardData[iIdex];
		}
		iIdex++;
	}
	return NULL;
}
void HNMJPlayer::showHuPai(bool bZiMo)
{
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		float fHuScale = utility::parseFloat(WidgetFun::getWidgetUserInfo(pSprite,"HuScale"));
		pSprite->setScale(fHuScale);
		std::string kImagic = WidgetFun::getWidgetUserInfo(pSprite,"MingImagic");
		if (i == (pRootNode->getChildrenCount()-1) && !bZiMo)
		{
			setCardImagic(pSprite,m_kHandCardData[m_kHandCardCout-1],kImagic);
		}
		else
		{
			setCardImagic(pSprite,0,kImagic);
		}
	}
}