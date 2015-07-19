#include "Enemy.h"
#include "Character.h"
#include "Uber.h"

Enemy::Enemy(float spawnX, float spawnY) : Character(spawnX, spawnY, Uber::I().enemySprite) {
}


Enemy::~Enemy() {
}

void Enemy::GotHit(Character* source) {

}