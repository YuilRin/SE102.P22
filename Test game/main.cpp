#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "SpriteBatch.h"
#include "WICTextureLoader.h"

#pragma comment(lib, "d3d11.lib")

using namespace DirectX;

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
ID3D11ShaderResourceView* currentTexture = nullptr;

float charX = 300.0f, charY = 400.0f;
bool isJumping = false;
float jumpStartTime = 0.0f;

void HandleKeyPress(WPARAM key) {
    switch (key) {
    case 'A': // Chuyển sang char2 và di chuyển sang trái
        currentTexture = textureChar2;
        charX -= 10.0f;
        break;
    case 'S': // Chuyển về char1
        currentTexture = textureChar1;
        break;
    case 'D': // Di chuyển sang phải
        charX += 10.0f;
        break;
    case 'W': // Nhảy
        if (!isJumping) {
            isJumping = true;
            jumpStartTime = (float)GetTickCount64() / 1000.0f;
        }
        break;
    }
}

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

    CreateWICTextureFromFile(device, L"char1.png", nullptr, &textureChar1);
    CreateWICTextureFromFile(device, L"char2.png", nullptr, &textureChar2);

    currentTexture = textureChar1;
    return true;
}

void RenderFrame(float elapsedTime) {
    float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    deviceContext->ClearRenderTargetView(renderTargetView, clearColor);

    if (isJumping) {
        float jumpTimeElapsed = elapsedTime - jumpStartTime;
        if (jumpTimeElapsed < 0.5f) {
            charY = 300.0f;
        }
        else if (jumpTimeElapsed < 1.0f) {
            charY = 350.0f;
        }
        else {
            charY = 400.0f;
            isJumping = false;
        }
    }

    spriteBatch->Begin();
    spriteBatch->Draw(currentTexture, XMFLOAT2(charX, charY));
    spriteBatch->End();

    swapChain->Present(0, 0);
}

void RunGame() {
    MSG msg = {};
    float startTime = (float)GetTickCount64() / 1000.0f;

    while (msg.message != WM_QUIT) {
        float currentTime = (float)GetTickCount64() / 1000.0f;
        float elapsedTime = currentTime;

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        RenderFrame(elapsedTime);
    }
}

void Cleanup() {
    if (textureChar1) textureChar1->Release();
    if (textureChar2) textureChar2->Release();
    if (renderTargetView) renderTargetView->Release();
    if (swapChain) swapChain->Release();
    if (deviceContext) deviceContext->Release();
    if (device) device->Release();
}

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
