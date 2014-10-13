#pragma once

#include <string>
#include <d3d11_1.h>
#include <D2d1_1.h>
#include <Dwrite.h>
#include <comdef.h>

// DXGI
_COM_SMARTPTR_TYPEDEF(IDXGIDevice, __uuidof(IDXGIDevice));
_COM_SMARTPTR_TYPEDEF(IDXGISurface, __uuidof(IDXGISurface));
_COM_SMARTPTR_TYPEDEF(IDXGISwapChain, __uuidof(IDXGISwapChain));
// D3D11
_COM_SMARTPTR_TYPEDEF(ID3D11Device, __uuidof(ID3D11Device));
_COM_SMARTPTR_TYPEDEF(ID3D11DeviceContext, __uuidof(ID3D11DeviceContext));
_COM_SMARTPTR_TYPEDEF(ID3D11RenderTargetView, __uuidof(ID3D11RenderTargetView));
_COM_SMARTPTR_TYPEDEF(ID3D11Texture2D, __uuidof(ID3D11Texture2D));

namespace procdraw {

    class D3D11Graphics {
    public:
        D3D11Graphics(HWND hWnd);
        ~D3D11Graphics();
        void Background(float h, float s, float v);
        void Present();
    private:
        HWND hWnd_;
        D3D_FEATURE_LEVEL d3dFeatureLevel_;
        IDXGISwapChainPtr swapChain_;
        ID3D11DevicePtr d3dDevice_;
        ID3D11DeviceContextPtr immediateContext_;
        ID3D11Texture2DPtr backBuffer_;
        ID3D11RenderTargetViewPtr renderTargetView_;
        void InitD3D();
    };

}
