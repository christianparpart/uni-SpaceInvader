#ifndef SPAWN_POINT_H
#define SPAWN_POINT_H

#include "game.hpp"

#include <sgfx/primitives.hpp>

#include <memory>
#include <random>

inline int random_uniform_int(int max)
{
    static std::mt19937 random_gen{std::random_device{}()};
    std::uniform_int_distribution<int> dist(0, max - 1);
    return dist(random_gen);
}

template <typename spawn_function_type>
class spawn_point final : public game_object {
  public:
    spawn_point(sgfx::rectangle area, spawn_function_type spawn_fun, std::chrono::milliseconds delay)
        : area_{area}, spawn_fun_{spawn_fun}, delay_{delay}, time_remaining_{delay}
    {
    }

    game_object::status update(game_proxy proxy, std::chrono::milliseconds delta) override
    {
        while (time_remaining_ <= delta)
        {
            const auto pos = sgfx::point{area_.top_left.x + random_uniform_int(area_.size.w),
                                         area_.top_left.y + random_uniform_int(area_.size.h)};
            spawn_fun_(proxy, pos);
            delta -= time_remaining_;
            time_remaining_ = delay_;
        }
        time_remaining_ -= delta;

        return game_object::status::alive;
    }

    void draw(sgfx::canvas_view target) const {}

  private:
    sgfx::rectangle area_;
    spawn_function_type spawn_fun_;
    std::chrono::milliseconds delay_, time_remaining_;
};

template <typename spawn_function_type>
spawn_point(sgfx::rectangle, spawn_function_type, std::chrono::milliseconds)
    ->spawn_point<spawn_function_type>;

template <typename spawn_function_type>
auto make_spawn_point(sgfx::rectangle area, spawn_function_type spawn_fun, std::chrono::milliseconds delay)
{
    return new spawn_point<spawn_function_type>(area, spawn_fun, delay);
}

#endif
