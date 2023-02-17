#include "scenes/intro.hpp"
#include "scenes/title.hpp"

#include "intro_bg_cats.asset.h"
#include "intro_dialog_bg.asset.h"

void IntroScene::init()
{
	GenericScene::init();		
	dialog_fg_ptr = reserve_sprite_vram(Measure()._4bpp().tiles(2*28));		
	Hardware::SpritePalette[0] = Colors::White;		
	for(int i=0;i<DIALOG_TILES_WIDTH;i++)
	{
		short* frame_offset = (short*)(((int)dialog_fg_ptr) + Measure()._4bpp().tiles(2*i));			
		
		dialog_fg_frames[i] = new ObjFrame(frame_offset, 4);
		Sprite* column = create_sprite(SIZE_8x16, dialog_fg_frames[i], {8+8*(i),4});
		column->set_palette_number(vwf_color>>4);
	}	
	for(int i=0;i<(DIALOG_TILES_WIDTH+3)/4;i++)
	{
		create_sprite(SIZE_32x16, new ObjFrame(&ROA_intro_dialog_bg, 0, 0), {8+32*i,4})->set_mode_semi_transparent();
	}

	vwf.set_render_space(dialog_fg_ptr, 2, DIALOG_TILES_WIDTH);
	
	set_intro_bg_palette_effect(new PaletteFadeFromColorEffect(Hardware::BackgroundPalette, Colors::Black, get_shadow_background_palette(), 10));
			
	Video::bgSetAlpha(4, 8, 2, 6); // ??
	
	show_dialog(&dialog_lines[0]);
}

void IntroScene::change_background(void*, void*)
{
	replace_bitmap(&ROA_intro_bg_cats);
}

void IntroScene::end_scene(void*, void*)
{
	close()->next(new TitleScene());
}

void IntroScene::show_dialog(DialogLine* line) { dialog_stream = line; }

bool IntroScene::process_dialog()
{
	if(!dialog_stream) 
		return false;

	vwf.clear(Video::Pal4bit);
	int count = vwf.put_text(dialog_stream->message, Video::Pal4bit, SolidColorBrush(vwf_color&0xF));
	dialog_stream->message+=count;
	
	if(count==0)
	{			
		if(dialog_stream->line_finished_handler != nullptr)
		{
			(this->*(dialog_stream->line_finished_handler))(this, nullptr);
		}
		dialog_stream = dialog_stream->next;
		return false;
	}
	else
	{
		//for(int i=0;i<10;i++) force_wait_vblank();
		wait_for_key_pressed(Keys::A | Keys::Start);
		return true;
	}
	return true;
}	

void IntroScene::frame()
{
	process_dialog();
	GenericScene::frame();			
}

IntroScene::~IntroScene()
{		
	release_sprite_vram(dialog_fg_ptr);		
	set_outro_bg_palette_effect(new PaletteFadeToColorEffect(Hardware::BackgroundPalette, Colors::Black, 10));
}