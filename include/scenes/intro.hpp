#pragma once

#include <Astralbrew>

using Astralbrew::World::Scene;

class Intro : public Scene
{
private:
	bool ready = false;
	
	VramManager vram_obj = VramManager(0x06014000, 0x4000);
public:	
	virtual void init() override;
	
	virtual void frame() override;
	
	virtual void on_key_down(int keys) override;	
	
	~Intro();
};
