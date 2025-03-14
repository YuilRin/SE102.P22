#pragma once
#include "GameObject.h"
#include "DX11Renderer.h"

class Enemy : public GameObject {
public:
    float speedX = 100, speedY = 100;
    ID3D11ShaderResourceView* texture;

    Enemy(ID3D11Device* device, ID3D11DeviceContext* context, const std::wstring& texturePath)
        : GameObject(400, 300, 64, 64, 100, 100) // Thêm width, height, speedX, speedY
    {
        texture = DX11Renderer::LoadTexture(device, context, texturePath);
    }

    void Update(float deltaTime) override;
    void Render(DX11Renderer& renderer) override;
};
