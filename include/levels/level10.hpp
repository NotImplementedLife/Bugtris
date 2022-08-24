#pragma once

#include "levels/level_ota.hpp"


class Level10 : public Level
{
public:	
	
	class GoalReachedAction : public LevelOTA
	{
		virtual void action() override
		{
			level->blank_skip(60);
			level->show_dialog("Cappuccino", "Bad news!\nWe've fallen to a cyberattack that somehow"
				"messed up with the speed panel! ",
				[](void* self)
				{
					((Level*)self)->next_level();
					FATAL_ERROR("Entrypoint jump missed");
				});
		}
	};
	
	int keys_count = 0;
	int keys_down_count = 0;	
	
	GoalReachedAction goal_reached_action;
	
	Level9() : Level(9) { }	
	
	virtual void init() override
	{		
		Level::init();						
		goal_reached_action.set_level(this);						
		set_goal(7);
	}
	
	virtual void on_level_start() override
	{		
		show_dialog("Bob", "Huh. One of our teamworkers was talking about a strange graphical glitch that "
					"showed up these days. Let's see what is this about.");		
	}

	virtual void on_score_changed(int old_value) override
	{
		if(get_score()>=get_goal()) 
		{
			goal_reached_action.execute();
		}		
	}
	
	virtual void on_lines_cleared(int value) override
	{
		inc_score(value);
		board_mesh.set_all_values(mk_block(get_piece_generator()->rand_color(), get_piece_generator()->rand_shape()));
	}
	
	~Level9() { }
};