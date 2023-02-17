#pragma once

#include "scenes/level.hpp"

#include "level4.hpp"

class Level3 : public Level
{
public:
	Level3() : Level(3, &Instantiator::instantiate<Level3>, &Instantiator::instantiate<Level4>) { }
	
	void init() override
	{
		Level::init();
		hide_speed_panel();		
		set_goal(3);		
		move_direction = -1;
				
		lines_cleared.add_event(&Level3::on_lines_cleared, this);
		score_changed.add_event(&Level3::on_score_changed, this);
		key_down.add_event(&Level3::on_keys_down, this);
		
		resources_loaded.add_event(&Level3::on_resources_loaded, this);		
	}
	
	void on_resources_loaded(void*, void*)
	{
		show_dialog(&level_start_dialog);
	}
	
	~Level3()
	{		
		lines_cleared.remove_event(&Level3::on_lines_cleared, this);
		score_changed.remove_event(&Level3::on_score_changed, this);
		resources_loaded.remove_event(&Level3::on_resources_loaded, this);
		key_down.remove_event(&Level3::on_keys_down, this);
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
	
	int keys_count = 0;
	int keys_down_count = 0;
	void on_keys_down(void*, void* _keys)
	{
		int keys = (int)_keys;
		keys_count += ((keys & (Keys::Left | Keys::Right))!=0);
		keys_down_count += ((keys & Keys::Down)!=0);
		if(keys_count==7)
			some_moves_performed_ota.execute();
		if(keys_down_count==3)
			down_moves_performed_ota.execute();			
	}
		
	DialogLine level_start_dialog =  bob_says("Huh. You want the pieces to fall faster ah? Here you are.\n"
											 "Press DOWN and don't waste your time anymore.");
	DialogLine some_moves_dialog = bob_says("Huh???\n\nThe controls are broken...\nThat's it, you need to finish your test anyway though.\n");
	DialogLine down_moves_dialog = cappu_says("Up to the heavens haha!");
	DialogLine level_complete_dialog = bob_says("Enough for today. I'll make some mentenance... I guess.", &Level::next_level_handler);
	
	OneTimeAction some_moves_performed_ota { &Level3::on_some_moves_performed, this};
	OneTimeAction down_moves_performed_ota { &Level3::on_down_moves_performed, this};
	OneTimeAction goal_reached_ota    { &Level3::on_level_completed , this};
	
	void on_level_completed(void*, void*)
	{
		vblank_skip(60);
		show_dialog(&level_complete_dialog);
	}
	
	void on_some_moves_performed(void*, void*) { show_dialog(&some_moves_dialog); }
	void on_down_moves_performed(void*, void*) { show_dialog(&down_moves_dialog); }
};