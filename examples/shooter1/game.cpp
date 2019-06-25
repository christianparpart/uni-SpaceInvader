#include "game.hpp"
#include <algorithm>
#include <memory>
#include <utility>
#include "game_object_visitor.hpp"
#include "stats.hpp"

sgfx::rle_image const& resource_manager::rle(std::string const& path)
{
    auto i = rle_images_.find(path);
    if (i != rle_images_.end())
        return i->second;

    rle_images_.emplace(make_pair(path, sgfx::load_rle(path)));
    return rle(path);
}

game::game() : wnd_{640, 480, "Space!"}
{
}

stats game::run()
{
    stats stats;

    while (wnd_.handle_events() && !wnd_.should_close())
    {
        using namespace std::chrono;
        auto const now = high_resolution_clock::now();
        leftover_time += duration_cast<milliseconds>(now - last_time);
        last_time = now;

        while (leftover_time >= step_time)
        {
            {
                perf_timer timer{stats.collider};
                collision_mgr_.handle_collisions();
            }

            for (std::size_t i = 0; i < objs_.size(); ++i)
            {
                perf_timer timer{stats.updates};
                auto const obj_state = game_object_updater::update(game_proxy{this}, step_time, *objs_[i]);

                if (obj_state == game_object::status::dead)
                {
                    std::swap(objs_[i], objs_.back());
                    objs_.pop_back();
                    --i;
                }
            }

            leftover_time -= step_time;
        }

        std::for_each(begin(objs_), end(objs_), [&](auto& obj) {
            perf_timer timer{stats.draws};
            game_object_drawer::draw(wnd_, *obj);
        });

        {
            perf_timer timer{stats.render};
            wnd_.show();
        }
    }

    std::cout << "stats: " << stats << std::endl;

    return stats;
}

void game::spawn(std::unique_ptr<game_object> new_obj)
{
    objs_.push_back(std::move(new_obj));
}
