#pragma once

#include "scenes/level.hpp"

#include "level3.hpp"

class Level2 : public Level
{
public:
	Level2() : Level(2, &Instantiator::instantiate<Level2>, &Instantiator::instantiate<Level3>) { }
	
	void init() override
	{
		Level::init();
		hide_speed_panel();		
		set_goal(5);		
		mesh_spawn_x = 0;		
		ffw_enabled = false;
				
		lines_cleared.add_event(&Level2::on_lines_cleared, this);
		score_changed.add_event(&Level2::on_score_changed, this);
	}
	
	~Level2()
	{		
		lines_cleared.remove_event(&Level2::on_lines_cleared, this);
		score_changed.remove_event(&Level2::on_score_changed, this);
	}
		
	DialogLine first_full_line_dialog = cappu_says("Oh yeahhh! It works now");
	DialogLine level_complete_dialog[3] = 
	{	
		cappu_says("That's great!\n\nNow let me introduce you to your next partner.", &level_complete_dialog[1]),
		bob_says("Huh?\n\nOh hi! I'm Bob. I like ice cream and sleeping.\n\n"
		         "Huh? About the project? Oh yeah, the project... I want to show a pleasant feature. Huh.", &level_complete_dialog[2]),
		cappu_says("Isn't that great news?\n\n Let's go!", &Level::next_level_handler)		 
	};
	
	void on_first_full_line(void*, void*)
	{
		vblank_skip(60);
		show_dialog(&first_full_line_dialog);
	}	
	
	void on_lines_cleared(void* sender, void* _value)
	{
		int value = (int)_value;
		inc_score(value);
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
	
	OneTimeAction first_full_line_ota { &Level2::on_first_full_line, this };
	OneTimeAction goal_reached_ota    { &Level2::on_level_completed , this};
	
	void on_level_completed(void*, void*)
	{
		vblank_skip(60);
		show_dialog(&level_complete_dialog[0]);
	}
};