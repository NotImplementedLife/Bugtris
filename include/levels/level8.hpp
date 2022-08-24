#pragma once

#include "levels/level_ota.hpp"


class Level8 : public Level
{
public:	
	inline static int attempts_count = 0;
	
	Level8() : Level(8) { }			
	
	class PGen : public PieceGenerator
	{			
		Level8* level;
	public:
		PGen(Level8* level) : level(level) { }
		
		virtual Piece next() const override
		{						
			if(level->get_score()>0)
			{
				level->inc_score(-1);
				return { rand_piece(), rand_color(), rand_shape() };				
			}
			else 
			{
				level->goal_reached_action.execute();
				return { 0xFF, 0, 0 };
			}
			
		}
	};
	
	class GoalReachedAction : public LevelOTA
	{
		virtual void action() override
		{
			level->user_controllable_mesh = new Mesh(0,0,1,1);
			level->blank_skip(60);
			level->show_dialog("Cappuccino", "Nice job!",
				[](void* self)
				{					
					((Level*)self)->next_level();
					FATAL_ERROR("Entrypoint jump missed");
				});
		}
	};
	
	friend class GoalReachedAction;
	
	virtual void on_level_start() override
	{		
		show_dialog("Cappuccino", "Today we're trying to push the game to its limit. Your task is to use "
					"all the pieces that you've got.");
	}
	
	GoalReachedAction goal_reached_action;
	
	virtual void init() override
	{		
		Level::init();				
		goal_reached_action.set_level(this);										
		set_score(50);
		set_goal(0);
		set_piece_generator(new PGen(this));
		set_speed(25);
	}	
	
	~Level8() 
	{
		delete get_piece_generator();
	}
};