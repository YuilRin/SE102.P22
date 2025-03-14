#ifndef __GAME_H__
#define __GAME_H__

#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class Game
{
public:
    Game(HINSTANCE hInstance, LPWSTR name, int width, int height, int fps, int isFullScreen);
    virtual ~Game();

    void virtual init();
    void virtual release();

    void virtual updateInput(float deltatime);
    void virtual update(float deltatime);
    void virtual draw();
    void virtual loadResource();

    void run();
    void render();
    static void exit();

protected:
    HWND hWnd;
    int screenWidth, screenHeight;
    float frameRate;

    static int isExit;

    ID3D11Device* device;
    ID3D11DeviceContext* deviceContext;
    IDXGISwapChain* swapChain;
    ID3D11RenderTargetView* renderTargetView;

    void initDirect3D();
    void cleanupDirect3D();
};

#endif // __GAME_H__
