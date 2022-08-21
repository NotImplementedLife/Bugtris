#pragma once

#include "levels/level_ota.hpp"
#include "levels/level2.hpp"

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
					reinterpret_cast<Scene*>(self)->close()->next(new Level2());
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
		first_full_line_action.set_level(this);
		goal_reached_action.set_level(this);		
		set_goal(5);				
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
		if(value>=get_goal()) 
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