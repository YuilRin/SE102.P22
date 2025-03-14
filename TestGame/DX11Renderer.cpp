#include "DX11Renderer.h"
#include <wrl.h>
#include <vector>
#include <wincodec.h>
#include <d2d1_1.h>

using namespace Microsoft::WRL;

ID2D1DeviceContext* g_d2dDeviceContext = nullptr;
/*

void DX11Renderer::DrawTexture(ID3D11ShaderResourceView* texture, float x, float y, float width, float height) {
    if (!texture || !context) return;

    context->PSSetShaderResources(0, 1, &texture);
    // C?n Shader + Vertex Buffer ?? v? hình ?nh, thêm debug xem có g?i t?i ?ây không
   // MessageBox(NULL, L"DrawTexture called!", L"Debug", MB_OK);
}
void DX11Renderer::TestDrawRectangle() {
    if (!context) return;

    struct Vertex {
        float x, y, z;
    };

    Vertex vertices[] = {
        { 0.0f,  0.5f, 0.0f },
        { 0.5f, -0.5f, 0.0f },
        { -0.5f, -0.5f, 0.0f }
    };

    ID3D11Buffer* vertexBuffer;
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(vertices);
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;

    HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &vertexBuffer);
    if (FAILED(hr)) {
        MessageBox(NULL, L"Failed to create vertex buffer!", L"Error", MB_OK);
        return;
    }

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    context->Draw(3, 0);

    vertexBuffer->Release();
}
*/

void DX11Renderer::DrawTexture(ID3D11ShaderResourceView* texture, float x, float y, float width, float height) {
    if (!texture || !g_d2dDeviceContext) return;
    if (!texture) {
        MessageBox(NULL, L"Texture is null!", L"Error", MB_OK);
    }

    if (!g_d2dDeviceContext) {
        MessageBox(NULL, L"g_d2dDeviceContext is null!", L"Error", MB_OK);
    }

    ComPtr<IDXGISurface> dxgiSurface;
    ComPtr<ID2D1Bitmap1> d2dBitmap;

    if (!d2dBitmap) {
        MessageBox(NULL, L"d2dBitmap is NULL!", L"Error", MB_OK);
        return;
    }

    HRESULT hr = texture->QueryInterface(__uuidof(IDXGISurface), reinterpret_cast<void**>(dxgiSurface.GetAddressOf()));
    if (SUCCEEDED(hr)) {
        hr = g_d2dDeviceContext->CreateBitmapFromDxgiSurface(dxgiSurface.Get(), nullptr, d2dBitmap.GetAddressOf());
    }

    if (SUCCEEDED(hr)) {
        D2D1_RECT_F destRect = { x, y, x + width, y + height };
        g_d2dDeviceContext->DrawBitmap(d2dBitmap.Get(), &destRect);
    }
}


ID3D11ShaderResourceView* DX11Renderer::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* context, const std::wstring& filePath) {
    ComPtr<IWICImagingFactory> wicFactory;
    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));
    if (FAILED(hr)) return nullptr;

    ComPtr<IWICBitmapDecoder> decoder;
    hr = wicFactory->CreateDecoderFromFilename(filePath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
    if (FAILED(hr)) return nullptr;

    ComPtr<IWICBitmapFrameDecode> frame;
    hr = decoder->GetFrame(0, &frame);
    if (FAILED(hr)) return nullptr;

    ComPtr<IWICFormatConverter> converter;
    hr = wicFactory->CreateFormatConverter(&converter);
    if (FAILED(hr)) return nullptr;

    hr = converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);
    if (FAILED(hr)) return nullptr;

    UINT width, height;
    frame->GetSize(&width, &height);
    std::vector<BYTE> buffer(width * height * 4);
    hr = converter->CopyPixels(nullptr, width * 4, buffer.size(), buffer.data());
    if (FAILED(hr)) return nullptr;

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = buffer.data();
    initData.SysMemPitch = width * 4;

    ComPtr<ID3D11Texture2D> texture;
    hr = device->CreateTexture2D(&desc, &initData, &texture);
    if (FAILED(hr)) return nullptr;

    ComPtr<ID3D11ShaderResourceView> textureView;
    hr = device->CreateShaderResourceView(texture.Get(), nullptr, textureView.GetAddressOf());
    if (FAILED(hr)) return nullptr;
    if (!texture) {
        MessageBox(NULL, L"Failed to load player texture!", L"Error", MB_OK);
    }

    return textureView.Detach();
}



void DX11Renderer::Cleanup() {
    if (playerBitmap) { playerBitmap->Release(); playerBitmap = nullptr; }
    if (enemyBitmap) { enemyBitmap->Release(); enemyBitmap = nullptr; }
    if (d2dRenderTarget) { d2dRenderTarget->Release(); d2dRenderTarget = nullptr; }
    if (d2dFactory) { d2dFactory->Release(); d2dFactory = nullptr; }
    if (wicFactory) { wicFactory->Release(); wicFactory = nullptr; }
    if (swapChain) { swapChain->Release(); swapChain = nullptr; }
    if (renderTargetView) { renderTargetView->Release(); renderTargetView = nullptr; }
    if (context) { context->Release(); context = nullptr; }
    if (device) { device->Release(); device = nullptr; }
}

