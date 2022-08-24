#pragma once

#include "levels/level_ota.hpp"

class Level3 : public Level
{
public:	
	class SomeMovesPerfrmedAction : public LevelOTA
	{		
		virtual void action() override
		{
			level->blank_skip(60);	
			level->show_dialog("Bob", "Huh???\n\nThe controls are broken...\nThat's it, you need to finish your test anyway though.\n");							
		}
	};
	
	class DownMovesPerfrmedAction : public LevelOTA
	{		
		virtual void action() override
		{
			level->blank_skip(60);	
			level->show_dialog("Cappuccino", "Up to the heavens haha!");
		}
	};
	
	class GoalReachedAction : public LevelOTA
	{
		virtual void action() override
		{
			level->blank_skip(60);
			level->show_dialog("Bob", "Enough for today. I'll make some mentenance... I guess.",
				[](void* self)
				{
					((Level*)self)->next_level();
					FATAL_ERROR("Entrypoint jump missed");
				});
		}
	};
	
	int keys_count = 0;
	int keys_down_count = 0;
	SomeMovesPerfrmedAction some_moves_performed_action;
	
	GoalReachedAction goal_reached_action;
	
	DownMovesPerfrmedAction down_moves_performed_action;
	
	Level3() : Level(3) { }	
	
	virtual void init() override
	{		
		Level::init();		
		hide_speed_panel();
		some_moves_performed_action.set_level(this);
		goal_reached_action.set_level(this);
		down_moves_performed_action.set_level(this);		
		move_direction = -1;		
		set_goal(3);
	}
	
	virtual void on_level_start() override
	{		
		show_dialog("Bob", "Huh. You want the pieces to fall faster ah? Here you are.\n"
			"Press DOWN and don't waste your time anymore.");
	}

	virtual void on_score_changed(int old_value) override
	{
		if(get_score()>=get_goal()) 
		{
			goal_reached_action.execute();
		}		
	}
	
	virtual void on_key_down(int keys) override
	{
		keys_count += ((keys & (KEY_LEFT | KEY_RIGHT | KEY_DOWN))!=0);
		keys_down_count += ((keys & KEY_DOWN)!=0);
		if(keys_count==7)
			some_moves_performed_action.execute();
		if(keys_down_count==3)
		{
			down_moves_performed_action.execute();			
		}
		Level::on_key_down(keys);
	}	
	
	virtual void on_lines_cleared(int value) override
	{
		inc_score(value);
	}
	
	~Level3() { }
};