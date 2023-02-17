#pragma once

#include "normal_level.hpp"

class Level5 : public NormalLevel
{
public:
	Level5() : NormalLevel(5, &Instantiator::instantiate<Level5>, nullptr) { }
	
	void init() override
	{
		NormalLevel::init();		
		set_goal(7);				
		
		level_completed.add_event(&Level5::on_level_completed, this);
		level_start.add_event(&Level5::on_level_start, this);
		
		score_changed.add_event(&Level5::on_score_changed, this);
	}
	
	void on_score_changed(void*, void*) { set_speed(get_speed()-3); }	
		
	
	~Level5()
	{		
		level_completed.remove_event(&Level5::on_level_completed, this);
		level_start.remove_event(&Level5::on_level_start, this);
		score_changed.remove_event(&Level5::on_score_changed, this);
	}
		
	static void speed_bar_show_off(void* sender, void*)
	{
		for(int i=1;i<60;i++)
		{
			if(i&1)
			{
				((Level5*)sender)->set_speed(1+i/2);
			}
			((Level*)sender)->force_wait_vblank();
		}		
	}
	
	DialogLine start_dialog[2] =
	{
		cappu_says("Now let's add some dynamism to the game. I'm introducing you to the new speed panel", &Level5::speed_bar_show_off, &start_dialog[1]),
		cappu_says("It shows how fast the pieces fall in this game.\nGood luck!")
	};
	DialogLine end_dialog = cappu_says("Great work!", &Level::next_level_handler);
	
	void on_level_start(void*, void*) { show_dialog(&start_dialog[0]); }
	void on_level_completed(void*, void*) { show_dialog(&end_dialog); }
	
};