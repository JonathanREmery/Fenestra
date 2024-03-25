#include <sstream>

#include "graphics.h"

namespace Fenestra {
    Graphics::Graphics(HWND hWnd) noexcept {
        if (hWnd == nullptr) {
            MessageBoxA(nullptr, "Failed to create graphics object! Window handle is null!", "Error", MB_ICONERROR | MB_OK);
            exit(-1);
        }

        DXGI_SWAP_CHAIN_DESC scd;

        scd.BufferCount = 1;
        scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        scd.BufferDesc.Height = 0;
        scd.BufferDesc.Width = 0;
        scd.BufferDesc.RefreshRate.Numerator = 0;
        scd.BufferDesc.RefreshRate.Denominator = 0;
        scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.Flags = 0;
        scd.OutputWindow = hWnd;
        scd.SampleDesc.Count = 1;
        scd.SampleDesc.Quality = 0;
        scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        scd.Windowed = TRUE;

        HRESULT result = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            0,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &scd,
            &this->pSwapChain,
            &this->pDevice,
            nullptr,
            &this->pDeviceContext
        );

        if (result != 0) {
            std::string message = "Failed to create device and swap chain! Error code: " + std::to_string(result);
            MessageBoxA(nullptr, message.c_str(), "Error", MB_ICONERROR | MB_OK);
            exit(-1);
        }

        ID3D11Resource* pBackBuffer = nullptr;
        this->pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));

        this->pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &this->pTarget);
        pBackBuffer->Release();
    }

    Graphics::~Graphics() noexcept {
        if (this->pDevice != nullptr) {
            this->pDevice->Release();
        }

        if (this->pSwapChain != nullptr) {
            this->pSwapChain->Release();
        }

        if (this->pDeviceContext != nullptr) {
            this->pDeviceContext->Release();
        }

        if (this->pTarget != nullptr) {
            this->pTarget->Release();
        }
    }
    
    void Graphics::endFrame() const noexcept {
        this->pSwapChain->Present(1u, 0u);
    }

    void Graphics::clearBuffer(float red, float green, float blue) const noexcept {
        const float color[] = { red, green, blue, 1.0f };
        this->pDeviceContext->ClearRenderTargetView(this->pTarget, color);
    }
}