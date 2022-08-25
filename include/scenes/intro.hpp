#pragma once

#include <Astralbrew>

using Astralbrew::World::Scene;
using Astralbrew::Text::VwfEngine;
using Astralbrew::Memory::VramManager;
using Astralbrew::Memory::Address;
using Astralbrew::Entity::Sprite;

class Intro : public Scene
{
private:
	bool ready = false;	
	
	VramManager vram_obj = VramManager(0x06014000, 0x4000);
	
	Address text_drawing_address;
	Address text_bg_address;
	VwfEngine vwf = VwfEngine(Astralbrew::Resources::Fonts::default_8x16);
	Sprite* dialog_blocks[28];
	Address dialog_blocks_addr[28];
	Sprite* dialog_bg[28];
	
	bool new_dialog = false;
	const char* dialog_stream = nullptr;
	void (*dialog_callback)(void*) = 0;
	
	bool process_dialog();
public:	
	virtual void init() override;
	
	virtual void frame() override;
	
	virtual void on_key_down(int keys) override;	
	
	void show_dialog(const char* message, void (*callback)(void*) = 0);
	
	~Intro();
	
	inline static int hbl_cnt_crd=0;
	__attribute__((target("thumb"))) inline static void exit_hblank_effect()
	{				
		Astralbrew::Video::bgScroll(2,hbl_cnt_crd++,0);
		Astralbrew::Video::bgUpdate();
	}
};
