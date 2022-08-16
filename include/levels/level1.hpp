#pragma once

#include "scenes/board.hpp"

class Level1 : public Board
{
public:	
	class FirstFullLineAction : public Astralbrew::World::OneTimeAction
	{
	private:
		Level1* level = nullptr;
	public:
		void set_level(Level1* level) {this->level=level;}
		
		virtual void action() override
		{
			level->blank_skip(60);			
			level->show_dialog("Cappuccino","It's a full line! It should disappear.\nWhy is it not working?\nHmmm...");
		}
	};
	
	class GoalReachedAction : public Astralbrew::World::OneTimeAction
	{
	private:
		Level1* level = nullptr;
	public:
		void set_level(Level1* level) {this->level=level;}
		
		virtual void action() override
		{
			level->blank_skip(60);			
			level->show_dialog("Cappuccino", "Alright, that's enough. I gathered all the data I needed.",
				[](void* self)
				{
					((Level1*)self)->close(nullptr);
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
	}
	
	virtual void on_full_lines_count(int value) override
	{		
		set_score(value);
		if(value==1)
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