#pragma once

#include "levels/level_ota.hpp"

class Level2 : public Board
{
public:	
	class FirstFullLineAction : public LevelOTA
	{		
		virtual void action() override
		{
			level->blank_skip(60);			
			level->show_dialog("Cappuccino", "WORKS!!!");
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
	
	Level2()
	{		
		first_full_line_action.set_level(this);
		goal_reached_action.set_level(this);
	}	
	
	virtual void init() override
	{
		Board::init();
		clear_lines = true;
		set_goal(5);
		show_dialog("Cappuccino", "I think I've repaired it.");
	}

	virtual void on_score_changed(int old_value) override
	{
		if(get_score()>=get_goal()) 
		{
			goal_reached_action.execute();
		}		
	}
	
	virtual void on_full_lines_count(int value) override
	{		
		set_score(value);
		if(value>=1)
		{			
			first_full_line_action.execute();
		}		
	}
	
	~Level2() { }
};