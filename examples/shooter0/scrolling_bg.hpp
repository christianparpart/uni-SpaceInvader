#ifndef SCROLLING_BG_H
#define SCROLLING_BG_H

#include "game.hpp"

#include <sgfx/image.hpp>
#include <sgfx/primitives.hpp>

#include <chrono>
#include <memory>
#include <string>

class scrolling_bg final : public game_object {
  public:
    scrolling_bg(game_proxy proxy, const std::string& filename) : img_{sgfx::load_rle(filename)} {}

    game_object::status update(game_proxy proxy, std::chrono::milliseconds delta) override
    {
        pos_.y = (pos_.y + scroll_speed_) % img_.height();

        return game_object::status::alive;
    }

    void draw(sgfx::canvas_view target) const
    {
        sgfx::draw(target, img_, pos_);
        sgfx::draw(target, img_, pos_ - sgfx::vec{0, img_.height()});
    }

  private:
    sgfx::rle_image img_;
    sgfx::point pos_{0, 0};
    int scroll_speed_ = 2;
};

#endif
