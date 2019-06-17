#ifndef SGFX_PRIMITIVE_TYPES_H
#define SGFX_PRIMITIVE_TYPES_H

#include <cstdint>

namespace sgfx
{
	struct vec
	{
		int x, y;
		
		constexpr auto& operator+=(vec v)
		{
			x+=v.x;
			y+=v.y;
			return *this;
		}
		
		constexpr auto& operator*=(int v)
		{
			x*=v;
			y*=v;
			return *this;
		}
		
		constexpr auto& operator/=(int v)
		{
			x/=v;
			y/=v;
			return *this;
		}
	};
	
	constexpr vec operator*(int i, vec v)
	{
		return v*=i;
	}
	
	constexpr vec operator*(vec v, int i)
	{
		return v*=i;
	}
	
	constexpr vec operator/(vec v, int i)
	{
		return v/=i;
	}

	struct point
	{
		int x, y;
		
		constexpr auto& operator+=(vec v)
		{
			x+=v.x;
			y+=v.y;
			return *this;
		}
		
		constexpr auto& operator-=(vec v)
		{
			x-=v.x;
			y-=v.y;
			return *this;
		}
	};
	
	constexpr point operator+(point lhs, const vec& rhs)
	{
		lhs+=rhs;
		return lhs;
	}
	
	constexpr point operator-(point lhs, const vec& rhs)
	{
		lhs-=rhs;
		return lhs;
	}
	
	constexpr vec operator-(const point& lhs, const point& rhs)
	{
		return {lhs.x-rhs.x,lhs.y-rhs.y};
	}
	
	constexpr bool operator==(const point& lhs, const point& rhs)
	{
		return lhs.x==rhs.x && lhs.y==rhs.y;
	}
	
	constexpr bool operator!=(const point& lhs, const point& rhs)
	{
		return !(lhs==rhs);
	}
	
	struct dimension
	{
		std::uint16_t w, h;
	};
	
	struct rectangle
	{
		point top_left;
		dimension size;
		
		constexpr bool contains(const rectangle& other) const
		{
			if(other.top_left.x<top_left.x) return false;
			if(other.top_left.x+other.size.w>top_left.x+size.w) return false;
			if(other.top_left.y<top_left.y) return false;
			if(other.top_left.y+other.size.h>top_left.y+size.h) return false;
			
			return true;
		}
	};
	
	constexpr bool overlap(const rectangle& lhs, const rectangle& rhs)
	{
		if(lhs.top_left.x+lhs.size.w<rhs.top_left.x) return false;
		if(rhs.top_left.x+rhs.size.w<lhs.top_left.x) return false;
		if(lhs.top_left.y+lhs.size.h<rhs.top_left.y) return false;
		if(rhs.top_left.y+rhs.size.h<lhs.top_left.y) return false;
		
		return true;
	}
}

#endif
