#include "collision_manager.hpp"

collision_manager::handle collision_manager::register_collider(collider& new_collider)
{
	colliders_.push_front(&new_collider);
	return {this,std::begin(colliders_)};
}

void collision_manager::handle_collisions()
{
	for(auto it0=std::begin(colliders_); it0!=std::end(colliders_); ++it0)
	{
		for(auto it1=std::next(it0); it1!=std::end(colliders_); ++it1)
		{
			if(overlap((*it0)->bounds(),(*it1)->bounds()))
			{
				(*it0)->hit();
				(*it1)->hit();
			}
		}
	}
}
