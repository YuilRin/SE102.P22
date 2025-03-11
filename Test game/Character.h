#pragma once
#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <d3d11.h>
#include "SpriteBatch.h"

class Character {
public:
    Character();
    Character(const std::string& name, int hp);
    virtual ~Character();

    virtual void attack();
    virtual std::string getName() const;
    int getHP() const;
    void takeDamage(int damage);
    void move(float dx, float dy);

    virtual void draw(ID3D11DeviceContext* context, DirectX::SpriteBatch* spriteBatch);

protected:
    float x, y;
    float speed;
    int hp;
};

// Bi?n toàn c?c ch?a texture c?a Character
extern ID3D11ShaderResourceView* characterTexture;

#endif
