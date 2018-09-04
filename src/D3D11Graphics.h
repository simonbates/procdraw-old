/* Copyright (c) 2014, 2015, 2018 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#include <DirectXMath.h>
#include <comdef.h>
#include <d3d11_1.h>
#include <string>

// DXGI
_COM_SMARTPTR_TYPEDEF(IDXGIDevice, __uuidof(IDXGIDevice));
_COM_SMARTPTR_TYPEDEF(IDXGISurface, __uuidof(IDXGISurface));
_COM_SMARTPTR_TYPEDEF(IDXGISwapChain, __uuidof(IDXGISwapChain));
// D3D10
_COM_SMARTPTR_TYPEDEF(ID3D10Blob, __uuidof(ID3D10Blob));
// D3D11
_COM_SMARTPTR_TYPEDEF(ID3D11Buffer, __uuidof(ID3D11Buffer));
_COM_SMARTPTR_TYPEDEF(ID3D11DepthStencilView, __uuidof(ID3D11DepthStencilView));
_COM_SMARTPTR_TYPEDEF(ID3D11Device, __uuidof(ID3D11Device));
_COM_SMARTPTR_TYPEDEF(ID3D11DeviceContext, __uuidof(ID3D11DeviceContext));
_COM_SMARTPTR_TYPEDEF(ID3D11InputLayout, __uuidof(ID3D11InputLayout));
_COM_SMARTPTR_TYPEDEF(ID3D11PixelShader, __uuidof(ID3D11PixelShader));
_COM_SMARTPTR_TYPEDEF(ID3D11RenderTargetView, __uuidof(ID3D11RenderTargetView));
_COM_SMARTPTR_TYPEDEF(ID3D11Texture2D, __uuidof(ID3D11Texture2D));
_COM_SMARTPTR_TYPEDEF(ID3D11VertexShader, __uuidof(ID3D11VertexShader));

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
    void RotateX(float angle);
    void RotateY(float angle);
    void RotateZ(float angle);
    void Translate(float x, float y, float z);
    void Scale(float x, float y, float z);

private:
    HWND hWnd_;
    D3D_FEATURE_LEVEL d3dFeatureLevel_;
    IDXGISwapChainPtr swapChain_;
    ID3D11DevicePtr d3dDevice_;
    ID3D11DeviceContextPtr d3dContext_;
    ID3D11Texture2DPtr backBuffer_;
    ID3D11RenderTargetViewPtr renderTargetView_;
    ID3D11Texture2DPtr depthStencilBuffer_;
    ID3D11DepthStencilViewPtr depthStencilView_;
    ID3D11VertexShaderPtr vertexShader_;
    ID3D11PixelShaderPtr pixelShader_;
    ID3D11InputLayoutPtr inputLayout_;
    CBufferPerObject cbData_;
    ID3D11BufferPtr constantBuffer_;
    ID3D11BufferPtr tetrahedronVertexBuffer_;
    ID3D11BufferPtr cubeVertexBuffer_;
    ID3D11BufferPtr cubeIndexBuffer_;
    DirectX::XMFLOAT4X4 viewProjectionMatrix_;
    DirectX::XMFLOAT4X4 worldMatrix_;
    DirectX::XMFLOAT4 lightDirection_;
    DirectX::XMFLOAT4 lightColor_;
    DirectX::XMFLOAT4 ambientLightColor_;
    DirectX::XMFLOAT4 materialColor_;
    void InitD3D();
    ID3D10BlobPtr CompileShaderFromFile(_In_ LPCWSTR pFileName,
                                        _In_ LPCSTR pEntrypoint,
                                        _In_ LPCSTR pTarget);
    void CreateVertexShader();
    void CreatePixelShader();
    void CreateConstantBuffer();
    ID3D11BufferPtr CreateVertexBuffer(ShaderVertex* vertices, int numVertices);
    ID3D11BufferPtr CreateIndexBuffer(UINT* indices, int numIndices);
    void CreateTetrahedronVertexBuffer();
    void CreateCubeBuffers();
    void InitViewProjectionMatrix();
    void ResetMatrix();
    void InitLighting();
    void InitMaterial();
    void UpdateConstantBufferForObject();
};

} // namespace Procdraw
