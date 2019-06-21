#include "asteroid.hpp"
#include "game.hpp"
#include "player.hpp"
#include "scrolling_bg.hpp"
#include "spawn_point.hpp"

#include <sgfx/key.hpp>
#include <sgfx/primitives.hpp>

#include <chrono>

int main(int argc, char* argv[])
{
    using namespace sgfx;
	using std::ref;

	auto rm = resource_manager{};
    auto g = game{};

    g.spawn<scrolling_bg>(rm.rle("img/bg.rle"));
    g.spawn<player>(ref(rm), "img/ship", player::key_config{sgfx::key::left, sgfx::key::right, sgfx::key::up,
														    sgfx::key::down, sgfx::key::space});
    g.spawn(make_spawn_point(
        {{0, 0}, {100, 100}},
        [&](auto proxy, auto pos) { proxy.template spawn<asteroid>(rm.rle("img/asteroid.rle"), pos); },
        std::chrono::seconds{3}));
    g.run();

    return 0;
}
