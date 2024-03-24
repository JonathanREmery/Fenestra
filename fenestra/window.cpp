#include <string>

#include "window.h"

namespace Fenestra {
    Window::Window(const char* title) noexcept : Window(title, 800, 600) {}
    Window::Window(const char* title, int width, int height) noexcept : Window(title, width, height, nullptr) {}
    Window::Window(const char* title, int width, int height, const char* iconPath) noexcept : Window(title, width, height, iconPath, nullptr) {}

    Window::Window(const char* title, int width, int height, const char* iconPath, MESSAGE_HANDLER handleMessage) noexcept :
        title(title),
        width(width),
        height(height),
        iconPath(iconPath),
        hInstance(GetModuleHandle(nullptr)),
        handleMessage(handleMessage)
    {
        registerWindowClass();
        createWindow();
    }

    Window::~Window() noexcept {
        DestroyWindow(this->hWnd);
        UnregisterClassA(windowClassName, this->hInstance);
    }

    void Window::registerWindowClass() const noexcept {
        WNDCLASSA wc;

        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wc.hCursor = nullptr;
        wc.hInstance = this->hInstance;
        wc.lpfnWndProc = Window::wndProcSetup;
        wc.lpszClassName = this->windowClassName;
        wc.lpszMenuName = nullptr;
        wc.style = CS_OWNDC;

        if (this->iconPath != nullptr) {
            wc.hIcon = (HICON)LoadImageA(nullptr, this->iconPath, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
        } else {
            wc.hIcon = nullptr;
        }

        RegisterClassA(&wc);
    }

    void Window::createWindow() noexcept {
        this->hWnd = CreateWindowA(
            this->windowClassName,
            this->title,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            this->width,
            this->height,
            nullptr,
            nullptr,
            this->hInstance,
            this
        );

        ShowWindow(hWnd, SW_SHOW);
    }

    LRESULT WINAPI Window::wndProcSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
        if (msg == WM_NCCREATE) {
            const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
            const Window* const pWnd = (Window*)pCreate->lpCreateParams;

            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
            SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::wndProcThunk));

            if (pWnd->handleMessage != nullptr) {
                pWnd->handleMessage(msg, wParam, lParam);
            }

            return DefWindowProc(hWnd, msg, wParam, lParam);
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    LRESULT WINAPI Window::wndProcThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
        Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        if (pWnd->handleMessage != nullptr) {
            pWnd->handleMessage(msg, wParam, lParam);
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    int Window::update() noexcept {
        MSG msg;
        int gResult = GetMessage(&msg, nullptr, 0, 0);

        if (gResult <= 0) {
            return 0;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);

        return 1;
    }
}