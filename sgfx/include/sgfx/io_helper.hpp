#ifndef SGFX_IO_HELPER_H
#define SGFX_IO_HELPER_H

#include <iostream>
#include <type_traits>

#include <cstdint>

namespace sgfx
{
	template <typename T, typename std::enable_if<(std::is_integral<T>::value)>::type* =nullptr>
	void write_le(std::ostream &out, const T& val)
	{
		std::uint8_t byte_val;
		std::make_unsigned_t<T> unsigned_value=val;
		for(std::size_t i=0;i<sizeof(T);++i)
		{
			byte_val=static_cast<std::uint8_t>(unsigned_value & 0xFF);
			out.write(reinterpret_cast<char*>(&byte_val),sizeof(byte_val)); //waaah, a reinterpret_cast ;_;;
			unsigned_value>>=8;
		}
	}

	template <typename T, typename std::enable_if<(std::is_integral<T>::value)>::type* =nullptr>
	T read_le(std::istream& in)
	{
		T return_value=0;
		
		std::uint8_t byte_val=0;
		std::make_unsigned_t<T> unsigned_value=0;
		for(std::size_t i=0;i<sizeof(T);++i)
		{
			unsigned_value>>=8;
			in.read(reinterpret_cast<char*>(&byte_val),sizeof(std::uint8_t));
			unsigned_value+=static_cast<decltype(unsigned_value)>(byte_val)<<((sizeof(T)-1)*8);
			
		}
		return_value=unsigned_value<std::numeric_limits<T>::max()?
			static_cast<T>(unsigned_value):
			static_cast<T>(unsigned_value-std::numeric_limits<T>::min()) + std::numeric_limits<T>::min();
		
		return return_value;
	}
}

#endif
