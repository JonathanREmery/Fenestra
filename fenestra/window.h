#ifndef WINDOW_H
#define WINDOW_H

#include "win.h"

namespace Fenestra {
    class Window {
        private:
            static constexpr LPCWSTR windowClassName = L"FenestraWindowClass";

            const char* title;
            const int width;
            const int height;
            const char* iconPath;

            const HINSTANCE hInstance;
            HWND hWnd;

            void registerWindowClass() const noexcept;
            void createWindow() noexcept;

            static LRESULT WINAPI wndProcSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
            static LRESULT WINAPI wndProcThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
            
            WNDPROC wndProc;
        public:
            Window(const char* title) noexcept;
            Window(const char* title, int width, int height) noexcept;
            Window(const char* title, int width, int height, const char* iconPath) noexcept;
            Window(const char* title, int width, int height, const char* iconPath, WNDPROC windowProcedure) noexcept;
            ~Window() noexcept;

            void setWindowProcedure(WNDPROC windowProcedure) noexcept;
    };
}

#endif