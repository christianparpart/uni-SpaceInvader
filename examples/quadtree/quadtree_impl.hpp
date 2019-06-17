/*
 * Never include this file directly! It is included by quadtree.hpp and
 * exists only in order to properly split interface and implementation
 * despite the template.
*/

#include <array>
#include <memory>
#include <optional>

/*
 * This can be used to iterate over the nodes of a quadtree.
 * Dereferencing(i.e. *) gives access to the elements stored in this node as well as the area covered by it
*/
template <typename T, std::size_t max_depth>
class quadtree<T, max_depth>::node_iterator
{
	public:
	node_iterator(sgfx::rectangle area, const quadtree<T,max_depth>::node_t* node)
	{
		/*do something*/
	}
	
	auto& operator++()
	{
		//Move to the next node
		return *this;
	}
	
	auto operator++(int)
	{
		auto tmp=*this;
		operator++();
		return tmp;
	}
	
	struct node_ref
	{
		sgfx::rectangle area() const { /*return the area coverd by the current node*/ }
		auto& elements() const { /*return something that can be used to iterate over the elements contained in the current node*/ }
		
		//store some pointer/reference to the current node
	};
	
	node_ref operator*() const
	{
		//construct and return a node_ref granting access to the node pointed to by this iterator
	}
	
	const node_ref* operator->() const
	{
		//same as above, but return a pointer
		//ensure the pointed to object lives at least until this iterator is incremented or destroyed
	}
		
	private:
	/*I guess we might need some data here?*/
	
	friend node_iterator subtree_iterator(const node_iterator&)
	{
		//return a node_iterator pointing to the same node as the given one
		//however: the returned iterator should iterate only over the current node and all its children
		//so instead of returning to the current_nodes parent, its end should be reached
	}
	
	friend bool operator==(const node_iterator& iter, iteration_sentinel) noexcept
	{
		//return true if iter was incremented beyond its valid range (has reached its end)
		//false otherwise
		
		//The iterator has to keep some form of stack or queue to facilitate its tree traversal.
		//In its simplest version, this provides a simple way to check for the end without having to compare with some invalid value:
		//if the traversal stack/queue is empty, the end was reached
	}
	
	friend bool operator!=(const node_iterator& iter, iteration_sentinel sentinel) noexcept
	{
		return !(iter==sentinel);
	}
};

/*
 * This can be used to iterate over the values stored in a quadtree
*/
template <typename T, std::size_t max_depth>
class quadtree<T, max_depth>::iterator
{
	public:
	explicit iterator(node_iterator node_iter)
	{
		//construct an iterator useful for iterating over all elements of node_iter and all elements in children thereof
	}
	
	auto& operator++()
	{
		/*missing*/
		return *this;
	}
	
	auto operator++(int)
	{
		auto tmp=*this;
		operator++();
		return tmp;
	}
	
	const T& operator*() const
	{
		//return the element being pointed to
	}
	
	const T* operator->() const
	{
		//as above, but return a pointer
	}
	
	private:
	node_iterator node_iter_;
	/*some more data?*/

	friend auto subtree(const iterator& iter) noexcept
	{
		struct
		{
			auto begin() const { return iter_; }
			auto end() const { return iteration_sentinel{}; }
			
			iterator iter_;
		} ret_val{iterator{subtree_iterator(node_iter_)}};
		
		return ret_val;
	}
	
	friend bool operator==(const iterator& iter, iteration_sentinel) noexcept
	{
		//return true if iter was incremented beyond its valid range (has reached its end)
		//false otherwise
	}
	
	friend bool operator!=(const iterator& iter, iteration_sentinel sentinel) noexcept
	{
		return !(iter==sentinel);
	}
};

template <typename T, std::size_t max_depth>
quadtree<T, max_depth>::quadtree(sgfx::rectangle area)
{
	/*Do something*/
}

template <typename T, std::size_t max_depth>
typename quadtree<T, max_depth>::iterator quadtree<T, max_depth>::insert(const T& value)
{
	//Do something reasonable. That is:
	//Recursivley subdivide and extend the tree until we find the smallest subarea into which value fits and insert into this node
	//Return an iterator to the newly inserted element
	//(This involves computing an AABB for value. Provide a customization point of your choice to do so)
}

template <typename T, std::size_t max_depth>
typename quadtree<T, max_depth>::iterator quadtree<T, max_depth>::begin() const
{
	//Return an iterator to the first element stored in this quadtree
}

template <typename T, std::size_t max_depth>
typename quadtree<T, max_depth>::iteration_sentinel quadtree<T, max_depth>::end() const
{
	return {}
}

template <typename T, std::size_t max_depth>
typename quadtree<T, max_depth>::node_iterator quadtree<T, max_depth>::node_begin() const
{
	//Return a node_iterator to the root_node
}
