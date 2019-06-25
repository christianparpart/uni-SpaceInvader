#include "asteroid.hpp"
#include "game_object_visitor.hpp"

asteroid::asteroid(game_proxy proxy, sgfx::rle_image const& image, sgfx::point pos)
    : autoregister_collider{proxy}, img_{image}, pos_{pos}
{
}

void asteroid::accept(game_object_visitor& visitor)
{
    visitor.visit(*this);
}

