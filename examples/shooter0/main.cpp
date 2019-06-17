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
	
	game g();
	
	g.spawn<scrolling_bg>("img/bg.rle");
	g.spawn<player>("img/ship",player::key_config{sgfx::key::left,sgfx::key::right,sgfx::key::up,sgfx::key::down, sgfx::key::space});
	g.spawn(make_spawn_point({{0,0},{100,100}},[](auto proxy, auto pos)
		{
			proxy.template spawn<asteroid>("img/asteroid.rle",pos);
		},std::chrono::seconds{3}));
	g.run();
	
	
	
	return 0;
}
