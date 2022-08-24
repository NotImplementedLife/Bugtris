#pragma once

#include "levels/level_ota.hpp"


class Level7 : public Level
{
public:	
	inline static int attempts_count = 0;
	
	Level7() : Level(7) { }	
	
	inline static const u8 predef_pieces[] = 
	{
		G_O, G_T, G_Z, G_T, G_I, G_J, G_O, G_J,
		G_L, G_S, G_T, G_J, G_T, G_Z, G_L, G_O, 
		G_J, G_J, G_L			
	};
	
	int crt_piece = 0;
	
	class PGen : public PieceGenerator
	{			
		Level7* level;
	public:
		PGen(Level7* level) : level(level) { }
		
		virtual Piece next() const override
		{						
			return { predef_pieces[level->crt_piece++], rand_color(), rand_shape() };
		}
	};
	
	class GoalReachedAction : public LevelOTA
	{
		virtual void action() override
		{
			level->blank_skip(60);
			level->show_dialog("Cappuccino", "I have to admit, that was a tough puzzle. Congratulations!",
				[](void* self)
				{					
					((Level*)self)->next_level();
					FATAL_ERROR("Entrypoint jump missed");
				});
		}
	};
	
	class PiecesConsumedAction : public LevelOTA
	{
		virtual void action() override
		{
			Level7::attempts_count++;			
			level->show_dialog("Cappuccino", "Ooops, Looks like we're out of pieces.",
				[](void* self)
				{
					if(attempts_count<3)
					{
						((Level*)self)->show_dialog("Cappuccino", "Let's try it once again.",
						[](void* self)
						{
							((Level*)self)->close()->next(new Level7());
						});
					}
					else
					{
						Level7::attempts_count=0;
						((Level*)self)->show_dialog("Cappuccino", "Well, this test really is hard, though Bob claims "
							"it is \"managable\". I guess you can skip it.",
							[](void* self)
							{
								((Level*)self)->next_level();
								FATAL_ERROR("Entrypoint jump missed");
							});
					}
				});
		}
	};
	
	int keys_count = 0;
	int keys_down_count = 0;
	
	GoalReachedAction goal_reached_action;
	PiecesConsumedAction piece_consumed_action;
	
	virtual void init() override
	{		
		Level::init();				
		goal_reached_action.set_level(this);						
		piece_consumed_action.set_level(this);						
		set_goal(7);
		set_piece_generator(new PGen(this));
		set_speed(25);
	}	
	
	virtual void frame() override
	{
		Level::frame();
		if(crt_piece == sizeof(predef_pieces)+2)
		{			
			piece_consumed_action.execute();
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
	
	~Level7() 
	{
		delete get_piece_generator();
	}
};