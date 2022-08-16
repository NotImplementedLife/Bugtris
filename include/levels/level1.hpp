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
			level->set_score(1);
			level->show_dialog("Cappuccino","Gotcha");
		}
	};
	
	FirstFullLineAction first_full_line_action;
	
	Level1()
	{
		first_full_line_action.set_level(this);		
	}
	
	virtual void on_full_lines_count(int value) override
	{		
		if(value==1)
		{			
			first_full_line_action.execute();
		}
	}
	
	~Level1() 
	{
		
	}
};