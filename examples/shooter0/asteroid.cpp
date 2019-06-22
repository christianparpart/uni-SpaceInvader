#include "asteroid.hpp"

asteroid::asteroid(game_proxy proxy, std::string const& image, sgfx::point pos)
    : autoregister_collider{proxy}, img_{sgfx::load_rle(image)}, pos_{pos}
{
}

game_object::status asteroid::update(game_proxy proxy, std::chrono::milliseconds delta)
{
    pos_ += vel_;

    const auto center_offset = sgfx::vec{img_.width(), img_.height()} / 2;
    const auto area = proxy.area();
    if ((pos_.x - center_offset.x > area.top_left.x + area.size.w && vel_.x > 0)
        || (pos_.x + center_offset.x < area.top_left.x && vel_.x < 0)
        || (pos_.y - center_offset.y > area.top_left.y + area.size.h && vel_.y > 0)
        || (pos_.y + center_offset.y < area.top_left.y && vel_.y < 0))
        return game_object::status::dead;

    return current_status_;
}

void asteroid::draw(sgfx::canvas_view target) const
{
    const auto center_offset = sgfx::vec{img_.width(), img_.height()} / 2;
    sgfx::draw(target, img_, pos_ - center_offset, key_color);
}
