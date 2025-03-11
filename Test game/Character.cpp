#include "Character.h"
#include <iostream>

Character::Character(const std::string& name, int hp) : name(name), hp(hp) {}

Character::~Character() {}

void Character::attack() {
    std::cout << name << " attacks!" << std::endl;
}

std::string Character::getName() const {
    return name;
}

int Character::getHP() const {
    return hp;
}

void Character::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
}
