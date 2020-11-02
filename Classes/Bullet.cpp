#include "Bullet.h"
#include <time.h>

USING_NS_CC;

Bullet* Bullet::GenerateBullet(cocos2d::Vec2 startPosition, float _scale, cocos2d::Scene* scene) {
    auto bullet = new(std::nothrow) Bullet;
    if (bullet && bullet->init()) {
        bullet->scale = _scale;
        bullet->activated = true;
        //bullet->autorelease();
        
        bullet->bulletSprite = bullet->create("bullet.png");
        bullet->setPosition(startPosition);
        bullet->setScale(_scale, _scale);

        bullet->addChild(bullet->bulletSprite);
        bullet->bulletSprite->setPosition(Vec2::ZERO);

        bullet->Init(startPosition, _scale, scene);
        return bullet;
    }
    CC_SAFE_RELEASE(bullet);
    return nullptr;
}

void Bullet::Init(cocos2d::Vec2 startPosition, float scale, cocos2d::Scene* scene) {

    startPos = startPosition;
    CCLOG("Initialized bullet!");

    auto bulletBody = cocos2d::PhysicsBody::createBox(Size(250, 250));

    bulletBody->setRotationEnable(false);
    bulletBody->setDynamic(false);
    bulletBody->setGravityEnable(false);
    bulletBody->setCollisionBitmask(0x000001);
    bulletBody->setCategoryBitmask(0x000001);
    bulletBody->setContactTestBitmask(true);
    bulletBody->setContactTestBitmask(true);

    this->setPhysicsBody(bulletBody);
}

void Bullet::Move(cocos2d::Vec2 playerPos)
{
    this->setPosition(playerPos + cocos2d::Vec2::ANCHOR_TOP_LEFT * 50);
    this->getPhysicsBody()->setVelocity(cocos2d::Vec2(0, 500));
}

void Bullet::Deactivate(cocos2d::Vec2 pos) {
    this->activated = false;

    this->getPhysicsBody()->setVelocity(cocos2d::Vec2(0, 0));
    this->setPosition(pos);

}