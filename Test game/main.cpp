﻿//#include <windows.h>
//#include <d3d11.h>
//#include <DirectXMath.h>

#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "SpriteBatch.h"
#include "WICTextureLoader.h"
#include <cstdlib>
#include <ctime>

#pragma comment(lib, "d3d11.lib")

using namespace DirectX;

// Cấu hình cửa sổ
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
HWND hwnd = NULL;
ID3D11Device* device = nullptr;
ID3D11DeviceContext* deviceContext = nullptr;
IDXGISwapChain* swapChain = nullptr;
ID3D11RenderTargetView* renderTargetView = nullptr;
std::unique_ptr<SpriteBatch> spriteBatch;
ID3D11ShaderResourceView* textureChar1 = nullptr;
ID3D11ShaderResourceView* textureChar2 = nullptr;
ID3D11ShaderResourceView* textureChar3 = nullptr;
ID3D11ShaderResourceView* currentTexture = nullptr;


// Vị trí nhân vật
float charX = 300.0f, charY = 100.0f;
const float charSpeed = 10.0f;

// Vị trí boss
float bossX = 100.0f, bossY = 400.0f;
float bossSpeed = 0.1f;
float bossLastMoveTime = 0.0f;
int bossDirX = 1, bossDirY = 1;

// Xử lý phím
void HandleKeyPress(WPARAM key) {
    switch (key) {
    case 'A': case VK_LEFT:
        if(charX>=10)
            charX -= charSpeed;
        break;
    case 'D': case VK_RIGHT:
        if (charX <=SCREEN_WIDTH-30)
        charX += charSpeed;
        break;
    case 'W': case VK_UP:
        if (charY >= 10)
        charY -= charSpeed;
        break;
    case 'S': case VK_DOWN:
        if (charY <= SCREEN_HEIGHT-50)
        charY += charSpeed;
        break;
    }
}

// Hàm xử lý Windows
LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN:
        HandleKeyPress(wParam);
        return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

// Khởi tạo DirectX 11
bool InitD3D(HWND hwnd) {
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hwnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;
    scd.BufferDesc.Width = SCREEN_WIDTH;
    scd.BufferDesc.Height = SCREEN_HEIGHT;

    if (FAILED(D3D11CreateDeviceAndSwapChain(
        NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0,
        D3D11_SDK_VERSION, &scd, &swapChain, &device, NULL, &deviceContext))) {
        MessageBox(hwnd, L"Failed to create device & swap chain", L"Error", MB_OK);
        return false;
    }

    ID3D11Texture2D* backBuffer;
    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
    backBuffer->Release();
    deviceContext->OMSetRenderTargets(1, &renderTargetView, NULL);

    D3D11_VIEWPORT viewport = { 0.0f, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, 1.0f };
    deviceContext->RSSetViewports(1, &viewport);

    spriteBatch = std::make_unique<SpriteBatch>(deviceContext);

    if (FAILED(CreateWICTextureFromFile(device, L"char1.png", nullptr, &textureChar1))) return false;
    if (FAILED(CreateWICTextureFromFile(device, L"char2.png", nullptr, &textureChar2))) return false;
    if (FAILED(CreateWICTextureFromFile(device, L"char3.png", nullptr, &textureChar3))) return false;

    currentTexture = textureChar1;
    srand((unsigned int)time(NULL));
    return true;
}

// Vẽ màn hình
void RenderFrame(float elapsedTime) {
    float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    deviceContext->ClearRenderTargetView(renderTargetView, clearColor);

    if (elapsedTime - bossLastMoveTime >= 1.0f) {
        bossDirX = (rand() % 3) - 1;
        bossDirY = (rand() % 3) - 1;
        bossLastMoveTime = elapsedTime;
    }

    bossX += bossDirX * bossSpeed;
    bossY += bossDirY * bossSpeed;

    if (bossX <= 0 || bossX >= SCREEN_WIDTH - 20) bossDirX *= -1;
    if (bossY <= 0 || bossY >= SCREEN_HEIGHT - 20) bossDirY *= -1;

    if (abs(charX - bossX) < 20 && abs(charY - bossY) < 20) {
        MessageBox(hwnd, L"Game Over!", L"Alert", MB_OK);
        PostQuitMessage(0);
    }

    spriteBatch->Begin();
    spriteBatch->Draw(currentTexture, XMFLOAT2(charX, charY));
    spriteBatch->Draw(textureChar3, XMFLOAT2(bossX, bossY));
    spriteBatch->End();

    swapChain->Present(0, 0);
}

// Chạy vòng lặp game
void RunGame() {
    MSG msg = {};
    float startTime = (float)GetTickCount64() / 1000.0f;
    while (msg.message != WM_QUIT) {
        float currentTime = (float)GetTickCount64() / 1000.0f;
        float elapsedTime = currentTime - startTime;
        startTime = currentTime;
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        RenderFrame(elapsedTime);
    }
}

// Cleanup
void Cleanup() {
    if (textureChar1) textureChar1->Release();
    if (textureChar2) textureChar2->Release();
    if (textureChar3) textureChar3->Release();
    if (renderTargetView) renderTargetView->Release();
    if (swapChain) swapChain->Release();
    if (deviceContext) deviceContext->Release();
    if (device) device->Release();
}

// Main
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { CS_HREDRAW | CS_VREDRAW, WindowProc, 0, 0, hInstance, NULL, LoadCursor(NULL, IDC_ARROW), NULL, NULL, L"DX11Class" };
    RegisterClass(&wc);
    hwnd = CreateWindow(L"DX11Class", L"DirectX 11 Sprite Example", WS_OVERLAPPEDWINDOW, 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);
    if (!InitD3D(hwnd)) return 0;
    RunGame();
    Cleanup();
    return 0;
}
