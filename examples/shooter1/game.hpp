#ifndef GAME_H
#define GAME_H

#include "collision_manager.hpp"
#include "stats.hpp"

#include <sgfx/image.hpp>
#include <sgfx/primitives.hpp>
#include <sgfx/window.hpp>

#include <chrono>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <vector>

class game;
class game_object;
class game_object_visitor;

class resource_manager {
  public:
    sgfx::rle_image const& rle(std::string const& path);

  private:
    std::unordered_map<std::string, sgfx::rle_image> rle_images_;
};

class game_proxy {
  public:
    template <typename T, typename... arg_types>
    void spawn(arg_types&&...);

    collision_manager::handle register_collider(collider& new_collider);

    bool is_pressed(sgfx::key_id) const;
    sgfx::rectangle area() const;

  private:
    game* parent_;

    explicit game_proxy(game* parent) : parent_{parent} {};

    friend class game;
};

class game_object {
  public:
    enum class [[nodiscard]] status{dead, alive};

    virtual void accept(game_object_visitor& visitor) = 0;

    virtual ~game_object() = default;
    game_object() = default;
    game_object(const game_object&) = delete;
    game_object& operator=(const game_object&) = delete;
    game_object(game_object&&) = delete;
    game_object& operator=(game_object&&) = delete;
};

class game {
  public:
    game();

    template <typename T, typename... arg_types>
    void spawn(arg_types&&... args)
    {
        spawn(std::make_unique<T>(game_proxy{this}, std::forward<arg_types>(args)...));
    }

    void spawn(std::unique_ptr<game_object>);

    stats run();

  private:
    // NB: The game should not own the drawing target, for recomposability.
    sgfx::window wnd_;

    collision_manager collision_mgr_;
    std::vector<std::unique_ptr<game_object>> objs_;

    std::chrono::high_resolution_clock::time_point last_time = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds leftover_time{0};
    std::chrono::milliseconds const step_time = std::chrono::milliseconds{1000} / 120;

    friend class game_proxy;
};

template <typename T, typename... arg_types>
inline void game_proxy::spawn(arg_types&&... args)
{
    parent_->spawn<T>(std::forward<arg_types>(args)...);
}

inline bool game_proxy::is_pressed(sgfx::key_id key) const
{
    return parent_->wnd_.is_pressed(key);
}

inline sgfx::rectangle game_proxy::area() const
{
    return {{0, 0}, {parent_->wnd_.width(), parent_->wnd_.height()}};
}

inline collision_manager::handle game_proxy::register_collider(collider& new_collider)
{
    return parent_->collision_mgr_.register_collider(new_collider);
}

class autoregister_collider : public collider {
  public:
    autoregister_collider(game_proxy proxy) : collision_handle_{proxy.register_collider(*this)} {}

  protected:
    ~autoregister_collider() = default;

  private:
    collision_manager::handle collision_handle_;
};

#endif
