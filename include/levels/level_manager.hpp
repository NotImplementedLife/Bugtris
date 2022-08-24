#pragma once

#include "scenes/level.hpp"

class LevelManager
{
public:
	static Level* generate(int id);
};