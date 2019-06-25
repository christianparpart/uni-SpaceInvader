#ifndef SCROLLING_BG_H
#define SCROLLING_BG_H

#include "game.hpp"
#include "game_object_visitor.hpp"

#include <sgfx/image.hpp>
#include <sgfx/primitives.hpp>

#include <chrono>
#include <memory>
#include <string>

class scrolling_bg final : public game_object {
  public:
    scrolling_bg(game_proxy proxy, sgfx::rle_image const& image) : img_{image} {}

    void accept(game_object_visitor& visitor) { visitor.visit(*this); }

    sgfx::rle_image const& img() const noexcept { return img_; }
    sgfx::point pos() const noexcept { return pos_; }
    int scroll_speed() const noexcept { return scroll_speed_; }

    sgfx::point& pos() noexcept { return pos_; }

  private:
    sgfx::rle_image const& img_;
    sgfx::point pos_{0, 0};
    int const scroll_speed_{2};
};

#endif
