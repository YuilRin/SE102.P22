#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "SpriteBatch.h"
#include "WICTextureLoader.h"

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
ID3D11ShaderResourceView* currentTexture = nullptr;

// Vị trí nhân vật
float charX = 300.0f, charY = 400.0f;
bool isJumping = false;
float jumpStartTime = 0.0f;

// Xử lý phím
void HandleKeyPress(WPARAM key) {
    switch (key) {
    case 'A': // Chuyển sang char2
        currentTexture = textureChar2;
        MessageBox(hwnd, L"Switched to char2", L"Info", MB_OK);
        break;
    case 'S': // Chuyển về char1
        currentTexture = textureChar1;
        MessageBox(hwnd, L"Switched to char1", L"Info", MB_OK);
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

    // Render Target View
    ID3D11Texture2D* backBuffer;
    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
    backBuffer->Release();
    deviceContext->OMSetRenderTargets(1, &renderTargetView, NULL);

    // Viewport
    D3D11_VIEWPORT viewport = { 0.0f, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, 1.0f };
    deviceContext->RSSetViewports(1, &viewport);

    // SpriteBatch
    spriteBatch = std::make_unique<SpriteBatch>(deviceContext);

    // Load textures
    if (FAILED(CreateWICTextureFromFile(device, L"char1.png", nullptr, &textureChar1))) {
        MessageBox(hwnd, L"Failed to load char1.png", L"Error", MB_OK);
        return false;
    }
    if (FAILED(CreateWICTextureFromFile(device, L"char2.png", nullptr, &textureChar2))) {
        MessageBox(hwnd, L"Failed to load char2.png", L"Error", MB_OK);
        return false;
    }

    currentTexture = textureChar1;
    return true;
}

// Vẽ màn hình
void RenderFrame(float elapsedTime) {
    // Xóa màn hình
    float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    deviceContext->ClearRenderTargetView(renderTargetView, clearColor);

    // Xử lý nhảy
    if (isJumping) {
        if (elapsedTime - jumpStartTime < 1.0f) {
            charY = 300.0f; // Nhảy lên
        }
        else {
            charY = 400.0f; // Rơi xuống
            isJumping = false;
        }
    }

    // Vẽ nhân vật
    spriteBatch->Begin();
    spriteBatch->Draw(currentTexture, XMFLOAT2(charX, charY));
    spriteBatch->End();

    // Hiển thị frame
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
