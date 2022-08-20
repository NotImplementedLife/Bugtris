#pragma once

#include <AstralbrewLib>
#include "scenes/board.hpp"

class LevelOTA : public Astralbrew::World::OneTimeAction
{
public:
	Board* level = nullptr;
public:
	void set_level(Board* level);
	
	virtual void action() = 0;

	~LevelOTA() = default;
};