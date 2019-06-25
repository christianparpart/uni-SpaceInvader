#include "game_object_visitor.hpp"
#include "game.hpp"
#include "asteroid.hpp"
#include "bullet.hpp"
#include "player.hpp"
#include "scrolling_bg.hpp"
#include "spawn_point.hpp"

// ---------------------------------------------------------------------------------
// updating

// TODO

// ---------------------------------------------------------------------------------
// drawing

void game_object_drawer::draw(sgfx::canvas_view view, game_object& obj)
{
    auto drawer = game_object_drawer{view};
    obj.accept(drawer);
}

void game_object_drawer::visit(asteroid& obj)
{
    auto const center_offset = sgfx::vec{obj.img().width(), obj.img().height()} / 2;
    sgfx::draw(target_, obj.img(), obj.pos() - center_offset, asteroid::key_color);
}

void game_object_drawer::visit(bullet& obj)
{
    auto const center_offset = sgfx::vec{obj.img().width(), obj.img().height()} / 2;
    sgfx::draw(target_, obj.img(), obj.pos() - center_offset, bullet::key_color);
}

void game_object_drawer::visit(player& obj)
{
    auto const& current_img = obj.img(obj.current_status());
    auto const center_offset = sgfx::vec{current_img.width(), current_img.height()} / 2;
    sgfx::draw(target_, current_img, obj.pos() - center_offset, player::key_color);

    auto life_rect = sgfx::rectangle{{10, 10}, {10, 50}};
    for (int i = 0; i < obj.lifes(); ++i, life_rect.top_left.x += 15)
        sgfx::fill(target_, life_rect, sgfx::color::red);
}

void game_object_drawer::visit(scrolling_bg& obj)
{
	sgfx::draw(target_, obj.img(), obj.pos());
	sgfx::draw(target_, obj.img(), obj.pos() - sgfx::vec{0, obj.img().height()});
}

void game_object_drawer::visit(spawn_point& /*obj*/)
{
	// no-op
}

