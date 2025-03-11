#pragma once
#ifndef BOSS_H
#define BOSS_H

#include "Character.h"
#include <vector>
#include <d3d11.h>
#include "SpriteBatch.h"

class Boss : public Character {
public:
    Boss();
    ~Boss();

    void attack() override;
    void specialAttack();
    void update(float screenWidth, float screenHeight);
    void draw(ID3D11DeviceContext* context, DirectX::SpriteBatch* spriteBatch) override;

private:
    float lastMoveTime;
    int dirX, dirY;
};

// Bi?n toàn c?c ch?a texture c?a Boss
extern ID3D11ShaderResourceView* bossTexture;

#endif
