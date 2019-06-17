#include <sgfx/canvas.hpp>
#include <sgfx/color.hpp>
#include <sgfx/key.hpp>
#include <sgfx/image.hpp>
#include <sgfx/primitives.hpp>
#include <sgfx/window.hpp>

#include <chrono>
#include <iomanip>
#include <sstream>

namespace
{
	auto compute_filename(const std::string& basename)
	{
		using namespace std::chrono;
		auto now=system_clock::now();
		auto now_c=system_clock::to_time_t(now);
		std::stringstream strm;
		strm<<std::put_time(std::localtime(&now_c),"-%Y-%m-%d_%H-%M-%S");
		return basename+strm.str()+".ppm";
	}
}

int main(int argc, char* argv[])
{
	using namespace sgfx;
	
	window main_window{640,480};
	
	auto bg_img=load_ppm("sample_bg.ppm");
	point bg_pos{0,0};
	int scroll_speed=2;

	auto fg_img=load_ppm("sample_fg.ppm");
	point fg_pos{main_window.width()/2-fg_img.width()/2,main_window.height()/2-fg_img.height()/2};
	
	bool zero_released=true;
	while(main_window.handle_events() && !main_window.should_close())
	{
		if(main_window.is_pressed(key_id{'w'}))
			fg_pos.y-=1;
		if(main_window.is_pressed(key_id{'s'}))
			fg_pos.y+=1;
			
		if(main_window.is_pressed(key_id{'a'}))
			fg_pos.x-=1;
		if(main_window.is_pressed(key_id{'d'}))
			fg_pos.x+=1;
			
		if(main_window.is_pressed(key::escape))
			break;
		
		if(main_window.is_pressed(key_id{'0'}))
		{
			if(zero_released)
			{
				save_ppm(main_window,compute_filename("screenshot"));
				zero_released=false;
			}
		}
		else
			zero_released=true;
		
		bg_pos.y=(bg_pos.y+scroll_speed)%bg_img.height();
		
		draw(main_window,bg_img,bg_pos);
		draw(main_window,bg_img,bg_pos-vec{0,bg_img.height()});
		
		draw(main_window,fg_img,fg_pos,color::rgb_color{0xcb,0x48,0xb7});
		
		main_window.show();
	};

	return 0;
}
