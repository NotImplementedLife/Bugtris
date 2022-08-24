#pragma once

#include "levels/level_ota.hpp"

class Level2 : public Level
{
public:	
	class FirstFullLineAction : public LevelOTA
	{		
		virtual void action() override
		{
			level->blank_skip(60);			
			level->show_dialog("Cappuccino", "Oh yeahhh! It works now");
		}
	};
	
	class GoalReachedAction : public LevelOTA
	{	
		virtual void action() override
		{
			level->blank_skip(60);
			level->show_dialog("Cappuccino", "That's great!\n\nNow let me introduce you to your next partner.",
				[](void* self)
				{	
					((Level*)self)->blank_skip(5);
					((Level*)self)->show_dialog("Bob", "Huh?\n\nOh hi! I'm Bob. I like ice cream and sleeping.\n\n"
						"Huh? About the project? Oh yeah, the project... I want to show a pleasant feature. Huh.",
						[](void* self)
						{							
							((Level*)self)->blank_skip(5);
							((Level*)self)->show_dialog("Cappuccino","Isn't that great news?\n\n Let's go!", [](void* self)
							{
								((Level*)self)->blank_skip(10);
								((Level*)self)->next_level();
								FATAL_ERROR("Entrypoint jump missed");
							});
						});
				});			
		}
	};	
	
	FirstFullLineAction first_full_line_action;
	
	GoalReachedAction goal_reached_action;
	
	Level2() : Level(2) { }	
	
	virtual void init() override
	{		
		Level::init();		
		hide_speed_panel();
		first_full_line_action.set_level(this);
		goal_reached_action.set_level(this);		
		set_goal(5);		
		ffw_enabled = false;		
	}
	
	virtual void on_level_start() override
	{		
		show_dialog("Cappuccino", "I think I've repaired it.");
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
		if(get_score() >= 1)
		{			
			first_full_line_action.execute();
		}		
	}
	
	~Level2() { }
};