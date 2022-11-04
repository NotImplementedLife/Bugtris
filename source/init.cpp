#include "scenes/title.hpp"

#include "levels/level1.hpp"
#include "scenes/intro.hpp"
#include "scenes/gba_jam_scene.hpp"


/*class _SplashScreen : public Astralbrew::Scenes::DefaultSplashScreen 
{
	inline void on_end() override {
		auto* com = this->close();		
		Scene* scene = new GBAJamScene();
		com->next(scene);
	}
};

void Astralbrew::World::init_main_scene() 
{		
	//Astralbrew::World::__MAIN_SCENE__ = new _SplashScreen(); 			
	Astralbrew::World::__MAIN_SCENE__ = new GBAJamScene();
}*/

astralbrew_launch_with_splash(GBAJamScene);
