#include "GameOverScene.h"

USING_NS_CC;

Scene* GameOver::createScene()
{
    auto scene = Scene::create();

    auto layer = GameOver::create();

    scene->addChild(layer);

    return layer;
}

bool GameOver::init() {
    if (!Scene::init()) {
        return false;
    }
    Director::getInstance()->getOpenGLView()->setFrameSize(1280, 720);
    Director::getInstance()->getOpenGLView()->setDesignResolutionSize(1280, 720, ResolutionPolicy::EXACT_FIT);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    this->setContentSize(Size(1280, 720));

    auto label = Label::create();
    auto moreButton = MenuItemImage::create("retry.png", "retry.png", [](Ref* sender) {});


    label->setString("YOU ARE A DEAD SOUL NOW...");
    label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 250));
    this->addChild(label);

    moreButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    moreButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    moreButton->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    moreButton->setScale(.2f);
    auto menu = Menu::create(moreButton, NULL);
    menu->setPosition(0, 0);

    this->addChild(menu);


    return true;
}