void DX11Renderer::BeginDraw() {
	d2dRenderTarget->BeginDraw();
}
void DX11Renderer::ClearScreen() {
    float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
    context->ClearRenderTargetView(renderTargetView, color);
}

bool DX11Renderer::Init(HWND hWnd) {
    // ? B?t c? h? tr? BGRA ?? ??m b?o t??ng thích v?i Direct2D
    UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;


    // ? C?u hình Swap Chain
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferDesc.Width = 800;
    scd.BufferDesc.Height = 600;
    scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // ?? ??i t? R8G8B8A8 sang B8G8R8A8 ?? h? tr? Direct2D
    scd.SampleDesc.Count = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.BufferCount = 1;
    scd.OutputWindow = hWnd;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    // ? T?o D3D11 Device + Swap Chain
    if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, nullptr, 0,
        D3D11_SDK_VERSION, &scd, &swapChain, &device, nullptr, &context))) {
        MessageBox(NULL, L"Failed to create D3D11 device and swap chain!", L"Error", MB_OK);
        return false;
    }

    // ? L?y BackBuffer và t?o Render Target View
    ID3D11Texture2D* backBuffer = nullptr;
    if (FAILED(swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)))) {
        MessageBox(NULL, L"Failed to get back buffer!", L"Error", MB_OK);
        return false;
    }
    if (FAILED(device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView))) {
        MessageBox(NULL, L"Failed to create render target view!", L"Error", MB_OK);
        backBuffer->Release();
        return false;
    }
    if (!renderTargetView) {
        MessageBox(NULL, L"renderTargetView is NULL!", L"Error", MB_OK);
        return false;
    }

    context->OMSetRenderTargets(1, &renderTargetView, nullptr);
    if (!renderTargetView) {
        MessageBox(NULL, L"RenderTargetView is NULL after setting!", L"Error", MB_OK);
    }
    backBuffer->Release();

 

    // ? Kh?i t?o Direct2D Factory
    if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory))) {
        MessageBox(NULL, L"Failed to create D2D1 Factory!", L"Error", MB_OK);
        return false;
    }

    // ? Kh?i t?o COM và WIC Factory
    if (FAILED(CoInitialize(nullptr))) {
        MessageBox(NULL, L"Failed to initialize COM!", L"Error", MB_OK);
        return false;
    }
    if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory)))) {
        MessageBox(NULL, L"Failed to create WIC Imaging Factory!", L"Error", MB_OK);
        return false;
    }

    // ? L?y Surface t? SwapChain ?? t?o Direct2D Render Target
    IDXGISurface* dxgiSurface = nullptr;
    if (FAILED(swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiSurface)))) {
        MessageBox(NULL, L"Failed to get DXGI surface!", L"Error", MB_OK);
        return false;
    }

    // ? ??nh d?ng h? tr? Direct2D (B8G8R8A8 + Premultiplied Alpha)
    D2D1_RENDER_TARGET_PROPERTIES props =
        D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
        );

    // ? T?o Direct2D Render Target t? DXGI Surface
    HRESULT hr = d2dFactory->CreateDxgiSurfaceRenderTarget(dxgiSurface, &props, &d2dRenderTarget);
    dxgiSurface->Release(); // Gi?i phóng surface sau khi dùng

    if (FAILED(hr)) {
        MessageBox(NULL, L"Failed to create Direct2D render target!", L"Error", MB_OK);
        return false;
    }

    // ? Ki?m tra l?i d2dRenderTarget
    if (!d2dRenderTarget) {
        MessageBox(NULL, L"d2dRenderTarget is null!", L"Error", MB_OK);
        return false;
    }
    if (!device) {
        MessageBox(NULL, L"device is NULL!", L"Error", MB_OK);
        return false;
    }
    if (!context) {
        MessageBox(NULL, L"context is NULL!", L"Error", MB_OK);
        return false;
    }


    // ? Load hình ?nh
    playerBitmap = LoadBitmapFromFile(L"player.png");
    enemyBitmap = LoadBitmapFromFile(L"enemy.png");


    if (!playerBitmap || !enemyBitmap) {
        MessageBox(NULL, L"Failed to load images!", L"Error", MB_OK);
        return false;
    }

    return true;
}


void DX11Renderer::Present() {
    HRESULT hr = swapChain->Present(1, 0);
    if (FAILED(hr)) {
        MessageBox(NULL, L"Present failed!", L"Error", MB_OK);
    }

}

ID2D1Bitmap* DX11Renderer::LoadBitmapFromFile(const wchar_t* filename) {
    IWICBitmapDecoder* decoder = nullptr;
    if (FAILED(wicFactory->CreateDecoderFromFilename(filename, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder))) {
        return nullptr;
    }

    IWICBitmapFrameDecode* frame = nullptr;
    decoder->GetFrame(0, &frame);

    IWICFormatConverter* converter = nullptr;
    wicFactory->CreateFormatConverter(&converter);
    converter->Initialize(frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0, WICBitmapPaletteTypeCustom);

    ID2D1Bitmap* bitmap = nullptr;
    d2dRenderTarget->CreateBitmapFromWicBitmap(converter, nullptr, &bitmap);

    decoder->Release();
    frame->Release();
    converter->Release();

    return bitmap;
}