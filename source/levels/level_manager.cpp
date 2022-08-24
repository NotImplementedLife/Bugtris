#include "levels/level_manager.hpp"
#include "levels/level1.hpp"
#include "levels/level2.hpp"
#include "levels/level3.hpp"
#include "levels/level4.hpp"
#include "levels/level5.hpp"
#include "levels/level6.hpp"
#include "levels/level7.hpp"
#include "levels/level8.hpp"

Level* LevelManager::generate(int id)
{
	switch(id)
	{
		case 1: return new Level1();
		case 2: return new Level2();
		case 3: return new Level3();
		case 4: return new Level4();
		case 5: return new Level5();
		case 6: return new Level6();
		case 7: return new Level7();
		case 8: return new Level8();
		
		
		default: return nullptr;
	}
}