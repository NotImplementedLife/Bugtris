#pragma once

#include "levels/level_ota.hpp"
#include "levels/level2.hpp"

class Level1 : public Board
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
					((Scene*)self)->close()->next(new Level2());
					FATAL_ERROR("Entrypoint jump missed");					
				});			
		}
	};
	
	FirstFullLineAction first_full_line_action;
	
	GoalReachedAction goal_reached_action;
	
	Level1()
	{		
		first_full_line_action.set_level(this);
		goal_reached_action.set_level(this);
	}	
	
	virtual void init() override
	{
		Board::init();
		set_goal(5);
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
	
	~Level1() { }
};