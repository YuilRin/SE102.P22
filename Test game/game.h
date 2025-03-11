#ifndef GAME_H
#define GAME_H
#include "Character.h"
#include "Boss.h"
#include <vector>
#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

class Game {
public:
    static Game* instance; // Thêm bi?n instance ?? l?u ??i t??ng hi?n t?i
    void initWindow(HINSTANCE hInstance, int nCmdShow);
    Game();
    ~Game();
    void initD3D();
    void run();
    void processInput(WPARAM wParam);

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    HWND hwnd;
    Character mainCharacter; // Thêm nhân v?t chính
    std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
    ID3D11Device* device;
    ID3D11DeviceContext* context; // Declare context
    ID3D11RenderTargetView* renderTargetView; // Declare renderTargetView
    IDXGISwapChain* swapChain;
};
extern std::vector<Boss> bosses;
#endif // GAME_H
