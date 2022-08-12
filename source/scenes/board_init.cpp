#include "scenes/board.hpp"

using namespace Astralbrew;
using namespace Astralbrew::Drawing;
using namespace Astralbrew::Memory;
using namespace Astralbrew::Video;

#include "board-bg.h"
#include "piece_tiles.h"
#include "dialog-bg.h"
#include "dialog-frame.h"

void Board::init_board_table()
{
	Address transparent_tile;
	vram_chr_1.reserve(&transparent_tile, 64);
	vram_chr_1.reserve(&board_bg_addr, board_bgTilesLen);
	vram_chr_1.reserve(&piece_bg_addr, piece_tilesTilesLen);		
	
	bgInit(3, Text256x256, Pal8bit, 1, 0);
	bgWrap(3);
	
	piece_tiles_start_id = ((int)piece_bg_addr.get_value() - (int)bgGetTilesPtr(3))/64;
	
	dmaCopy(board_bgMap, bgGetMapPtr(3), board_bgMapLen);
	copyTiles256(board_bgTiles, board_bg_addr.get_value(), board_bgTilesLen, board_bgPal, board_bgPalLen, 1);
		
	for(int i=0;i<32;i++) bgGetTilesPtr(3)[i]=0x3F3F; // black color that is not on transparent index
	
	copyTiles256(piece_tilesTiles, piece_bg_addr.get_value(), piece_tilesTilesLen, piece_tilesPal, piece_tilesPalLen, 64);
	
	bgSetScroll(3, 8, 48);
}

void Board::init_dialog_bg()
{		
	vram_chr_3.reserve(&dialog_bg_addr, dialog_bgTilesLen);
	
	bgInit(1, Text256x256, Pal4bit, 3, 2);
	
	dmaCopy(dialog_bgTiles, dialog_bg_addr.get_value(), dialog_bgTilesLen);
	dmaCopy(dialog_bgMap, bgGetMapPtr(1)+32*11, dialog_bgMapLen);
	dmaCopy(dialog_bgPal, BG_PALETTE_BANK(8), dialog_bgPalLen);	
}

void Board::init_dialog_fg()
{	
	vram_chr_2.reserve(&frame_bg_addr, dialog_frameTilesLen);
	vram_chr_2.reserve(&dialog_txt_addr, tiles_size_8bpp(4*23));		
	vram_chr_2.reserve(&dialog_title_addr, tiles_size_8bpp(2*7));
	vram_chr_2.reserve(&char_pic_addr, tiles_size_8bpp(4*4));		
	
	bgInit(0, Text256x256, Pal4bit, 2, 1);
	
	dmaCopy(dialog_frameTiles, frame_bg_addr.get_value(), dialog_frameTilesLen);
	dmaCopy(dialog_frameMap, bgGetMapPtr(0)+32*11, dialog_frameMapLen);
	dmaCopy(dialog_framePal, BG_PALETTE_BANK(7), dialog_framePalLen);
	
	vwf_title.set_render_space(dialog_title_addr.get_value(),2,12);
	VwfEngine::prepare_map(vwf_title, bgGetMapPtr(0), 32, 1, 12, 0x9);	
	
	vwf_body.set_render_space(dialog_txt_addr.get_value(), 4,23);
	VwfEngine::prepare_map(vwf_body, bgGetMapPtr(0), 32, 6, 15, 0x9);
	
	BG_PALETTE[0x9E] = RGB5(31,31, 7);
	BG_PALETTE[0x9F] = Colors::Yellow;
	
	int character_tid = bgTileId(char_pic_addr.get_value(), 0);	
	
	for(int y=0;y<4;y++)
	{
		for(int x=0;x<4;x++)
		{
			bgGetMapPtr(0)[32*(15+y)+1+x]=0xA000 | (character_tid++);
		}
	}
}
