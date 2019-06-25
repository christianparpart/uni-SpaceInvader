#include "player.hpp"
#include "game.hpp"  // resource_manager (TODO: put into its own file, even though it's just a 5-liner...)
#include "game_object_visitor.hpp"

#include <algorithm>

player::player(game_proxy proxy, resource_manager& rm, const std::string& name, const key_config& keys)
    : autoregister_collider{proxy}, resource_manager_{rm}, imgs_{&rm.rle(name + ".rle"),
                                                                 &rm.rle(name + "_left.rle"),
                                                                 &rm.rle(name + "_right.rle")},
      keys_{keys}, pos_{proxy.area().top_left + sgfx::vec{proxy.area().size.w / 2, proxy.area().size.h}}
{
}

void player::accept(game_object_visitor& visitor)
{
    visitor.visit(*this);
}

game_object::status player::update(game_proxy proxy, std::chrono::milliseconds delta)
{
    current_status_ = state::normal;
    if (proxy.is_pressed(keys_.left))
    {
        --pos_.x;
        current_status_ = state::flying_left;
    }
    if (proxy.is_pressed(keys_.right))
    {
        ++pos_.x;
        current_status_ = state::flying_right;
    }
    if (proxy.is_pressed(keys_.up))
        --pos_.y;
    if (proxy.is_pressed(keys_.down))
        ++pos_.y;

    if (proxy.is_pressed(keys_.shoot) && space_released_)
    {
        space_released_ = false;
        proxy.spawn<bullet>(resource_manager_.rle("img/missile.rle"), pos_ - sgfx::vec{0, 50});
    }
    else if (!proxy.is_pressed(keys_.shoot))
    {
        space_released_ = true;
    }

    const auto& current_img = *imgs_[static_cast<int>(current_status_)];
    const auto center_offset = sgfx::vec{current_img.width(), current_img.height()} / 2;
    const auto area = proxy.area();
    pos_.x = std::clamp(pos_.x, area.top_left.x + center_offset.x,
                        area.top_left.x + area.size.w - center_offset.x);
    pos_.y = std::clamp(pos_.y, area.top_left.y + center_offset.y,
                        area.top_left.y + area.size.h - center_offset.y);

    return lifes() > 0 ? game_object::status::alive : game_object::status::dead;
}

