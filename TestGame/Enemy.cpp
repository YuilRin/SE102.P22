#include "Enemy.h"
#include "DX11Renderer.h"

void Enemy::Update(float deltaTime) {
    x += speedX * deltaTime;
    y += speedY * deltaTime;

    if (x < 0 || x > 800 - width) speedX = -speedX;
    if (y < 0 || y > 600 - height) speedY = -speedY;
}

void Enemy::Render(DX11Renderer& renderer) {
    if (texture) {
        renderer.DrawTexture(texture, x, y, width, height);
       
    }
    if (!texture) {
        MessageBox(NULL, L"Texture is null!", L"Error", MB_OK);
    }

}
