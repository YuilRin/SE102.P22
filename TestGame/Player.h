#pragma once
#include "GameObject.h"
#include "DX11Renderer.h"

class Player : public GameObject {
public:
    ID3D11ShaderResourceView* texture;

    Player(ID3D11Device* device, ID3D11DeviceContext* context, const std::wstring& texturePath)
        : GameObject(100, 300, 50, 50) // Thêm width và height
    {
        texture = DX11Renderer::LoadTexture(device, context, texturePath);
       
    }

    void Update(float deltaTime) override;
    void Render(DX11Renderer& renderer) override;
};
