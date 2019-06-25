#include "bullet.hpp"
#include "game_object_visitor.hpp"

bullet::bullet(game_proxy proxy, sgfx::rle_image const& img, sgfx::point pos)
    : autoregister_collider{proxy}, img_{img}, pos_{pos}
{
}

void bullet::accept(game_object_visitor& visitor)
{
    visitor.visit(*this);
}
