#include "game_object_visitor.hpp"
#include "asteroid.hpp"
#include "bullet.hpp"
#include "game.hpp"
#include "player.hpp"
#include "scrolling_bg.hpp"
#include "spawn_point.hpp"

// ---------------------------------------------------------------------------------
// updating

game_object::status game_object_updater::update(game_proxy proxy, std::chrono::milliseconds delta,
                                                game_object& obj)
{
    auto updater = game_object_updater{proxy, delta};
    obj.accept(updater);
    return updater.status_;
}

void game_object_updater::visit(asteroid& obj)
{
    obj.pos() += obj.vel();

    const auto center_offset = sgfx::vec{obj.img().width(), obj.img().height()} / 2;
    const auto area = proxy_.area();

    if ((obj.pos().x - center_offset.x > area.top_left.x + area.size.w && obj.vel().x > 0)
        || (obj.pos().x + center_offset.x < area.top_left.x && obj.vel().x < 0)
        || (obj.pos().y - center_offset.y > area.top_left.y + area.size.h && obj.vel().y > 0)
        || (obj.pos().y + center_offset.y < area.top_left.y && obj.vel().y < 0))
        status_ = game_object::status::dead;
    else
        status_ = obj.current_status();
}

void game_object_updater::visit(bullet& obj)
{
    if (delta_ >= obj.remaining_lifetime())
        status_ = game_object::status::dead;
    else
    {
        obj.pos() += obj.vel();
        obj.remaining_lifetime() -= delta_;

        status_ = game_object::status::alive;
    }
}

void game_object_updater::visit(player& obj)
{
    obj.current_status() = player::state::normal;
    if (proxy_.is_pressed(obj.keys().left))
    {
        --obj.pos().x;
        obj.current_status() = player::state::flying_left;
    }
    if (proxy_.is_pressed(obj.keys().right))
    {
        ++obj.pos().x;
        obj.current_status() = player::state::flying_right;
    }
    if (proxy_.is_pressed(obj.keys().up))
        --obj.pos().y;
    if (proxy_.is_pressed(obj.keys().down))
        ++obj.pos().y;

    if (proxy_.is_pressed(obj.keys().shoot) && obj.space_released())
    {
        obj.space_released() = false;
        proxy_.spawn<bullet>(obj.get_resource_manager().rle("img/missile.rle"), obj.pos() - sgfx::vec{0, 50});
    }
    else if (!proxy_.is_pressed(obj.keys().shoot))
    {
        obj.space_released() = true;
    }

    const auto& current_img = obj.img(obj.current_status());
    const auto center_offset = sgfx::vec{current_img.width(), current_img.height()} / 2;
    const auto area = proxy_.area();
    obj.pos().x = std::clamp(obj.pos().x, area.top_left.x + center_offset.x,
							 area.top_left.x + area.size.w - center_offset.x);
    obj.pos().y = std::clamp(obj.pos().y, area.top_left.y + center_offset.y,
							 area.top_left.y + area.size.h - center_offset.y);

    status_ = obj.lifes() > 0 ? game_object::status::alive : game_object::status::dead;
}

void game_object_updater::visit(scrolling_bg& obj)
{
	obj.pos().y = (obj.pos().y + obj.scroll_speed()) % obj.img().height();

	status_ = game_object::status::alive;
}

void game_object_updater::visit(spawn_point& obj)
{
    auto delta = delta_;
	while (obj.time_remaining() <= delta)
	{
		auto const pos = sgfx::point{obj.area().top_left.x + random_uniform_int(obj.area().size.w),
									 obj.area().top_left.y + random_uniform_int(obj.area().size.h)};
		obj(proxy_, pos);
		delta -= obj.time_remaining();
		obj.time_remaining() = obj.delay();
	}
	obj.time_remaining() -= delta;

	status_ = game_object::status::alive;
}

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
