/* Copyright (c) 2014, 2015, 2018 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// clang-format off
#include "stdafx.h"
#define _USE_MATH_DEFINES
#include "D3D11Graphics.h"
// clang-format on
#include "Color.h"
#include "WinUtils.h"
#include <cmath>
#include <d3dcompiler.h>

using Microsoft::WRL::ComPtr;

namespace Procdraw {

D3D11Graphics::D3D11Graphics(HWND hWnd)
    : hWnd_(hWnd),
      d3dFeatureLevel_(D3D_FEATURE_LEVEL_10_0)
{
    InitD3D();
    CreateVertexShader();
    CreatePixelShader();

    // Constant Buffer
    DirectX::XMStoreFloat4x4(&cbData_.WorldViewProjection, DirectX::XMMatrixIdentity());
    CreateConstantBuffer();

    CreateTetrahedronVertexBuffer();
    CreateCubeBuffers();

    InitViewProjectionMatrix();
    ResetMatrix();
    InitLighting();
    InitMaterial();
}

D3D11Graphics::~D3D11Graphics()
{
    d3dContext_->ClearState();
}

void D3D11Graphics::Background(float h, float s, float v)
{
    float r, g, b;
    std::tie(r, g, b) = Hsv2rgb(h, s, v);
    FLOAT c[4] = {r, g, b, 1.0f};
    d3dContext_->ClearDepthStencilView(depthStencilView_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    d3dContext_->ClearRenderTargetView(renderTargetView_.Get(), c);
    ResetMatrix();
}

void D3D11Graphics::LightColor(float h, float s, float v)
{
    float r, g, b;
    std::tie(r, g, b) = Hsv2rgb(h, s, v);
    lightColor_.x = r;
    lightColor_.y = g;
    lightColor_.z = b;
    lightColor_.w = 0.0f;
}

void D3D11Graphics::AmbientLightColor(float h, float s, float v)
{
    float r, g, b;
    std::tie(r, g, b) = Hsv2rgb(h, s, v);
    ambientLightColor_.x = r;
    ambientLightColor_.y = g;
    ambientLightColor_.z = b;
    ambientLightColor_.w = 0.0f;
}

void D3D11Graphics::Color(float h, float s, float v)
{
    float r, g, b;
    std::tie(r, g, b) = Hsv2rgb(h, s, v);
    materialColor_.x = r;
    materialColor_.y = g;
    materialColor_.z = b;
    materialColor_.w = 1.0f;
}

void D3D11Graphics::Present()
{
    UINT SyncInterval = 1;
    swapChain_->Present(SyncInterval, 0);
}

void D3D11Graphics::Tetrahedron()
{
    UpdateConstantBufferForObject();

    d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    UINT stride = sizeof(ShaderVertex);
    UINT offset = 0;
    d3dContext_->IASetVertexBuffers(0, 1, tetrahedronVertexBuffer_.GetAddressOf(), &stride, &offset);

    d3dContext_->Draw(12, 0);
}

void D3D11Graphics::Cube()
{
    UpdateConstantBufferForObject();

    d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    UINT stride = sizeof(ShaderVertex);
    UINT offset = 0;
    d3dContext_->IASetVertexBuffers(0, 1, cubeVertexBuffer_.GetAddressOf(), &stride, &offset);
    d3dContext_->IASetIndexBuffer(cubeIndexBuffer_.Get(), DXGI_FORMAT_R32_UINT, 0);

    d3dContext_->DrawIndexed(36, 0, 0);
}

void D3D11Graphics::RotateX(float turns)
{
    auto rotationMatrix = DirectX::XMMatrixRotationX(turns * 2 * M_PI);
    auto worldMatrix = DirectX::XMLoadFloat4x4(&worldMatrix_);
    DirectX::XMStoreFloat4x4(&worldMatrix_, rotationMatrix * worldMatrix);
}

void D3D11Graphics::RotateY(float turns)
{
    auto rotationMatrix = DirectX::XMMatrixRotationY(turns * 2 * M_PI);
    auto worldMatrix = DirectX::XMLoadFloat4x4(&worldMatrix_);
    DirectX::XMStoreFloat4x4(&worldMatrix_, rotationMatrix * worldMatrix);
}

void D3D11Graphics::RotateZ(float turns)
{
    auto rotationMatrix = DirectX::XMMatrixRotationZ(turns * 2 * M_PI);
    auto worldMatrix = DirectX::XMLoadFloat4x4(&worldMatrix_);
    DirectX::XMStoreFloat4x4(&worldMatrix_, rotationMatrix * worldMatrix);
}

void D3D11Graphics::Translate(float x, float y, float z)
{
    auto translationMatrix = DirectX::XMMatrixTranslation(x, y, z);
    auto worldMatrix = DirectX::XMLoadFloat4x4(&worldMatrix_);
    DirectX::XMStoreFloat4x4(&worldMatrix_, translationMatrix * worldMatrix);
}

void D3D11Graphics::Scale(float x, float y, float z)
{
    auto scalingMatrix = DirectX::XMMatrixScaling(x, y, z);
    auto worldMatrix = DirectX::XMLoadFloat4x4(&worldMatrix_);
    DirectX::XMStoreFloat4x4(&worldMatrix_, scalingMatrix * worldMatrix);
}

void D3D11Graphics::InitD3D()
{
    // Device, context, and swap chain

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
    // TODO: RefreshRate is ignored in windowed mode
    // TODO: and in full-screen mode, it must be set to a rate supported by the display
    // TODO: For full-screen mode, get the available display refresh rates using IDXGIOutput::GetDisplayModeList
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd_;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr,
                                               driver_type,
                                               nullptr,
                                               createDeviceFlags,
                                               feature_levels,
                                               num_feature_levels,
                                               D3D11_SDK_VERSION,
                                               &sd,
                                               swapChain_.ReleaseAndGetAddressOf(),
                                               d3dDevice_.ReleaseAndGetAddressOf(),
                                               &d3dFeatureLevel_,
                                               d3dContext_.ReleaseAndGetAddressOf());
    if (hr == E_INVALIDARG) {
        // Try again without D3D_FEATURE_LEVEL_11_1
        hr = D3D11CreateDeviceAndSwapChain(nullptr,
                                           driver_type,
                                           nullptr,
                                           createDeviceFlags,
                                           &feature_levels[1],
                                           num_feature_levels - 1,
                                           D3D11_SDK_VERSION,
                                           &sd,
                                           swapChain_.ReleaseAndGetAddressOf(),
                                           d3dDevice_.ReleaseAndGetAddressOf(),
                                           &d3dFeatureLevel_,
                                           d3dContext_.ReleaseAndGetAddressOf());
    }
    ThrowOnFail(hr);

    // Render Target View

    ThrowOnFail(swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D),
                                      reinterpret_cast<void**>(backBuffer_.ReleaseAndGetAddressOf())));

    ThrowOnFail(d3dDevice_->CreateRenderTargetView(backBuffer_.Get(), nullptr, renderTargetView_.ReleaseAndGetAddressOf()));

    // Depth Stencil View

    D3D11_TEXTURE2D_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
    depthStencilDesc.Width = width;
    depthStencilDesc.Height = height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;
    ThrowOnFail(d3dDevice_->CreateTexture2D(&depthStencilDesc, nullptr, depthStencilBuffer_.ReleaseAndGetAddressOf()));

    ThrowOnFail(d3dDevice_->CreateDepthStencilView(depthStencilBuffer_.Get(), nullptr, depthStencilView_.ReleaseAndGetAddressOf()));

    // Configure Output-Merger

    d3dContext_->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), depthStencilView_.Get());

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

ComPtr<ID3D10Blob> D3D11Graphics::CompileShaderFromFile(_In_ LPCWSTR pFileName,
                                                        _In_ LPCSTR pEntrypoint,
                                                        _In_ LPCSTR pTarget)
{
    ComPtr<ID3D10Blob> compiledShader;
    ComPtr<ID3D10Blob> errorMessages;
    HRESULT hr = D3DCompileFromFile(pFileName,
                                    nullptr,
                                    nullptr,
                                    pEntrypoint,
                                    pTarget,
                                    0,
                                    0,
                                    compiledShader.GetAddressOf(),
                                    errorMessages.GetAddressOf());
    if (errorMessages) {
        throw std::runtime_error(static_cast<char*>(errorMessages->GetBufferPointer()));
    }
    ThrowOnFail(hr);
    return compiledShader;
}

void D3D11Graphics::CreateVertexShader()
{
    // Create shader

    ComPtr<ID3D10Blob> vs = CompileShaderFromFile(L"shaders\\flat.hlsl", "vertex_shader", "vs_4_0");
    ThrowOnFail(d3dDevice_->CreateVertexShader(vs->GetBufferPointer(),
                                               vs->GetBufferSize(),
                                               nullptr,
                                               vertexShader_.ReleaseAndGetAddressOf()));
    d3dContext_->VSSetShader(vertexShader_.Get(), 0, 0);

    // Input layout

    D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
         D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
         D3D11_INPUT_PER_VERTEX_DATA, 0}};

    ThrowOnFail(d3dDevice_->CreateInputLayout(inputElementDescriptions,
                                              ARRAYSIZE(inputElementDescriptions),
                                              vs->GetBufferPointer(),
                                              vs->GetBufferSize(),
                                              inputLayout_.ReleaseAndGetAddressOf()));
    d3dContext_->IASetInputLayout(inputLayout_.Get());
}

void D3D11Graphics::CreatePixelShader()
{
    ComPtr<ID3D10Blob> ps = CompileShaderFromFile(L"shaders\\flat.hlsl", "pixel_shader", "ps_4_0");
    ThrowOnFail(d3dDevice_->CreatePixelShader(ps->GetBufferPointer(),
                                              ps->GetBufferSize(),
                                              nullptr,
                                              pixelShader_.ReleaseAndGetAddressOf()));
    d3dContext_->PSSetShader(pixelShader_.Get(), 0, 0);
}

void D3D11Graphics::CreateConstantBuffer()
{
    D3D11_BUFFER_DESC cbDesc;
    ZeroMemory(&cbDesc, sizeof(cbDesc));
    cbDesc.ByteWidth = sizeof(CBufferPerObject);
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA cbSubresourceData;
    ZeroMemory(&cbSubresourceData, sizeof(cbSubresourceData));
    cbSubresourceData.pSysMem = &cbData_;

    ThrowOnFail(d3dDevice_->CreateBuffer(&cbDesc,
                                         &cbSubresourceData,
                                         constantBuffer_.ReleaseAndGetAddressOf()));

    d3dContext_->VSSetConstantBuffers(0, 1, constantBuffer_.GetAddressOf());
}

ComPtr<ID3D11Buffer> D3D11Graphics::CreateVertexBuffer(ShaderVertex* vertices, int numVertices)
{
    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
    vertexBufferDesc.ByteWidth = sizeof(ShaderVertex) * numVertices;
    vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexSubresourceData;
    ZeroMemory(&vertexSubresourceData, sizeof(vertexSubresourceData));
    vertexSubresourceData.pSysMem = vertices;

    ComPtr<ID3D11Buffer> vertexBuffer;
    ThrowOnFail(d3dDevice_->CreateBuffer(&vertexBufferDesc,
                                         &vertexSubresourceData,
                                         vertexBuffer.GetAddressOf()));

    return vertexBuffer;
}

ComPtr<ID3D11Buffer> D3D11Graphics::CreateIndexBuffer(UINT* indices, int numIndices)
{
    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
    indexBufferDesc.ByteWidth = sizeof(UINT) * numIndices;
    indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexSubresourceData;
    ZeroMemory(&indexSubresourceData, sizeof(indexSubresourceData));
    indexSubresourceData.pSysMem = indices;

    ComPtr<ID3D11Buffer> indexBuffer;
    ThrowOnFail(d3dDevice_->CreateBuffer(&indexBufferDesc,
                                         &indexSubresourceData,
                                         indexBuffer.GetAddressOf()));

    return indexBuffer;
}

void D3D11Graphics::CreateTetrahedronVertexBuffer()
{
    auto vertex1 = DirectX::XMFLOAT3(1.0f, 0.0f, -M_SQRT1_2);
    auto vertex2 = DirectX::XMFLOAT3(0.0f, 1.0f, M_SQRT1_2);
    auto vertex3 = DirectX::XMFLOAT3(0.0f, -1.0f, M_SQRT1_2);
    auto vertex4 = DirectX::XMFLOAT3(-1.0f, 0.0f, -M_SQRT1_2);

    DirectX::XMFLOAT3 face1Normal;
    DirectX::XMFLOAT3 face2Normal;
    DirectX::XMFLOAT3 face3Normal;
    DirectX::XMFLOAT3 face4Normal;

    TriangleNormal(&face1Normal, &vertex1, &vertex4, &vertex2);
    TriangleNormal(&face2Normal, &vertex1, &vertex3, &vertex4);
    TriangleNormal(&face3Normal, &vertex1, &vertex2, &vertex3);
    TriangleNormal(&face4Normal, &vertex2, &vertex4, &vertex3);

    ShaderVertex vertices[] = {
        // Face 1
        ShaderVertex(vertex1, face1Normal),
        ShaderVertex(vertex4, face1Normal),
        ShaderVertex(vertex2, face1Normal),
        // Face 2
        ShaderVertex(vertex1, face2Normal),
        ShaderVertex(vertex3, face2Normal),
        ShaderVertex(vertex4, face2Normal),
        // Face 3
        ShaderVertex(vertex1, face3Normal),
        ShaderVertex(vertex2, face3Normal),
        ShaderVertex(vertex3, face3Normal),
        // Face 4
        ShaderVertex(vertex2, face4Normal),
        ShaderVertex(vertex4, face4Normal),
        ShaderVertex(vertex3, face4Normal)};

    tetrahedronVertexBuffer_ = CreateVertexBuffer(vertices, ARRAYSIZE(vertices));
}

void D3D11Graphics::CreateCubeBuffers()
{
    auto vertex1 = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
    auto vertex2 = DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f);
    auto vertex3 = DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f);
    auto vertex4 = DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f);
    auto vertex5 = DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f);
    auto vertex6 = DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f);
    auto vertex7 = DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f);
    auto vertex8 = DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f);

    DirectX::XMFLOAT3 face1Normal;
    DirectX::XMFLOAT3 face2Normal;
    DirectX::XMFLOAT3 face3Normal;
    DirectX::XMFLOAT3 face4Normal;
    DirectX::XMFLOAT3 face5Normal;
    DirectX::XMFLOAT3 face6Normal;

    TriangleNormal(&face1Normal, &vertex2, &vertex5, &vertex8);
    TriangleNormal(&face2Normal, &vertex1, &vertex6, &vertex5);
    TriangleNormal(&face3Normal, &vertex1, &vertex4, &vertex7);
    TriangleNormal(&face4Normal, &vertex3, &vertex8, &vertex7);
    TriangleNormal(&face5Normal, &vertex1, &vertex2, &vertex3);
    TriangleNormal(&face6Normal, &vertex5, &vertex6, &vertex7);

    ShaderVertex vertices[] = {
        // Face 1
        ShaderVertex(vertex2, face1Normal),
        ShaderVertex(vertex5, face1Normal),
        ShaderVertex(vertex8, face1Normal),
        ShaderVertex(vertex3, face1Normal),
        // Face 2
        ShaderVertex(vertex1, face2Normal),
        ShaderVertex(vertex6, face2Normal),
        ShaderVertex(vertex5, face2Normal),
        ShaderVertex(vertex2, face2Normal),
        // Face 3
        ShaderVertex(vertex1, face3Normal),
        ShaderVertex(vertex4, face3Normal),
        ShaderVertex(vertex7, face3Normal),
        ShaderVertex(vertex6, face3Normal),
        // Face 4
        ShaderVertex(vertex3, face4Normal),
        ShaderVertex(vertex8, face4Normal),
        ShaderVertex(vertex7, face4Normal),
        ShaderVertex(vertex4, face4Normal),
        // Face 5
        ShaderVertex(vertex1, face5Normal),
        ShaderVertex(vertex2, face5Normal),
        ShaderVertex(vertex3, face5Normal),
        ShaderVertex(vertex4, face5Normal),
        // Face 6
        ShaderVertex(vertex5, face6Normal),
        ShaderVertex(vertex6, face6Normal),
        ShaderVertex(vertex7, face6Normal),
        ShaderVertex(vertex8, face6Normal)};

    UINT indices[] = {
        // Face 1
        0, 1, 2,
        0, 2, 3,
        // Face 2
        4, 5, 6,
        4, 6, 7,
        // Face 3
        8, 9, 10,
        8, 10, 11,
        // Face 4
        12, 13, 14,
        12, 14, 15,
        // Face 5
        16, 17, 18,
        16, 18, 19,
        // Face 6
        20, 21, 22,
        20, 22, 23};

    cubeVertexBuffer_ = CreateVertexBuffer(vertices, ARRAYSIZE(vertices));
    cubeIndexBuffer_ = CreateIndexBuffer(indices, ARRAYSIZE(indices));
}

void D3D11Graphics::InitViewProjectionMatrix()
{
    // TODO: Move the View-Projection matrix to its own 'camera' class

    // View
    auto eye = DirectX::XMVectorSet(0.0f, 0.0f, -10.0f, 0.0f);
    auto at = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    auto up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    auto viewMatrix = DirectX::XMMatrixLookAtLH(eye, at, up);

    // Projection
    // TODO: Angle of view?
    auto projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 1.0f, 1.0f, 100.0f);

    // View-Projection
    DirectX::XMStoreFloat4x4(&viewProjectionMatrix_, viewMatrix * projectionMatrix);
}

void D3D11Graphics::ResetMatrix()
{
    DirectX::XMStoreFloat4x4(&worldMatrix_, DirectX::XMMatrixIdentity());
}

void D3D11Graphics::InitLighting()
{
    lightDirection_ = DirectX::XMFLOAT4(1.0f, 1.0f, -1.0f, 0.0f);
    lightColor_ = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
    ambientLightColor_ = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
}

void D3D11Graphics::InitMaterial()
{
    materialColor_ = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
}

void D3D11Graphics::UpdateConstantBufferForObject()
{
    auto viewProjectionMatrix = DirectX::XMLoadFloat4x4(&viewProjectionMatrix_);
    auto worldMatrix = DirectX::XMLoadFloat4x4(&worldMatrix_);
    DirectX::XMStoreFloat4x4(&cbData_.WorldViewProjection, worldMatrix * viewProjectionMatrix);

    auto inverseWorldMatrix = DirectX::XMMatrixInverse(nullptr, worldMatrix);

    auto lightDirection = DirectX::XMLoadFloat4(&lightDirection_);
    DirectX::XMStoreFloat4(&cbData_.LightDirection,
                           DirectX::XMVector4Normalize(DirectX::XMVector4Transform(lightDirection, inverseWorldMatrix)));

    cbData_.LightColor = lightColor_;
    cbData_.AmbientLightColor = ambientLightColor_;
    cbData_.MaterialColor = materialColor_;

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    ZeroMemory(&mappedResource, sizeof(mappedResource));

    ThrowOnFail(d3dContext_->Map(constantBuffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
    memcpy(mappedResource.pData, &cbData_, sizeof(cbData_));
    d3dContext_->Unmap(constantBuffer_.Get(), 0);
}

} // namespace Procdraw
