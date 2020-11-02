/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "EnemyShip.h"
#include "Bullet.h"
#include "GameOverScene.h"

class Game : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    std::vector<EnemyShip*> enemyShips;

    std::vector<Bullet*> bulls;

    int lastBulletIndex;

    bool isTouching;
    int direction;
    int enemyMoveDirection=1;
    int killCounter, streakCounter;
    bool streak;
    float streakCountdown = 3;
    
    bool moveDown;

    float enemyMoveTimeDelta = 1 / 3.0f;
    float bulletFireRate = 1 / 5.0f;

    int bulletDamage = 25;
    
    cocos2d::Label* scoreText;

    cocos2d::Size visibleSize;

    cocos2d::Sprite* spaceShip;

    bool onMouseUp(cocos2d::EventMouse* mouse);
    void onMouseMoved(cocos2d::EventMouse* mouse);
    bool onMouseDown(cocos2d::EventMouse* mouse);

    bool onContactBegin(cocos2d::PhysicsContact& contact);

    virtual bool init();
    void SetPhysicsWorld(cocos2d::PhysicsWorld* world) { sceneWorld = world; };
    cocos2d::PhysicsWorld* sceneWorld;

    void update(float) override;
    void updatePosition(float);
    void fireBullet(float);

    CREATE_FUNC(Game);
};


#endif // __GAMESCENE_H__
