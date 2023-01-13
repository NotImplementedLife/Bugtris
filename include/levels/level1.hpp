#pragma once

#include "scenes/level.hpp"

class Level1 : public Level
{
public:
	Level1() : Level(1, &Instantiator::instantiate<Level1>, nullptr) { }
	
	void init() override
	{
		Level::init();
		hide_speed_panel();		
		set_goal(5);		
		mesh_spawn_x = 0;
		clear_lines = false;		
		ffw_enabled = false;
		
		level_start.add_event(&Level1::on_level_start, this);
		full_lines_count.add_event(&Level1::on_full_lines_count, this);
		
	}
	
	
	~Level1()
	{
		level_start.remove_event(&Level1::on_level_start, this);
		full_lines_count.remove_event(&Level1::on_full_lines_count, this);
	}
	
	DialogLine level_start_dialog = { "Cappuccino", "Ok so let's test it!", nullptr, nullptr };
	DialogLine first_full_line_dialog = { "Cappuccino", "It's a full line! It should disappear though...\nWhy is it not working?\nHmmm...", nullptr, nullptr };
	
	
	void on_level_start(void*, void*) {	show_dialog(&level_start_dialog); }
	
	void on_first_full_line(void*, void*)
	{
		vblank_skip(60);
		show_dialog(&first_full_line_dialog);
	}
	OneTimeAction first_full_line_ota { &Level1::on_first_full_line, this };
	
	void on_full_lines_count(void* sender, void* _value)
	{
		int value = (int)_value;
		set_score(value);
		if(value>=1)
		{
			first_full_line_ota.execute();
		}
	}
};