#pragma once
#include <chrono>
#include <iostream>

struct stats {
    std::chrono::steady_clock::duration collider{};
    std::chrono::steady_clock::duration updates{};
    std::chrono::steady_clock::duration draws{};
    std::chrono::steady_clock::duration render{};
};

struct perf_timer {
    std::chrono::steady_clock::duration& sink;
    std::chrono::time_point<std::chrono::steady_clock> const start = std::chrono::steady_clock::now();

    auto elapsed() const { return std::chrono::steady_clock::now() - start; }

    ~perf_timer() { sink += elapsed(); }
};

inline std::ostream& operator<<(std::ostream& os, stats const& st)
{
    os << "{collider: " << std::chrono::duration_cast<std::chrono::milliseconds>(st.collider).count()
       << "ms, updates: " << std::chrono::duration_cast<std::chrono::milliseconds>(st.updates).count()
       << "ms, draws: " << std::chrono::duration_cast<std::chrono::milliseconds>(st.draws).count() << "ms}";
    return os;
}
