#include "stdafx.h"
#include "d3d11graphics.h"
#include "color.h"
#include "win_util.h"

namespace procdraw {

    D3D11Graphics::D3D11Graphics(HWND hWnd) :
        hWnd_(hWnd),
        d3dFeatureLevel_(D3D_FEATURE_LEVEL_10_0)
    {
        InitD3D();
    }

    D3D11Graphics::~D3D11Graphics()
    {
        immediateContext_->ClearState();
    }

    void D3D11Graphics::Background(float h, float s, float v)
    {
        float r, g, b;
        Hsv2Rgb(h, s, v, r, g, b);
        FLOAT c[4] = { r, g, b, 1.0f };
        immediateContext_->ClearRenderTargetView(renderTargetView_, c);
    }

    void D3D11Graphics::Present()
    {
        swapChain_->Present(0, 0);
    }

    void D3D11Graphics::InitD3D()
    {
        RECT rc;
        GetClientRect(hWnd_, &rc);
        UINT width = rc.right - rc.left;
        UINT height = rc.bottom - rc.top;

        // TODO is D3D11_CREATE_DEVICE_BGRA_SUPPORT what I want here?
        UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        D3D_DRIVER_TYPE driver_type = D3D_DRIVER_TYPE_HARDWARE;

        D3D_FEATURE_LEVEL feature_levels[] = {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
        };
        UINT num_feature_levels = ARRAYSIZE(feature_levels);

        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 1;
        sd.BufferDesc.Width = width;
        sd.BufferDesc.Height = height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hWnd_;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;

        HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, driver_type, nullptr,
            createDeviceFlags, feature_levels, num_feature_levels,
            D3D11_SDK_VERSION, &sd, &swapChain_, &d3dDevice_,
            &d3dFeatureLevel_, &immediateContext_);
        if (hr == E_INVALIDARG) {
            // Try again without D3D_FEATURE_LEVEL_11_1
            hr = D3D11CreateDeviceAndSwapChain(nullptr, driver_type, nullptr,
                createDeviceFlags, &feature_levels[1], num_feature_levels - 1,
                D3D11_SDK_VERSION, &sd, &swapChain_, &d3dDevice_,
                &d3dFeatureLevel_, &immediateContext_);
        }
        ThrowOnFail(hr);

        ThrowOnFail(swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D),
            reinterpret_cast<void**>(&backBuffer_)));

        ThrowOnFail(d3dDevice_->CreateRenderTargetView(backBuffer_, nullptr, &renderTargetView_));

        immediateContext_->OMSetRenderTargets(1, &renderTargetView_.GetInterfacePtr(), nullptr);

        // Set up the viewport
        D3D11_VIEWPORT vp;
        vp.Width = (FLOAT)width;
        vp.Height = (FLOAT)height;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        immediateContext_->RSSetViewports(1, &vp);
    }

}
