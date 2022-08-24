#include "levels/level_manager.hpp"
#include "levels/level1.hpp"
#include "levels/level2.hpp"
#include "levels/level3.hpp"
#include "levels/level4.hpp"
#include "levels/level5.hpp"

Level* LevelManager::generate(int id)
{
	switch(id)
	{
		case 1: return new Level1();
		case 2: return new Level2();
		case 3: return new Level3();
		case 4: return new Level4();
		case 5: return new Level5();
		
		
		default: return nullptr;
	}
}