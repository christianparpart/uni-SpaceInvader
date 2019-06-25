#include "player.hpp"
#include "game.hpp"  // resource_manager (TODO: put into its own file, even though it's just a 5-liner...)
#include "game_object_visitor.hpp"

#include <algorithm>

player::player(game_proxy proxy, ::resource_manager& rm, const std::string& name, const key_config& keys)
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

