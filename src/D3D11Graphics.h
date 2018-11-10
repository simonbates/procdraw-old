/* Copyright (c) 2014, 2015, 2018 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#include <DirectXMath.h>
#include <d3d11_1.h>
#include <string>
#include <wrl.h>

namespace Procdraw {

struct ShaderVertex {
    DirectX::XMFLOAT4 pos;
    DirectX::XMFLOAT3 normal;
    ShaderVertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 normal)
        : normal(normal)
    {
        this->pos = DirectX::XMFLOAT4(pos.x, pos.y, pos.z, 1.0f);
    }
};

struct CBufferPerObject {
    DirectX::XMFLOAT4X4 WorldViewProjection;
    DirectX::XMFLOAT4 LightDirection;
    DirectX::XMFLOAT4 LightColor;
    DirectX::XMFLOAT4 AmbientLightColor;
    DirectX::XMFLOAT4 MaterialColor;
};

class D3D11Graphics {
public:
    D3D11Graphics(HWND hWnd);
    ~D3D11Graphics();
    void Background(float h, float s, float v);
    void LightColor(float h, float s, float v);
    void AmbientLightColor(float h, float s, float v);
    void Color(float h, float s, float v);
    void Present();
    void Tetrahedron();
    void Cube();
    void RotateX(float turns);
    void RotateY(float turns);
    void RotateZ(float turns);
    void Translate(float x, float y, float z);
    void Scale(float x, float y, float z);

private:
    HWND hWnd_;
    D3D_FEATURE_LEVEL d3dFeatureLevel_;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain_;
    Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice_;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext_;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer_;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView_;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer_;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView_;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader_;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader_;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout_;
    CBufferPerObject cbData_;
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer_;
    Microsoft::WRL::ComPtr<ID3D11Buffer> tetrahedronVertexBuffer_;
    Microsoft::WRL::ComPtr<ID3D11Buffer> cubeVertexBuffer_;
    Microsoft::WRL::ComPtr<ID3D11Buffer> cubeIndexBuffer_;
    DirectX::XMFLOAT4X4 viewProjectionMatrix_;
    DirectX::XMFLOAT4X4 worldMatrix_;
    DirectX::XMFLOAT4 lightDirection_;
    DirectX::XMFLOAT4 lightColor_;
    DirectX::XMFLOAT4 ambientLightColor_;
    DirectX::XMFLOAT4 materialColor_;
    void InitD3D();
    Microsoft::WRL::ComPtr<ID3D10Blob> CompileShader(LPCVOID pSrcData,
                                                     SIZE_T SrcDataSize,
                                                     LPCSTR pEntrypoint,
                                                     LPCSTR pTarget);
    void CreateVertexShader();
    void CreatePixelShader();
    void CreateConstantBuffer();
    Microsoft::WRL::ComPtr<ID3D11Buffer> CreateVertexBuffer(ShaderVertex* vertices, int numVertices);
    Microsoft::WRL::ComPtr<ID3D11Buffer> CreateIndexBuffer(UINT* indices, int numIndices);
    void CreateTetrahedronVertexBuffer();
    void CreateCubeBuffers();
    void InitViewProjectionMatrix();
    void ResetMatrix();
    void InitLighting();
    void InitMaterial();
    void UpdateConstantBufferForObject();
};

} // namespace Procdraw
