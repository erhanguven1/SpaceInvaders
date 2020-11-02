#ifndef __ENEMYSHIP_H__
#define __ENEMYSHIP_H__

#include "cocos2d.h"

#define string std::string

class EnemyShip : public cocos2d::Sprite {
public:
    cocos2d::Sprite *shipSprite;
    cocos2d::Vec2 startPos;

    char *spriteName;
    int health;
    float scale;

    static EnemyShip* GenerateEnemy(EnemyShip*, cocos2d::Scene* scene, cocos2d::Vec2 position);

    EnemyShip(char* _spriteName, int _health, float _scale) {
        spriteName = _spriteName;
        health = _health;
        scale = _scale;
    }

    void Update(cocos2d::Vec2 moveDir, float moveCount);

    void Init(cocos2d::Scene *scene, cocos2d::Vec2 position);
    void TakeDamage(int dmg);
};

class EnemyShipA : public EnemyShip {
public:
    EnemyShipA() :EnemyShip("enemyA.png", 50, .05f) {}
};

class EnemyShipB : public EnemyShip {
public:
    EnemyShipB() :EnemyShip("enemyB.png", 100, .1f) {}
};


#endif //__ENEMYSHIP_H__