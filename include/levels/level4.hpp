#pragma once

#include "scenes/level.hpp"
#include "level5.hpp"

class Level4 : public Level
{
public:
	Level4() : Level(4, &Instantiator::instantiate<Level4>, &Instantiator::instantiate<Level5>) { }
	
	void init() override
	{
		Level::init();
		hide_speed_panel();		
		set_goal(7);				
		
		level_start.add_event(&Level4::on_level_start, this);
		
		lines_cleared.add_event(&Level4::on_lines_cleared, this);
		score_changed.add_event(&Level4::on_score_changed, this);				
	}
	
	void on_level_start(void*, void*)
	{
		show_dialog(&start_dialog[0]);
	}
	
	DialogLine start_dialog[2] = 
	{
		bob_says("I worked hard all night. Everything seems to be fixed now.", &start_dialog[1]),
		cappu_says("Finally! Let's hope for a normal Tetris game!"),
	};
	
	DialogLine end_dialog = bob_says("Hope you enjoyed it. Huh", &Level::next_level_handler);
	
	~Level4()
	{		
		level_start.remove_event(&Level4::on_level_start, this);
	
		lines_cleared.remove_event(&Level4::on_lines_cleared, this);
		score_changed.remove_event(&Level4::on_score_changed, this);		
	}
	
	void on_score_changed(void*, void*)
	{
		if(get_score()>=get_goal())		
			goal_reached_ota.execute();
	}
	
	void on_lines_cleared(void* sender, void* _value)
	{
		inc_score((int)_value);
	}
		
	OneTimeAction goal_reached_ota    { &Level4::on_level_completed , this};
	
	void on_level_completed(void*, void*)
	{
		vblank_skip(60);		
		show_dialog(&end_dialog);
	}
	
	
};