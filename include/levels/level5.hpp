#pragma once

#include "levels/level_ota.hpp"


class Level5 : public Level
{
public:	
	
	class GoalReachedAction : public LevelOTA
	{
		virtual void action() override
		{
			level->blank_skip(60);
			level->show_dialog("Cappuccino", "Great work!",
				[](void* self)
				{					
					((Scene*)self)->close()->next(nullptr);
					FATAL_ERROR("Entrypoint jump missed");						
				});
		}
	};
	
	int keys_count = 0;
	int keys_down_count = 0;	
	
	GoalReachedAction goal_reached_action;
	
	Level5() : Level(5) { }	
	
	virtual void init() override
	{		
		Level::init();				
		goal_reached_action.set_level(this);		
		clear_lines = true;
		mesh_spawn_x = 3;
		set_goal(7);
	}
	
	virtual void on_level_start() override
	{		
		show_dialog("Cappuccino", "Now let's add some dynamism to the game. I'm introducing you to the new speed panel",
		[](void* sender)
		{
			for(int i=1;i<60;i++)
			{
				if(i&1)
				{
					((Level*)sender)->set_speed(1+i/2);
				}
				VBlankIntrWait();
			}
			((Level*)sender)->show_dialog("Cappuccino", "It shows how fast the pieces fall in this game.\nGood luck!");
		});
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
	
	~Level5() { }
};