#include "scenes/board.hpp"
#include "cats_pics.hpp"

using namespace Astralbrew::Drawing;
using namespace Astralbrew::Video;

void Board::show_dialog(const char* actor_name, const char* message)
{	
	bgShow(0);
	bgShow(1);	
	
	set_dialog_character(CatsPics[actor_name].tiles, CatsPics[actor_name].palette);
	
	vwf_title.put_text(actor_name, Pal4bit, SolidColorBrush(0xE));
	
	dialog_stream = message;
}

void Board::hide_dialog()
{
	bgHide(0);
	bgHide(1);
}

void Board::process_dialog()
{
	if(dialog_stream) {
		vwf_body.clear(Pal4bit);
		int count = vwf_body.put_text(dialog_stream, Pal4bit, SolidColorBrush(0xF));		
		dialog_stream+=count;		
		
		if(count==0) 
		{
			dialog_stream=nullptr;		
			hide_dialog();
		}
		else
		{
			wait_for_key_pressed(KEY_A);
		}
	}		
}

void Board::set_dialog_character(const void* tiles, const void* palette)
{
	if(tiles==nullptr)
	{
		for(int i=0;i<16;i++) BG_PALETTE[0xA0+i]=0;
		return;
	}
	dmaCopy(palette, BG_PALETTE_BANK(0xA), 32);
	dmaCopy(tiles, char_pic_addr.get_value(), 4*4*32);
}