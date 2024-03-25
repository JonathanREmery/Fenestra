#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "win.h"
#include <d3d11.h>

namespace Fenestra {
    class Graphics {
        private:
            ID3D11Device* pDevice;
            IDXGISwapChain* pSwapChain;
            ID3D11DeviceContext* pDeviceContext;
            ID3D11RenderTargetView* pTarget;
        public:
            Graphics(HWND hWnd) noexcept;
            ~Graphics() noexcept;
            void endFrame() const noexcept;
            void clearBuffer(float red, float green, float blue) const noexcept;
    };
}

#endif