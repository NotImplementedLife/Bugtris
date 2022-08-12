#include "scenes/board.hpp"
#include "cats_pics.hpp"

using namespace Astralbrew::Drawing;
using namespace Astralbrew::Video;

void Board::show_dialog(const char* actor_name, const char* message)
{	
	set_dialog_character(CatsPics[actor_name].tiles, CatsPics[actor_name].palette);
	
	vwf_title.put_text(actor_name, Pal4bit, SolidColorBrush(0xE));
	vwf_body.put_text(message, Pal4bit, SolidColorBrush(0xF));
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