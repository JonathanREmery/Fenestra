#include "fenestra/window.h"

void handleMessage(unsigned int msg, unsigned long long wParam, long long lParam) {
    switch (msg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
    }
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    Fenestra::Window window("Fenestra Window", 800, 600, nullptr, handleMessage);

    while (window.update() > 0) {}

    return 0;
}