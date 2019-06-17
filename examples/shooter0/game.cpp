#include "game.hpp"

game::game():
	wnd_{640,480,"Space!"}
{}

void game::run()
{
	while(wnd_.handle_events() && !wnd_.should_close())
	{
		using namespace std::chrono;
		auto now=high_resolution_clock::now();
		leftover_time+=duration_cast<milliseconds>(now-last_time);
		last_time=now;
		
		while(leftover_time>=step_time)
		{
			collision_mgr_.handle_collisions();
			
			for(std::size_t i=0;i<objs_.size();++i)
			{
				const auto obj_state=objs_[i]->update(game_proxy{this}, step_time);
				if(obj_state==game_object::status::dead)
				{
					std::swap(objs_[i],objs_.back());
					objs_.pop_back();
					--i;
				}
			}
				
			leftover_time-=step_time;
		}
		
		for(const auto& obj:objs_)
			obj->draw(wnd_);
			
		wnd_.show();
	}
}

void game::spawn(game_object* new_obj)
{
	objs_.push_back(std::move(new_obj));
}
