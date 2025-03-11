#include "Boss.h"
#include <iostream>

std::vector<Boss> bosses;
ID3D11ShaderResourceView* bossTexture = nullptr; // ??nh ngh?a bi?n toàn c?c

Boss::Boss() : lastMoveTime(0), dirX(1), dirY(1) {
    x = 200;
    y = 200;
    speed = 2.0f;
    hp = 300;
}


Boss::~Boss() {}

void Boss::attack() {
    std::cout << "Boss attacks ferociously!" << std::endl;
}

void Boss::specialAttack() {
    std::cout << "Boss unleashes a powerful attack!" << std::endl;
}

void Boss::update(float screenWidth, float screenHeight) {
    x += dirX * speed;
    y += dirY * speed;

    if (x <= 0 || x >= screenWidth - 50) dirX *= -1;
    if (y <= 0 || y >= screenHeight - 50) dirY *= -1;
}

void Boss::draw(ID3D11DeviceContext* context, DirectX::SpriteBatch* spriteBatch) {
    if (spriteBatch && bossTexture) {
        spriteBatch->Draw(bossTexture, DirectX::XMFLOAT2(x, y));
    }
}
