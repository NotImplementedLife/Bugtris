#pragma once

#include <Astralbrew>
#include "scenes/level.hpp"

class LevelOTA : public Astralbrew::World::OneTimeAction
{
public:
	Level* level = nullptr;
public:
	void set_level(Level* level);
	
	virtual void action() = 0;
	
	inline Level* get_level() { return level; }

	~LevelOTA() = default;
};