#ifndef WINDOW_H
#define WINDOW_H

#include "win.h"

namespace Fenestra {
    typedef void (*MESSAGE_HANDLER)(unsigned int, unsigned long long, long long);

    class Window {
        private:
            static constexpr const char* windowClassName = "FenestraWindowClass";

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

            MESSAGE_HANDLER handleMessage;
        public:
            Window(const char* title) noexcept;
            Window(const char* title, int width, int height) noexcept;
            Window(const char* title, int width, int height, const char* iconPath) noexcept;
            Window(const char* title, int width, int height, const char* iconPath, MESSAGE_HANDLER handleMessage) noexcept;
            ~Window() noexcept;

            void setMessageHandler(MESSAGE_HANDLER handleMessage) noexcept;

            int update() noexcept;
    };
}

#endif