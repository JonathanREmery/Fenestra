#include <string>
#include <cmath>

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
        handleMessage(handleMessage),
        graphics(nullptr),
        time(Time::getInstance())
    {
        registerWindowClass();
        createWindow();
    }

    Window::~Window() noexcept {
        if (this->graphics != nullptr) {
            delete this->graphics;
        }

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

        if (wc.hbrBackground == nullptr) {
            MessageBoxA(nullptr, "Failed to set window HBR background!", "Error", MB_ICONERROR | MB_OK);
        }

        if (this->iconPath != nullptr) {
            wc.hIcon = (HICON)LoadImageA(nullptr, this->iconPath, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

            if (wc.hIcon == nullptr) {
                MessageBoxA(nullptr, "Failed to load icon!", "Error", MB_ICONERROR | MB_OK);
            }
        } else {
            wc.hIcon = nullptr;
        }

        if (RegisterClassA(&wc) == 0) {
            MessageBoxA(nullptr, "Failed to register window class!", "Error", MB_ICONERROR | MB_OK);
            exit(-1);
        }
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

        if (this->hWnd == nullptr) {
            MessageBoxA(nullptr, "Failed to create window!", "Error", MB_ICONERROR | MB_OK);
            exit(-1);
        }

        this->graphics = new Graphics(this->hWnd);

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
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                return 0;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        this->graphics->clearBuffer(
            sin((this->time->getElapsedTime() / 1000.0) + (3.14 / 2.0)) * 0.5f + 0.5f,
            sin(this->time->getElapsedTime() / 1000.0) * 0.5f + 0.5f,
            sin((this->time->getElapsedTime() / 1000.0) - (3.14 / 2.0)) * 0.5f + 0.5f
        );
        this->graphics->endFrame();
        return 1;
    }
}