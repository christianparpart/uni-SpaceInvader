#ifndef BULLET_H
#define BULLET_H

#include "collision_manager.hpp"
#include "game.hpp"

#include <sgfx/image.hpp>
#include <sgfx/primitives.hpp>

#include <chrono>
#include <memory>
#include <string>

class bullet final : public game_object, autoregister_collider {
  public:
    bullet(game_proxy proxy, sgfx::rle_image const& img, sgfx::point pos);

    void accept(game_object_visitor&) override;

    sgfx::rectangle bounds() const override
    {
        return {pos_ - sgfx::vec{img_.width() / 2, img_.height() / 2}, {img_.width(), img_.height()}};
    }

    void hit() override { remaining_lifetime_ = std::chrono::seconds{0}; }

    sgfx::rle_image const& img() const noexcept { return img_; }
    sgfx::point pos() const noexcept { return pos_; }
    sgfx::vec vel() const noexcept { return vel_; }
    std::chrono::milliseconds remaining_lifetime() const noexcept { return remaining_lifetime_; }

	// modifier
    sgfx::point& pos() noexcept { return pos_; }
    std::chrono::milliseconds& remaining_lifetime() noexcept { return remaining_lifetime_; }

    static constexpr auto key_color = sgfx::color::rgb_color{0xcb, 0x48, 0xb7};

  private:
    sgfx::rle_image img_;
    sgfx::point pos_{0, 0};
    sgfx::vec vel_{0, -2};
    std::chrono::milliseconds remaining_lifetime_ = std::chrono::seconds{2};
};

#endif
