#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

class DX11Renderer; // Khai báo tr??c ?? tránh l?i

class GameObject {
protected:
    float x, y;        // T?a ??
    float speedX, speedY; // V?n t?c
    float width=100, height=100; // Kích th??c
public:
    GameObject(float startX, float startY, float w, float h, float spX = 0, float spY = 0)
        : x(startX), y(startY), width(w), height(h), speedX(spX), speedY(spY) {
    }

    virtual void Update(float deltaTime) = 0;
    virtual void Render(DX11Renderer& renderer) = 0;

    // Getter
    float GetX() const { return x; }
    float GetY() const { return y; }
    float GetWidth() const { return width; }
    float GetHeight() const { return height; }
};
