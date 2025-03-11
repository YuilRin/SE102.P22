#include <vector>
#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "SpriteBatch.h"
#include "WICTextureLoader.h"
#include <cstdlib>
#include <ctime>

#pragma comment(lib, "d3d11.lib")

using namespace DirectX;


struct Fireball {
    float x, y;
    int direction;
};

struct boss
{
	float x, y;
	float speed;
	float lastMoveTime;
	int dirX, dirY;
};

std::vector<boss> Boss;

// Cấu hình cửa sổ
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
HWND hwnd = NULL;
ID3D11Device* device = nullptr;
ID3D11DeviceContext* deviceContext = nullptr;
IDXGISwapChain* swapChain = nullptr;
ID3D11RenderTargetView* renderTargetView = nullptr;

std::unique_ptr<SpriteBatch> spriteBatch;

ID3D11ShaderResourceView* textureChar = nullptr;
ID3D11ShaderResourceView* textureFire = nullptr;
ID3D11ShaderResourceView* textureBoss = nullptr;

std::vector<Fireball> fireballs;

// Vị trí nhân vật
float charX = 300.0f, charY = 100.0f;
const float charSpeed = 10.0f;
int FireDirection = 0;


// Xử lý phím
void HandleKeyPress(WPARAM key) {
    switch (key) {
    case 'A': case VK_LEFT:
        if(charX>=10)
        {
            charX -= charSpeed;
            FireDirection = 0;
		}
        break;
    case 'D': case VK_RIGHT:
        if (charX <=SCREEN_WIDTH-30)
        {
            charX += charSpeed;
			FireDirection = 1;
		}
        break;
    case 'W': case VK_UP:
        if (charY >= 10)
        {       
            charY -= charSpeed;
		    FireDirection = 2;
		}
        break;
    case 'S': case VK_DOWN:
        if (charY <= SCREEN_HEIGHT - 50)
        {
            charY += charSpeed;
            FireDirection = 3;
        }
        break;
    case 'J':
        {
        fireballs.push_back({ charX, charY, FireDirection });

		    break;
        }
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

    if (FAILED(CreateWICTextureFromFile(device, L"char1.png", nullptr, &textureChar))) return false;
    if (FAILED(CreateWICTextureFromFile(device, L"fire.png", nullptr, &textureFire))) return false;
    if (FAILED(CreateWICTextureFromFile(device, L"char3.png", nullptr, &textureBoss))) return false;

    int numBoss = 5; // Số lượng boss
    for (int i = 0; i < numBoss; i++) {
        int dirX = rand() % 3 - 1;
        int dirY = rand() % 3 - 1;
        // Nếu cả hai bằng 0, đặt lại giá trị
        if (dirX == 0 && dirY == 0) {
            dirX = (rand() % 2 == 0) ? 1 : -1; // Chọn ngẫu nhiên -1 hoặc 1
        }

        Boss.push_back({
            (float)(rand() % SCREEN_WIDTH),  // Vị trí ngẫu nhiên X
            (float)(rand() % SCREEN_HEIGHT), // Vị trí ngẫu nhiên Y
            50.0f,                           // Tốc độ di chuyển


            (float)GetTickCount(),           // Lần di chuyển cuối
			dirX, dirY                       // Hướng di chuyển

            });
    }
    srand((unsigned int)time(NULL));
    return true;
}

// Vẽ màn hình
void RenderFrame(float elapsedTime) {
    float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    deviceContext->ClearRenderTargetView(renderTargetView, clearColor);

    // Boss move
    for (auto& boss : Boss) {
        // Cập nhật vị trí
        boss.x += boss.speed * boss.dirX * elapsedTime;
        boss.y += boss.speed * boss.dirY * elapsedTime;


        // Kiểm tra va chạm với biên màn hình
        if (boss.x <= 0) {
            boss.x = 0;  // Đảm bảo không vượt ra ngoài màn hình
            boss.dirX = 1; // Đổi hướng sang phải
        }
        else if (boss.x >= SCREEN_WIDTH - 20) {
            boss.x = SCREEN_WIDTH - 20;
            boss.dirX = -1; // Đổi hướng sang trái
        }

        if (boss.y <= 0) {
            boss.y = 0;
            boss.dirY = 1; // Đổi hướng xuống dưới
        }
        else if (boss.y >= SCREEN_HEIGHT - 20) { // Cần đổi SCREEN_WIDTH -> SCREEN_HEIGHT
            boss.y = SCREEN_HEIGHT - 20;
            boss.dirY = -1; // Đổi hướng lên trên
        }

        // Kiểm tra va chạm với nhân vật
        if (abs(charX - boss.x) < 20 && abs(charY - boss.y) < 20) {
            MessageBox(hwnd, L"Game Over!", L"Alert", MB_OK);
            PostQuitMessage(0);
        }
        if (boss.dirX == 0 && boss.dirY == 0) {
            boss.dirX = (rand() % 2 == 0) ? 1 : -1;
            boss.dirY = (rand() % 2 == 0) ? 1 : -1;
        }
    }

    for (auto it = fireballs.begin(); it != fireballs.end(); ) {
        switch (it->direction) 
        {
        case 0: it->x -= 0.5f; break;
        case 1: it->x += 0.5f; break;
        case 2: it->y -= 0.5f; break;
        case 3: it->y += 0.5f; break;
        }
       
            if (it->x < 0 || it->x > SCREEN_WIDTH || it->y < 0 || it->y > SCREEN_HEIGHT)
            {

                it = fireballs.erase(it);
            }
            else {
                ++it;
            }
      
    }
   
    spriteBatch->Begin();
    spriteBatch->Draw(textureChar, XMFLOAT2(charX, charY));
    for (const auto& fireball : fireballs) {
        spriteBatch->Draw(textureFire, XMFLOAT2(fireball.x, fireball.y));
    }
	for (const auto& boss : Boss) {
		spriteBatch->Draw(textureBoss, XMFLOAT2(boss.x, boss.y));
	}
    
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
    if (textureChar) textureChar->Release();
    if (textureFire) textureFire->Release();
	if (textureBoss) textureBoss->Release();
    if (renderTargetView) renderTargetView->Release();
    if (swapChain) swapChain->Release();
    if (deviceContext) deviceContext->Release();
    if (device) device->Release();
}

// Hiển thị màn hình chờ trước khi vào game
void ShowStartScreen(HWND hwnd) {
    MSG msg = {};
    HDC hdc = GetDC(hwnd);
    SetTextColor(hdc, RGB(255, 255, 255)); 
    SetBkMode(hdc, TRANSPARENT); 

    bool keyPressed = false;

    while (!keyPressed) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) exit(0);
            if (msg.message == WM_KEYDOWN) keyPressed = true;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Xóa màn hình bằng màu đen
        float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
        deviceContext->ClearRenderTargetView(renderTargetView, clearColor);

        // Dùng GDI để vẽ chữ lên của cửa sổ
        RECT textRect = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT };
        DrawText(hdc, L"Press any key to begin", -1, &textRect, DT_NOCLIP | DT_SINGLELINE);

        swapChain->Present(1, 0);
    }

    ReleaseDC(hwnd, hdc);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { CS_HREDRAW | CS_VREDRAW, WindowProc, 0, 0, hInstance, NULL, LoadCursor(NULL, IDC_ARROW), NULL, NULL, L"DX11Class" };
    RegisterClass(&wc);
    hwnd = CreateWindow(L"DX11Class", L"DirectX 11 Sprite Example", WS_OVERLAPPEDWINDOW, 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);
    
    if (!InitD3D(hwnd)) return 0;
    
    ShowStartScreen(hwnd);
    
    RunGame();
    Cleanup();
    return 0;
}
