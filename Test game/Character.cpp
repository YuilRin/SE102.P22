#include "Character.h"
#include <iostream>

ID3D11ShaderResourceView* characterTexture = nullptr; // ??nh ngh?a bi?n toàn c?c

Character::Character(const std::string& name, int hp) : x(0), y(0), speed(2.0f), hp(hp) {}

Character::Character() : x(100), y(100), speed(2.0f), hp(100) {}

Character::~Character() {}

void Character::attack() {
    std::cout << "Character attacks!" << std::endl;
}

std::string Character::getName() const {
    return "Character";
}

int Character::getHP() const {
    return hp;
}

void Character::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
}

void Character::move(float dx, float dy) {
    x += dx * speed;
    y += dy * speed;
}

void Character::draw(ID3D11DeviceContext* context, DirectX::SpriteBatch* spriteBatch) {
    if (spriteBatch && characterTexture) {
        spriteBatch->Draw(characterTexture, DirectX::XMFLOAT2(x, y));
    }
}
