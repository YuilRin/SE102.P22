#include "Player.h"
#include "DX11Renderer.h"

void Player::Update(float deltaTime) {
    if (GetAsyncKeyState('W')) y -= 200.0f * deltaTime;
    if (GetAsyncKeyState('S')) y += 200.0f * deltaTime;
    if (GetAsyncKeyState('A')) x -= 200.0f * deltaTime;
    if (GetAsyncKeyState('D')) x += 200.0f * deltaTime;
}

void Player::Render(DX11Renderer& renderer) {
    if (texture) {
        renderer.DrawTexture(texture, x, y, width, height);
    }
    if (!texture) {
        MessageBox(NULL, L"Texture is null!", L"Error", MB_OK);
    }

}
