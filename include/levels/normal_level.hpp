#pragma once

#include "scenes/level.hpp"

class NormalLevel : public Level
{
public:
	NormalLevel(int id, Instantiator::FunctionType<Level> this_level_instantiator, Instantiator::FunctionType<Level> next_level_instantiator) 
		: Level(id, this_level_instantiator, next_level_instantiator) { }
	
	void init() override
	{
		Level::init();		
		
		lines_cleared.add_event(&NormalLevel::on_lines_cleared, this);
		score_changed.add_event(&NormalLevel::on_score_changed, this);				
	}	
		
	~NormalLevel()
	{				
		lines_cleared.remove_event(&NormalLevel::on_lines_cleared, this);
		score_changed.remove_event(&NormalLevel::on_score_changed, this);		
	}
	
	void on_score_changed(void*, void*)
	{
		if(get_score()>=get_goal())		
			goal_reached_ota.execute();
	}
	
	void on_lines_cleared(void* sender, void* _value)
	{
		inc_score((int)_value);
	}
		
	OneTimeAction goal_reached_ota    { &NormalLevel::on_level_completed , this};
	
	void on_level_completed(void*, void*)
	{
		vblank_skip(60);		
		level_completed.trigger(this, nullptr);
	}
	
	Event level_completed;
	
};