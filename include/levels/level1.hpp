#pragma once

#include "levels/level_ota.hpp"

class Level1 : public Level
{
public:	
	class FirstFullLineAction : public LevelOTA
	{		
		virtual void action() override
		{
			level->blank_skip(60);			
			level->show_dialog("Cappuccino", "It's a full line! It should disappear though...\nWhy is it not working?\nHmmm...");
		}
	};
	
	class GoalReachedAction : public LevelOTA
	{	
		virtual void action() override
		{
			level->blank_skip(60);
			level->show_dialog("Cappuccino", "Alright, that's enough. I gathered all the data I needed.",
				[](void* self)
				{					
					((Level*)self)->next_level();
					FATAL_ERROR("Entrypoint jump missed");					
				});			
		}
	};
	
	FirstFullLineAction first_full_line_action;
	
	GoalReachedAction goal_reached_action;
	
	Level1() : Level(1) { }	
	
	virtual void init() override
	{		
		Level::init();
		hide_speed_panel();
		first_full_line_action.set_level(this);
		goal_reached_action.set_level(this);		
		set_goal(5);		
		mesh_spawn_x = 0;
		clear_lines = false;		
		ffw_enabled = false;
	}
	
	virtual void on_level_start() override
	{		
		show_dialog("Cappuccino", "Ok so let's test it!");
	}
	
	virtual void on_full_lines_count(int value) override
	{		
		set_score(value);
		if(value>=1)
		{			
			first_full_line_action.execute();			
		}
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
	}
	
	~Level1() { }
};