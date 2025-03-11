#include "game.h"
#include <iostream>
#include "SpriteBatch.h"
#include "WICTextureLoader.h"
#include <d3d11.h>
#include <SimpleMath.h>
#include <wrl/client.h>

// Initialize static variable
Game* Game::instance = nullptr;
Game::Game() {
    instance = this;
    initD3D();

    // Ensure context is properly initialized
    if (context == nullptr) {
        // Handle the error or initialize context here
        std::cerr << "Error: context is not initialized." << std::endl;
        return;
    }

    spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
    bosses.push_back(Boss());
}

void Game::initD3D() {
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferDesc.Width = 800;
    scd.BufferDesc.Height = 600;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.BufferCount = 1;
    scd.OutputWindow = hwnd;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
        D3D11_SDK_VERSION, &scd, &swapChain,
        &device, nullptr, &context
    );

    if (FAILED(hr)) {
        MessageBox(nullptr, L"Failed to initialize Direct3D!", L"Error", MB_OK);
        exit(1);
    }
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
}

Game::~Game() {
    instance = nullptr; // Clear instance when the object is destroyed
}

void Game::processInput(WPARAM wParam) {
    switch (wParam) {
    case 'W':
        mainCharacter.move(0, -1);
        break;
    case 'A':
        mainCharacter.move(-1, 0);
        break;
    case 'S':
        mainCharacter.move(0, 1);
        break;
    case 'D':
        mainCharacter.move(1, 0);
        break;
    default:
        break;
    }
}

void Game::initWindow(HINSTANCE hInstance, int nCmdShow) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"GameWindowClass";

    RegisterClass(&wc);

    hwnd = CreateWindowEx(
        0,
        L"GameWindowClass",
        L"My Game",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr, nullptr, hInstance, nullptr
    );

    if (!hwnd) {
        MessageBox(nullptr, L"Failed to create window!", L"Error", MB_OK);
        exit(1);
    }
}

void Game::run() {
    ShowWindow(hwnd, SW_SHOW);

    MSG msg = {};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Update Boss positions
        for (Boss& boss : bosses) {
            boss.update(800, 600); // Screen limit 800x600
        }

        // Clear screen and start drawing
        context->ClearRenderTargetView(renderTargetView, DirectX::Colors::Black);
        
        spriteBatch->Begin();

        // Draw bosses
        for (Boss& boss : bosses) {
            boss.draw(context, spriteBatch.get());
        }

        // Draw main character
        mainCharacter.draw(context, spriteBatch.get());

        spriteBatch->End();
        swapChain->Present(1, 0); // Display new frame
    }
}

LRESULT CALLBACK Game::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_KEYDOWN:
        if (Game::instance) {  // Check if instance is created
            Game::instance->processInput(wParam);
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
