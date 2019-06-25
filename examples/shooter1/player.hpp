#ifndef PLAYER_H
#define PLAYER_H

#include "bullet.hpp"
#include "game.hpp"

#include <sgfx/color.hpp>
#include <sgfx/image.hpp>
#include <sgfx/key.hpp>
#include <sgfx/primitives.hpp>

#include <chrono>
#include <memory>
#include <string>
#include <array>

class player final : public game_object, autoregister_collider {
  public:
    struct key_config {
        sgfx::key_id left, right, up, down;
        sgfx::key_id shoot;
    };

    player(game_proxy proxy, resource_manager& rm, const std::string& name, const key_config& keys);

	void accept(game_object_visitor&) override;

    game_object::status update(game_proxy proxy, std::chrono::milliseconds delta) override;
    void draw(sgfx::canvas_view target) const override;

    sgfx::rectangle bounds() const override
    {
        auto const& current_img = *imgs_[static_cast<int>(current_status_)];
        auto const center_offset = sgfx::vec{current_img.width(), current_img.height()} / 2;
        return {pos_ - center_offset, {current_img.width(), current_img.height()}};
    }

    void hit() override { --lifes; }

  private:
    resource_manager& resource_manager_;
    std::array<sgfx::rle_image const*, 3> imgs_;
    key_config keys_;

    enum class state { normal, flying_left, flying_right };
    state current_status_ = state::normal;
    sgfx::point pos_{0, 0};
    int lifes = 3;

    bool space_released = true;

    static constexpr auto key_color = sgfx::color::rgb_color{0xcb, 0x48, 0xb7};
};

#endif
