#ifndef TIME_H
#define TIME_H

#include <chrono>

namespace Fenestra {
    class Time {
        private:
            static Time* instance;
            
            std::chrono::high_resolution_clock::time_point start;

            Time() noexcept;
            ~Time() noexcept;
        public:
            static Time* getInstance() noexcept;
            double getElapsedTime() noexcept;
    };
}

#endif