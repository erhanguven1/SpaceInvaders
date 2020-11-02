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

#include "GameScene.h"

USING_NS_CC;

Scene* Game::createScene()
{
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2::ZERO);
    auto layer = Game::create();
    layer->SetPhysicsWorld(scene->getPhysicsWorld());

    scene->addChild(layer);

    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Game::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::initWithPhysics() )
    {
        return false;
    }
    this->getPhysicsWorld()->setFixedUpdateRate(1);


    Director::getInstance()->getOpenGLView()->setFrameSize(1280, 720);
    Director::getInstance()->getOpenGLView()->setDesignResolutionSize(1280, 720, ResolutionPolicy::EXACT_FIT);
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    this->setContentSize(Size(1280, 720));


    scoreText = Label::create();
    scoreText->setString("0");
    scoreText->setSystemFontSize(30);
    scoreText->setPosition(Vec2(visibleSize.width-45, visibleSize.height-30));

    this->addChild(scoreText);

    spaceShip = Sprite::create("spaceship.png");
    spaceShip->setPosition(Vec2(visibleSize.width/2, 0));
    spaceShip->setScale(visibleSize.width / 5000, visibleSize.height / 5000);
    this->addChild(spaceShip,2);

    auto side = cocos2d::PhysicsBody::createEdgeBox(this->getContentSize());
    side->setCollisionBitmask(0x000003);
    side->setContactTestBitmask(true);

    this->setPhysicsBody(side);

    auto mouseListener = EventListenerMouse::create();

    mouseListener->onMouseUp = CC_CALLBACK_1(Game::onMouseUp, this);
    mouseListener->onMouseMove = CC_CALLBACK_1(Game::onMouseMoved, this);
    mouseListener->onMouseDown = CC_CALLBACK_1(Game::onMouseDown, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
    
    this->scheduleUpdate();
    this->schedule(CC_SCHEDULE_SELECTOR(Game::updatePosition), enemyMoveTimeDelta);
    this->schedule(CC_SCHEDULE_SELECTOR(Game::fireBullet), bulletFireRate);
    
    auto contactListener = EventListenerPhysicsContact::create();

    contactListener->onContactBegin = CC_CALLBACK_1(Game::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    

    for (int i = 0; i < 16; i++)
    {
        auto a = EnemyShip::GenerateEnemy(new EnemyShipA(),this , Vec2(75 + (i) * 60, visibleSize.height - 75));
        this->addChild(a);
        enemyShips.push_back(a);
    }

    for (int i = 0; i < 16; i++)
    {
        auto a = EnemyShip::GenerateEnemy(new EnemyShipB(), this, Vec2(75 + (i) * 60, visibleSize.height-25));

        this->addChild(a);
        enemyShips.push_back(a);
    }

    
    for (size_t i = 0; i < 2; i++)
    {
        bulls.push_back(Bullet::GenerateBullet(spaceShip->getPosition() + Vec2::ANCHOR_TOP_LEFT * -60, 0.05f, this));
        this->addChild(bulls.back());

    }


    lastBulletIndex = 4;

    return true;
}

void Game::update(float delta) 
{
    if (streak) {
        streakCountdown -= delta;
        if (streakCountdown <= 0) {
            spaceShip->setColor(cocos2d::Color3B::WHITE);

            streak = false;
            streakCountdown = 3;
            bulletDamage = 25;
        }
    }

    if (isTouching)
    {
        float newPosX = clampf(spaceShip->getPosition().x + direction * 3, 30, visibleSize.width-30);
        spaceShip->setPositionX(newPosX);

    }
}
void Game::updatePosition(float delta) {

    for each (EnemyShip * var in enemyShips)
    {
        var->Update(moveDown ? -Vec2::ANCHOR_TOP_LEFT * 100 : -Vec2::ANCHOR_BOTTOM_RIGHT * 10 * enemyMoveDirection, 15);
    }
    moveDown = false;

}
void Game::fireBullet(float delta) {

    if (bulls.empty())
    {
        bulls.push_back(Bullet::GenerateBullet(spaceShip->getPosition() + Vec2::ANCHOR_TOP_LEFT * -200, 0.05f, this));
        this->addChild(bulls.back());
    }
    if (!bulls.back()->activated&& bulls.back()->getParent()==nullptr) {
        this->addChild(bulls.back());
    }

    if (streak) {
        bulls.back()->bulletSprite->setColor(Color3B::YELLOW);
    }
    else if(bulls.back()->bulletSprite->getColor()!= Color3B::WHITE) {
        bulls.back()->bulletSprite->setColor(Color3B::WHITE);
    }

    bulls.back()->Move(spaceShip->getPosition());
    bulls.pop_back();
}

bool Game::onContactBegin(cocos2d::PhysicsContact& contact) 
{
    PhysicsBody *body1 = contact.getShapeA()->getBody();
    PhysicsBody *body2 = contact.getShapeB()->getBody();

    PhysicsBody* bulletBody;
    PhysicsBody* enemyBody;

    bool bulletCollisionConditionWithEnemy = body1->getCollisionBitmask() == 0x000001 &&
        body2->getCollisionBitmask() == 0x000002;

    bool bulletCollisionConditionWithEnemy2 = body2->getCollisionBitmask() == 0x000001 &&
        body1->getCollisionBitmask() == 0x000002;



    if (bulletCollisionConditionWithEnemy || bulletCollisionConditionWithEnemy2)
    {
        if (bulletCollisionConditionWithEnemy) {
            enemyBody = body2;
            bulletBody = body1;
        }
        else {
            enemyBody = body1;
            bulletBody = body2;
        }

        auto a = (Bullet*)bulletBody->getNode();
        bulls.push_back(a);
        a->Deactivate(spaceShip->getPosition() + Vec2::ANCHOR_TOP_LEFT * -200);
        removeChild(a);

        auto b = (EnemyShip*)enemyBody->getNode();

        if(b != nullptr)
            b->TakeDamage(bulletDamage);

        if (b != nullptr && b->health <= 0) {

            enemyShips.erase(std::remove(enemyShips.begin(), enemyShips.end(), b));
            this->removeChild(b);

            if (enemyShips.size() == 0) {
                auto newScene = Game::createScene();
                _director->replaceScene(newScene);
            }

            
            killCounter++;
            scoreText->setString(std::to_string(killCounter));

            if (!streak) {
                streakCounter++;
            }


            if (!streak && streakCounter % 5 == 0)
            {
                spaceShip->setColor(cocos2d::Color3B::ORANGE);
                streakCounter = 0;
                bulletDamage = 100;
                streak = true;
            }
        }

    }

    bool bulletCollisionConditionWithEdge = body1->getCollisionBitmask() == 0x000003 &&
        body2->getCollisionBitmask() == 0x000001;
    bool bulletCollisionConditionWithEdge2 = body1->getCollisionBitmask() == 0x000001 &&
        body2->getCollisionBitmask() == 0x000003;

    if (bulletCollisionConditionWithEdge || bulletCollisionConditionWithEdge2)
    {
        if (bulletCollisionConditionWithEdge) {
            bulletBody = body2;
        }
        else {
            bulletBody = body1;

        }
        auto a = (Bullet*)bulletBody->getNode();
        bulls.push_back(a);
        a->Deactivate(spaceShip->getPosition() + Vec2::ANCHOR_TOP_LEFT * -200);
        removeChild(a);
    }

    bool enemyShipCollisionConditionWithEdge = body1->getCollisionBitmask() == 0x000003 &&
        body2->getCollisionBitmask() == 0x000002;

    if (enemyShipCollisionConditionWithEdge) 
    {
        moveDown = true;
        enemyMoveDirection *= -1;

        if (enemyShips[0]->getPositionY() < 0) {
                auto newScene = GameOver::createScene();
                _director->replaceScene(newScene);
        }
    }

    return true;
}

bool Game::onMouseUp(cocos2d::EventMouse* mouse)
{
    isTouching = false;
    return 1;
}
void Game::onMouseMoved(cocos2d::EventMouse* mouse)
{
    direction = mouse->getCursorX() > visibleSize.width / 2 ? 1 : -1;
}
bool Game::onMouseDown(cocos2d::EventMouse* mouse)
{
    isTouching = true;
    return 1;
}