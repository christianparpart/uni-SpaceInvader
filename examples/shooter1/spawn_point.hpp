#ifndef SPAWN_POINT_H
#define SPAWN_POINT_H

#include "game.hpp"

#include <sgfx/primitives.hpp>

#include <functional>
#include <memory>
#include <random>

inline int random_uniform_int(int max)
{
    static std::mt19937 random_gen{std::random_device{}()};
    std::uniform_int_distribution<int> dist(0, max - 1);
    return dist(random_gen);
}

using spawn_function_type = std::function<void(game_proxy, sgfx::point)>;

class spawn_point final : public game_object {
  public:
    spawn_point(sgfx::rectangle area, spawn_function_type spawn_fun, std::chrono::milliseconds delay)
        : area_{area}, spawn_fun_{spawn_fun}, delay_{delay}, time_remaining_{delay}
    {
    }

	void accept(game_object_visitor& visitor) { return visitor.visit(*this); }

    game_object::status update(game_proxy proxy, std::chrono::milliseconds delta) override
    {
        while (time_remaining_ <= delta)
        {
            auto const pos = sgfx::point{area_.top_left.x + random_uniform_int(area_.size.w),
                                         area_.top_left.y + random_uniform_int(area_.size.h)};
            spawn_fun_(proxy, pos);
            delta -= time_remaining_;
            time_remaining_ = delay_;
        }
        time_remaining_ -= delta;

        return game_object::status::alive;
    }

  private:
    sgfx::rectangle area_;
    spawn_function_type spawn_fun_;
    std::chrono::milliseconds delay_, time_remaining_;
};

auto inline make_spawn_point(sgfx::rectangle area, spawn_function_type spawn_fun,
                             std::chrono::milliseconds delay)
{
    return std::make_unique<spawn_point>(area, spawn_fun, delay);
}

#endif
