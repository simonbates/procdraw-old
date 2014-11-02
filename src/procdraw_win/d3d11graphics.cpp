#include "stdafx.h"
#include "d3d11graphics.h"
#include "color.h"
#include "win_util.h"
#include <d3dcompiler.h>

namespace procdraw {

    D3D11Graphics::D3D11Graphics(HWND hWnd) :
        hWnd_(hWnd),
        d3dFeatureLevel_(D3D_FEATURE_LEVEL_10_0)
    {
        InitD3D();
        CreateVertexShader();
        CreatePixelShader();
        CreateTriangleVertexBuffer();
    }

    D3D11Graphics::~D3D11Graphics()
    {
        d3dContext_->ClearState();
    }

    void D3D11Graphics::Background(float h, float s, float v)
    {
        float r, g, b;
        Hsv2Rgb(h, s, v, r, g, b);
        FLOAT c[4] = { r, g, b, 1.0f };
        d3dContext_->ClearRenderTargetView(renderTargetView_, c);
    }

    void D3D11Graphics::Present()
    {
        swapChain_->Present(0, 0);
    }

    void D3D11Graphics::Triangle()
    {
        d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        UINT stride = sizeof(ShaderVertex);
        UINT offset = 0;
        d3dContext_->IASetVertexBuffers(0, 1, &triangleVertexBuffer_.GetInterfacePtr(), &stride, &offset);

        d3dContext_->Draw(3, 0);
    }

    void D3D11Graphics::InitD3D()
    {
        // Device, immediate context, and swap chain

        RECT rc;
        GetClientRect(hWnd_, &rc);
        UINT width = rc.right - rc.left;
        UINT height = rc.bottom - rc.top;

        UINT createDeviceFlags = 0;
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
            &d3dFeatureLevel_, &d3dContext_);
        if (hr == E_INVALIDARG) {
            // Try again without D3D_FEATURE_LEVEL_11_1
            hr = D3D11CreateDeviceAndSwapChain(nullptr, driver_type, nullptr,
                createDeviceFlags, &feature_levels[1], num_feature_levels - 1,
                D3D11_SDK_VERSION, &sd, &swapChain_, &d3dDevice_,
                &d3dFeatureLevel_, &d3dContext_);
        }
        ThrowOnFail(hr);

        // Render Target View

        ThrowOnFail(swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D),
            reinterpret_cast<void**>(&backBuffer_)));

        ThrowOnFail(d3dDevice_->CreateRenderTargetView(backBuffer_, nullptr, &renderTargetView_));

        d3dContext_->OMSetRenderTargets(1, &renderTargetView_.GetInterfacePtr(), nullptr);

        // Viewport

        D3D11_VIEWPORT vp;
        vp.Width = (FLOAT)width;
        vp.Height = (FLOAT)height;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        d3dContext_->RSSetViewports(1, &vp);
    }

    ID3D10BlobPtr D3D11Graphics::CompileShaderFromFile(_In_ LPCWSTR pFileName,
        _In_ LPCSTR pEntrypoint, _In_ LPCSTR pTarget)
    {
        ID3D10BlobPtr compiledShader;
        ID3D10BlobPtr errorMessages;
        HRESULT hr = D3DCompileFromFile(pFileName, nullptr, nullptr,
            pEntrypoint, pTarget, 0, 0, &compiledShader, &errorMessages);
        if (errorMessages != nullptr) {
            throw std::runtime_error(static_cast<char*>(errorMessages->GetBufferPointer()));
        }
        ThrowOnFail(hr);
        return compiledShader;
    }

    void D3D11Graphics::CreateVertexShader()
    {
        // Create shader

        ID3D10BlobPtr vs = CompileShaderFromFile(L"shaders\\shaders1.hlsl", "vertex_shader", "vs_5_0");
        ThrowOnFail(d3dDevice_->CreateVertexShader(vs->GetBufferPointer(),
            vs->GetBufferSize(), nullptr, &vertexShader_));
        d3dContext_->VSSetShader(vertexShader_, 0, 0);

        // Input layout

        D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        ThrowOnFail(d3dDevice_->CreateInputLayout(inputElementDescriptions,
            ARRAYSIZE(inputElementDescriptions), vs->GetBufferPointer(),
            vs->GetBufferSize(), &inputLayout_));
        d3dContext_->IASetInputLayout(inputLayout_);
    }

    void D3D11Graphics::CreatePixelShader()
    {
        ID3D10BlobPtr ps = CompileShaderFromFile(L"shaders\\shaders1.hlsl", "pixel_shader", "ps_5_0");
        ThrowOnFail(d3dDevice_->CreatePixelShader(ps->GetBufferPointer(),
            ps->GetBufferSize(), nullptr, &pixelShader_));
        d3dContext_->PSSetShader(pixelShader_, 0, 0);
    }

    void D3D11Graphics::CreateTriangleVertexBuffer()
    {
        ShaderVertex vertices[] =
        {
            ShaderVertex(DirectX::XMFLOAT4(-0.8f, 0.8f, 0.0f, 1.0f),
                DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)),
            ShaderVertex(DirectX::XMFLOAT4(0.8f, -0.8f, 0.0f, 1.0f),
                DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)),
            ShaderVertex(DirectX::XMFLOAT4(-0.8f, -0.8f, 0.0f, 1.0f),
                DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f))
        };

        D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
        vertexBufferDesc.ByteWidth = sizeof(ShaderVertex) * ARRAYSIZE(vertices);
        vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA vertexSubresourceData;
        ZeroMemory(&vertexSubresourceData, sizeof(vertexSubresourceData));
        vertexSubresourceData.pSysMem = vertices;

        ThrowOnFail(d3dDevice_->CreateBuffer(&vertexBufferDesc,
            &vertexSubresourceData, &triangleVertexBuffer_));
    }

}
