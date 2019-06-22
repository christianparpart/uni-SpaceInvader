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
    bullet(game_proxy proxy, std::string const& img, sgfx::point pos);

    game_object::status update(game_proxy proxy, std::chrono::milliseconds delta) override;
    void draw(sgfx::canvas_view target) const override;

    sgfx::rectangle bounds() const override
    {
        return {pos_ - sgfx::vec{img_.width() / 2, img_.height() / 2}, {img_.width(), img_.height()}};
    }

    void hit() override { remaining_lifetime_ = std::chrono::seconds{0}; }

  private:
    sgfx::rle_image img_;
    sgfx::point pos_{0, 0};
    sgfx::vec vel_{0, -2};
    std::chrono::milliseconds remaining_lifetime_ = std::chrono::seconds{2};

    static constexpr auto key_color = sgfx::color::rgb_color{0xcb, 0x48, 0xb7};
};

#endif
