#pragma once

#include "levels/level_ota.hpp"
#include <stdlib.h>


class Level10 : public Level
{
public:	
	
	class GoalReachedAction : public LevelOTA
	{
		virtual void action() override
		{
			level->blank_skip(60);
			level->show_dialog("Bob", "Well done!",
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
	
	Level10() : Level(10) { }	
	
	virtual void init() override
	{		
		Level::init();						
		goal_reached_action.set_level(this);						
		set_goal(4);
	}
	
	virtual void on_level_start() override
	{		
		show_dialog("Bob", "Bad news!\nWe've fallen to a cyberattack that somehow "
				"messed up with the speed panel!");		
	}

	virtual void on_score_changed(int old_value) override
	{
		if(get_score()>=get_goal())
		{
			goal_reached_action.execute();
		}		
	}
	
	virtual void on_piece_placed() override
	{
		if(rand()&1)
		{
			if(get_speed()<30)
			{
				set_speed(get_speed()+3);				
			}
		}
		else
		{
			if(get_speed()>3)
			{
				set_speed(get_speed()-3);				
			}
		}
	}
	
	virtual void on_lines_cleared(int value) override
	{
		inc_score(value);		
	}
	
	~Level10() { }
};


