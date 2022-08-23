#pragma once

#include "levels/level_ota.hpp"
#include "levels/level5.hpp"


class Level4 : public Level
{
public:	
	
	class GoalReachedAction : public LevelOTA
	{
		virtual void action() override
		{
			level->blank_skip(60);
			level->show_dialog("Cappuccino", "Excellent!",
				[](void* self)
				{
					((Level*)self)->show_dialog("Bob", "Hope you enjoyed it. Huh",
						[](void* self)
						{
							((Scene*)self)->close()->next(new Level5());
							FATAL_ERROR("Entrypoint jump missed");
						});
				});
		}
	};
	
	int keys_count = 0;
	int keys_down_count = 0;	
	
	GoalReachedAction goal_reached_action;
	
	Level4() : Level(4) { }	
	
	virtual void init() override
	{		
		Level::init();				
		hide_speed_panel();
		goal_reached_action.set_level(this);		
		clear_lines = true;
		mesh_spawn_x = 3;
		set_goal(7);
	}
	
	virtual void on_level_start() override
	{		
		show_dialog("Bob", "I worked hard all night. Everything seems to be fixed now.",
		[](void* sender)
		{
			((Level*)sender)->show_dialog("Cappuccino", "Finally! Let's hope for a normal Tetris game!");
		});
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
	
	~Level4() { }
};