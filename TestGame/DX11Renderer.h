#pragma once
#include <d3d11.h>
#include <d2d1.h>
#include <dwrite.h>
#include <string>
#include <wincodec.h> // WIC (Windows Imaging Component) ?? load ?nh

class DX11Renderer {
public:
    void TestDrawRectangle();


    void DrawTexture(ID3D11ShaderResourceView* texture, float x, float y, float width, float height);

    static ID3D11ShaderResourceView* LoadTexture(ID3D11Device* device, ID3D11DeviceContext* context, const std::wstring& filePath);

    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;
    IDXGISwapChain* swapChain = nullptr;
    ID3D11RenderTargetView* renderTargetView = nullptr;

    ID2D1Factory* d2dFactory = nullptr;
    ID2D1RenderTarget* d2dRenderTarget = nullptr;
    IWICImagingFactory* wicFactory = nullptr;  // WIC Factory ?? load ?nh

    ID2D1Bitmap* playerBitmap = nullptr;
    ID2D1Bitmap* enemyBitmap = nullptr;

    bool Init(HWND hWnd);
    void ClearScreen();
    void BeginDraw();
    void EndDraw();
    void DrawBitmap(ID2D1Bitmap* bitmap, float x, float y, float width, float height);
    ID2D1Bitmap* LoadBitmapFromFile(const wchar_t* filename);
    void Present();
    void Cleanup();
};
