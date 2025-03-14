#include <windows.h>
#include "DX11Renderer.h"
#include "Game.h"

DX11Renderer renderer;
Game* game = nullptr; // Dùng con tr? ?? kh?i t?o v?i device & context
bool running = true;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_DESTROY) {
        running = false;
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 1. Kh?i t?o c?a s?
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WindowProc, 0L, 0L, GetModuleHandle(nullptr),
                      nullptr, nullptr, nullptr, nullptr, L"DX11Game", nullptr };
    RegisterClassEx(&wc);
    HWND hWnd = CreateWindow(wc.lpszClassName, L"DirectX 11 Game tuan 1", WS_OVERLAPPEDWINDOW,
        0, 0, 800, 600, nullptr, nullptr, wc.hInstance, nullptr);
    if (!hWnd) return 0;

    // 2. Kh?i t?o DirectX 11
    if (!renderer.Init(hWnd)) return 0;

    // 3. T?o game và truy?n device, context
    game = new Game(renderer.device, renderer.context);

    ShowWindow(hWnd, nCmdShow);

    MSG msg;
    while (running) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // 4. C?p nh?t và render game
        renderer.ClearScreen();
        game->Update(0.016f);  // Gi? s? FPS c? ??nh 60
        game->Render(renderer);
        renderer.Present();
    }

    // 5. D?n d?p tài nguyên
    delete game;
    renderer.Cleanup();
    UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}
