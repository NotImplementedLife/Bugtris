#pragma once

#include "levels/level_ota.hpp"


class Level6 : public Level
{
public:	
	class PGen : public PieceGenerator
	{
		virtual Piece next() const override
		{
			return { G_O, rand_color(), rand_shape() };
		}
	};
	
	class GoalReachedAction : public LevelOTA
	{
		virtual void action() override
		{
			level->blank_skip(60);
			level->show_dialog("Cappuccino", "Something wrong with the RNG?! We need to look into that.",
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
	
	Level6() : Level(6) { }	
	
	virtual void init() override
	{		
		Level::init();				
		goal_reached_action.set_level(this);						
		set_goal(8);
		set_piece_generator(new PGen());
	}	

	virtual void on_score_changed(int old_value) override
	{
		if(get_score()>=get_goal()) 
		{
			goal_reached_action.execute();
		}			
		set_speed(get_speed()-3);
	}
	
	virtual void on_lines_cleared(int value) override
	{
		inc_score(value);
	}
	
	~Level6() 
	{
		delete get_piece_generator();
	}
};