#ifndef QUADTREE_H
#define QUADTREE_H

#include <sgfx/primitives.hpp>

#include <cstddef>

template <typename T, std::size_t max_depth>
class quadtree
{
	public:
	class node_iterator;
	class iterator;
	class iteration_sentinel{};
	
	explicit quadtree(sgfx::rectangle area);
	
	iterator insert(const T& value);
	
	iterator begin() const;
	iteration_sentinel end() const;
	
	node_iterator node_begin() const;
	
	auto nodes() const
	{
		struct
		{
			auto begin() const { return parent_->node_begin(); }
			auto end() const { return parent_->end(); }
			
			const quadtree* parent_;
		} ret_val{this};
		
		return ret_val;
	}
	
	private:
	/*Missing*/
};

#include "quadtree_impl.hpp"

#endif
