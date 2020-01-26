// Copyright 2020 Simon Bates
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef PROCDRAW_D3D11GRAPHICS_H
#define PROCDRAW_D3D11GRAPHICS_H

#include <DirectXMath.h>
#include <d3d11_1.h>
#include <string>
#include <wil/com.h>

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
    wil::com_ptr<IDXGISwapChain> swapChain_;
    wil::com_ptr<ID3D11Device> d3dDevice_;
    wil::com_ptr<ID3D11DeviceContext> d3dContext_;
    wil::com_ptr<ID3D11Texture2D> backBuffer_;
    wil::com_ptr<ID3D11RenderTargetView> renderTargetView_;
    wil::com_ptr<ID3D11Texture2D> depthStencilBuffer_;
    wil::com_ptr<ID3D11DepthStencilView> depthStencilView_;
    wil::com_ptr<ID3D11VertexShader> vertexShader_;
    wil::com_ptr<ID3D11PixelShader> pixelShader_;
    wil::com_ptr<ID3D11InputLayout> inputLayout_;
    CBufferPerObject cbData_;
    wil::com_ptr<ID3D11Buffer> constantBuffer_;
    wil::com_ptr<ID3D11Buffer> tetrahedronVertexBuffer_;
    wil::com_ptr<ID3D11Buffer> cubeVertexBuffer_;
    wil::com_ptr<ID3D11Buffer> cubeIndexBuffer_;
    DirectX::XMFLOAT4X4 viewProjectionMatrix_;
    DirectX::XMFLOAT4X4 worldMatrix_;
    DirectX::XMFLOAT4 lightDirection_;
    DirectX::XMFLOAT4 lightColor_;
    DirectX::XMFLOAT4 ambientLightColor_;
    DirectX::XMFLOAT4 materialColor_;
    void InitD3D();
    wil::com_ptr<ID3D10Blob> CompileShader(LPCVOID pSrcData,
                                           SIZE_T SrcDataSize,
                                           LPCSTR pEntrypoint,
                                           LPCSTR pTarget);
    void CreateVertexShader();
    void CreatePixelShader();
    void CreateConstantBuffer();
    wil::com_ptr<ID3D11Buffer> CreateVertexBuffer(ShaderVertex* vertices, int numVertices);
    wil::com_ptr<ID3D11Buffer> CreateIndexBuffer(UINT* indices, int numIndices);
    void CreateTetrahedronVertexBuffer();
    void CreateCubeBuffers();
    void InitViewProjectionMatrix();
    void ResetMatrix();
    void InitLighting();
    void InitMaterial();
    void UpdateConstantBufferForObject();
};

} // namespace Procdraw

#endif
