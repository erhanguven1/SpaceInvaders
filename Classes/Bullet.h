#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"
class Bullet:public cocos2d::Sprite
{
public:
	cocos2d::Sprite* bulletSprite;
	cocos2d::Vec2 startPos;
	int id;
	float scale;

	bool activated;

	cocos2d::Action *action;

	static Bullet* GenerateBullet(cocos2d::Vec2 startPosition, float scale, cocos2d::Scene* scene);
	
	void Init(cocos2d::Vec2, float, cocos2d::Scene*);
	void Move(cocos2d::Vec2);
	void Move(cocos2d::Vec2, bool direction);
	void Deactivate(cocos2d::Vec2);

};
#endif //__BULLET_H__