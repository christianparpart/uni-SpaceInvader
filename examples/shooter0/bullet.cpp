#include "bullet.hpp"

bullet::bullet(game_proxy proxy, const std::string& img, sgfx::point pos)
    : autoregister_collider{proxy}, img_{sgfx::load_rle(img)}, pos_{pos}
{
}

game_object::status bullet::update(game_proxy proxy, std::chrono::milliseconds delta)
{
    if (delta >= remaining_lifetime_)
        return game_object::status::dead;

    pos_ += vel_;

    remaining_lifetime_ -= delta;
    return game_object::status::alive;
}

void bullet::draw(sgfx::canvas_view target) const
{
    const auto center_offset = sgfx::vec{img_.width(), img_.height()} / 2;
    sgfx::draw(target, img_, pos_ - center_offset, key_color);
}
