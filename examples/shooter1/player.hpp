#ifndef PLAYER_H
#define PLAYER_H

#include "bullet.hpp"
#include "game.hpp"

#include <sgfx/color.hpp>
#include <sgfx/image.hpp>
#include <sgfx/key.hpp>
#include <sgfx/primitives.hpp>

#include <array>
#include <chrono>
#include <memory>
#include <string>

class player final : public game_object, autoregister_collider {
  public:
    struct key_config {
        sgfx::key_id left, right, up, down;
        sgfx::key_id shoot;
    };

    player(game_proxy proxy, resource_manager& rm, const std::string& name, const key_config& keys);

    void accept(game_object_visitor&) override;

    sgfx::rectangle bounds() const override
    {
        auto const& current_img = *imgs_[static_cast<int>(current_status_)];
        auto const center_offset = sgfx::vec{current_img.width(), current_img.height()} / 2;
        return {pos_ - center_offset, {current_img.width(), current_img.height()}};
    }

    void hit() override { --lifes_; }

    static constexpr auto key_color = sgfx::color::rgb_color{0xcb, 0x48, 0xb7};

    enum class state { normal, flying_left, flying_right };

    resource_manager& resource_manager() const noexcept { return resource_manager_; }

    sgfx::rle_image const& img(state _state) const noexcept { return *imgs_.at(static_cast<size_t>(_state)); }
    key_config const& keys() const noexcept { return keys_; }

    state current_status() const noexcept { return current_status_; }
    sgfx::point pos() const noexcept { return pos_; }
    int lifes() const noexcept { return lifes_; }
    bool space_released() const noexcept { return space_released_; }

	// modifier
    state& current_status() noexcept { return current_status_; }
    sgfx::point& pos() noexcept { return pos_; }
    int& lifes() noexcept { return lifes_; }
    bool& space_released() noexcept { return space_released_; }

  private:
    ::resource_manager& resource_manager_;
    std::array<sgfx::rle_image const*, 3> imgs_;
    key_config keys_;
    state current_status_ = state::normal;
    sgfx::point pos_{0, 0};
    int lifes_ = 3;
    bool space_released_ = true;
};

#endif
