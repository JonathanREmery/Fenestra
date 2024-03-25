#include "time.h"

namespace Fenestra {
    Time* Time::instance = nullptr;

    Time::Time() noexcept {
        start = std::chrono::high_resolution_clock::now();
    }

    Time::~Time() noexcept {
        delete instance;
    }

    Time* Time::getInstance() noexcept {
        if (instance == nullptr) {
            instance = new Time();
        }

        return instance;
    }

    double Time::getElapsedTime() noexcept {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        return duration.count();
    }
}