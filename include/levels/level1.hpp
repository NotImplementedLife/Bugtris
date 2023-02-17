#pragma once

#include "scenes/level.hpp"

#include "level2.hpp"

class Level1 : public Level
{
public:
	Level1() : Level(1, &Instantiator::instantiate<Level1>, &Instantiator::instantiate<Level2>) { }
	
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
		score_changed.add_event(&Level1::on_score_changed, this);
	}
	
	~Level1()
	{
		level_start.remove_event(&Level1::on_level_start, this);
		full_lines_count.remove_event(&Level1::on_full_lines_count, this);
		score_changed.remove_event(&Level1::on_score_changed, this);
	}
	
	DialogLine level_start_dialog = cappu_says("Ok so let's test it!");
	DialogLine first_full_line_dialog = cappu_says("It's a full line! It should disappear though...\nWhy is it not working?\nHmmm...");
	DialogLine level_complete_dialog = cappu_says("Alright, that's enough. I gathered all the data I needed.", &Level::next_level_handler);
	
	void on_level_start(void*, void*) {	show_dialog(&level_start_dialog); }
	
	void on_first_full_line(void*, void*)
	{
		vblank_skip(60);
		show_dialog(&first_full_line_dialog);
	}	
	
	void on_full_lines_count(void* sender, void* _value)
	{
		int value = (int)_value;
		set_score(value);
		if(value>=1)
		{
			first_full_line_ota.execute();
		}
	}
	
	void on_score_changed(void*, void*)
	{
		if(get_score()>=get_goal()) 
		{
			goal_reached_ota.execute();
		}	
	}
	
	OneTimeAction first_full_line_ota { &Level1::on_first_full_line, this };
	OneTimeAction goal_reached_ota    { &Level1::on_level_completed , this};
	
	void on_level_completed(void*, void*)
	{
		vblank_skip(60);
		show_dialog(&level_complete_dialog);
	}
};