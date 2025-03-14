#pragma once
#include "Player.h"
#include "Enemy.h"
#include "DX11Renderer.h"

class Game {
    Player player;
    Enemy enemy;

public:
    Game(ID3D11Device* device, ID3D11DeviceContext* context)
        : player(device, context, L"player.png"),
        enemy(device, context, L"enemy.png") {
    }

    void Update(float deltaTime) {
        player.Update(deltaTime);
        enemy.Update(deltaTime);
    }

    void Render(DX11Renderer& renderer) {
        
        player.Render(renderer);
        enemy.Render(renderer);
       // TestDrawRectangle(renderer); 

    //    MessageBox(NULL, L"Game::Render() called", L"Debug", MB_OK);
       
    }
    void TestDrawRectangle(DX11Renderer& renderer);
};

inline void Game::TestDrawRectangle(DX11Renderer& renderer) {
    renderer.TestDrawRectangle();
}
