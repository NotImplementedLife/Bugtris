#include "scenes/intro.hpp"
#include "scenes/title.hpp"

using namespace Astralbrew;
using namespace Astralbrew::Drawing;
using namespace Astralbrew::Video;
using namespace Astralbrew::Utils;
using namespace Astralbrew::Objects;

#include "intro-bg-galaxy.h"

void Intro::init()
{
	Video::setMode(3);
	objEnable1D();
	
	bgInit(2, Text256x256, Pal4bit, 0, 0);
	
	dmaCopy(intro_bg_galaxyBitmap, (void*)0x06000000, intro_bg_galaxyBitmapLen);
	
	Address transparent_tile;
	vram_obj.reserve(&transparent_tile, tiles_size_4bpp(1));
	
	vram_obj.reserve(&text_drawing_address, tiles_size_4bpp(2*28));
	vram_obj.reserve(&text_bg_address, tiles_size_4bpp(2));
	
	vwf.set_render_space((void*)0x06014020, 2, 28);
	vwf.clear(Pal4bit);
		
	show_dialog("Somewhere in a parallel universe, ...",
		[](void* self)
		{
			reinterpret_cast<Intro*>(self)->show_dialog("... a group of cats creates a Tetris game.",
			[](void* self)
			{
				reinterpret_cast<Intro*>(self)->close()->next(new Title());
			});
		});
		
	u16* _d_bg = (u16*)text_bg_address.get_value();
	for(int i=0;i<32;i++)
	{
		_d_bg[i] = 0x2222;
	}
	
	SPRITE_PALETTE[1] = Colors::White;
	SPRITE_PALETTE[2] = Colors::Black;	
		
	for(int i=0;i<28;i++)
	{
		dialog_blocks_addr[i] = text_drawing_address + tiles_size_4bpp(2*i);
		dialog_blocks[i] = Sprite::quick16(&dialog_blocks_addr[i], SIZE_8x16, ANCHOR_TOP_LEFT);
		dialog_blocks[i]->set_position(8+8*i, 10);
		dialog_blocks[i]->get_attribute()->set_priority(0);
		dialog_blocks[i]->update_position(nullptr);
		dialog_blocks[i]->update_visual();
		
		dialog_bg[i] = Sprite::quick16(&text_bg_address, SIZE_8x16, ANCHOR_TOP_LEFT);
		dialog_bg[i]->set_position(8+8*i, 10);
		dialog_bg[i]->get_attribute()->set_priority(1);
		dialog_bg[i]->update_position(nullptr);
		dialog_bg[i]->update_visual();
		dialog_blocks[i]->show_in_front_of(dialog_bg[i]);
		
		// OBJ Mode: Semi transparent
		// do the hard way since it's not yet implemented in Astralbrew
		u16* attr0 = (u16*)dialog_bg[i]->get_attribute();
		*attr0 |= 1<<10; 
	}
	
	// Obj over BG2 yields to different results on no$gba and VBA/mGBA.
	// Placing sprites below BG2 seems to solve the inconsistency.
	bgSetAlpha(2, 6, 4, 8);
}

void Intro::show_dialog(const char* message, void (*callback)(void*))
{
	new_dialog = true;
	dialog_stream = message;
	dialog_callback = callback;
}

bool Intro::process_dialog()
{
	if(!dialog_stream) 
		return false;
	
	new_dialog = false;
	vwf.clear(Pal4bit);
	int count = vwf.put_text(dialog_stream, Pal4bit, SolidColorBrush(0x1));
	dialog_stream+=count;		
	
	if(count==0) 
	{
		dialog_stream=nullptr;		
		//hide_dialog();
		if(dialog_callback)
		{				
			dialog_callback(this);	
			if(!new_dialog)
				dialog_callback = 0;				
		}		
		return false;
	}
	else
	{
		wait_for_key_pressed(KEY_A | KEY_START);
		return true;
	}
}

void Intro::frame()
{
	process_dialog();
	
	OamPool::deploy();
	if(ready)
	{
		close()->next(new Title());
	}	
}

void Intro::on_key_down(int keys)
{
	if(keys & KEY_START)
	{
		ready=true;
	}
}

Intro::~Intro()
{	
	for(int i=28;i--;)
	{
		delete dialog_blocks[i];
		delete dialog_bg[i];		
	}	
	OamPool::deploy();
	
	
	zeroize((void*)0x06000000, intro_bg_galaxyBitmapLen); // possibly faulty?
}