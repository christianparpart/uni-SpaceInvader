#ifndef ASTEROID_H
#define ASTEROID_H

#include "collision_manager.hpp"
#include "game.hpp"

#include <sgfx/image.hpp>
#include <sgfx/primitives.hpp>

#include <chrono>
#include <memory>
#include <string>

class asteroid final : public game_object, autoregister_collider {
  public:
    asteroid(game_proxy proxy, sgfx::rle_image const& image, sgfx::point pos);

    void accept(game_object_visitor& visitor) override;

    game_object::status update(game_proxy proxy, std::chrono::milliseconds delta) override;

    sgfx::rectangle bounds() const override
    {
        return {pos_ - sgfx::vec{img_.width() / 2, img_.height() / 2}, {img_.width(), img_.height()}};
    }

    void hit() override { current_status_ = game_object::status::dead; }

    sgfx::rle_image const& img() const noexcept { return img_; }
    sgfx::point pos() const noexcept { return pos_; }
    sgfx::vec vel() const noexcept { return vel_; }
    game_object::status current_status() const noexcept { return current_status_; }

    static constexpr auto key_color = sgfx::color::rgb_color{0xcb, 0x48, 0xb7};

  private:
    sgfx::rle_image const& img_;
    sgfx::point pos_;
    sgfx::vec vel_{1, 2};

    game_object::status current_status_ = game_object::status::alive;
};

#endif
