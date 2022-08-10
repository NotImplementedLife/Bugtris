#include "scenes/board.hpp"

using namespace Astralbrew;
using namespace Astralbrew::Memory;
using namespace Astralbrew::Video;

#include "board-bg.h"
#include "piece_tiles.h"
#include "dialog-bg.h"
#include "dialog-frame.h"

#include <stdlib.h>

void Board::draw_mesh(const Mesh& mesh)
{
	if(mesh.width()==0 || mesh.height()==0)
		return;
	u16* map = ((u16*)MAP_BASE_ADR(0)) + 7*32 + 11;
	for(int y=mesh.y();y<mesh.y()+mesh.height();y++)
	{
		for(int x=mesh.x();x<mesh.x()+mesh.width();x++)
		{
			if(0<=x && x<10 && 0<=y && y<18)
			{
				int val = mesh.coord_at(x,y);
				
				if(val!=0)
				{
					map[32*y+x]=(val&0x7F)+piece_tiles_start_id;
				}
			}
		}
	}
}

static const u8 mesh_gfx[5][16] = 
{
	{
		0,1,0,0,
		0,1,0,0,
		0,1,0,0,
		0,1,0,0,
	},
	{
		0,0,0,0,
		0,1,1,0,
		0,1,1,0,
		0,0,0,0,
	},
	{
		0,0,0,0,
		1,1,0,0,
		0,1,1,0,
		0,0,0,0,
	},
	{
		0,1,1,0,
		0,1,0,0,
		0,1,0,0,
		0,0,0,0,
	},
	{
		0,1,1,0,
		0,0,1,0,
		0,0,1,0,
		0,0,0,0,
	},
};

void Board::spawn_mesh(int gid, int color, int shape)
{	
	user_controllable_mesh = new Mesh(0,0,4,4,mesh_gfx[gid]);
	user_controllable_mesh->replace(1, mk_block(color, shape));
}

void Board::clear()
{
	u16* map = ((u16*)MAP_BASE_ADR(0)) + 7*32 + 11;
	const register int zero=0;
	for(int y=0;y<18;y++)
	{		
		for(int x=0;x<10;x++)
		{			
			__asm("STRH %[_0], [%[dest_map]]"				
				:
				: [dest_map] "r" (map+32*y+x), [_0] "r" (zero));
		}
	}
}

bool Board::ucm_in_bounds()
{
	int x = user_controllable_mesh->x();
	int y = user_controllable_mesh->y();
	int w = user_controllable_mesh->width();
	int h = user_controllable_mesh->height();
	for(int iy=y;iy<y+h;iy++)			
		for(int ix=x;ix<x+w;ix++)			
			if(ix<0 || iy<0 || ix>=10 || iy>=18)
			{
				if(user_controllable_mesh->coord_at(ix,iy))
					return false;
			}
			else if(user_controllable_mesh->coord_at(ix,iy) && board_mesh.coord_at(ix,iy))
			{
				return false;
			}
		
	return true;
}

void Board::init()
{	
	Address transparent_tile;
	vram_chr_1.reserve(&transparent_tile, 64);
	//vram_chr_2.reserve(&transparent_tile, 32);
	vram_chr_3.reserve(&transparent_tile, 32);
	
	vram_chr_1.reserve(&board_bg_addr, board_bgTilesLen);
	vram_chr_1.reserve(&piece_bg_addr, piece_tilesTilesLen);		
	
	vram_chr_2.reserve(&frame_bg_addr, dialog_frameTilesLen);
	vram_chr_2.reserve(&dialog_txt_addr, tiles_size_8bpp(4*23));	
	
	//Address dialog_bg_addr;	

	Video::setMode(0);
	bgInit(3, Text256x256, Pal8bit, 1, 0);
	bgWrap(3);
	
	piece_tiles_start_id = ((int)piece_bg_addr.get_value() - (int)bgGetTilesPtr(3))/64;
	
	dmaCopy(board_bgMap, bgGetMapPtr(3), board_bgMapLen);
	copyTiles256(board_bgTiles, board_bg_addr.get_value(), board_bgTilesLen, board_bgPal, board_bgPalLen, 1);
		
	for(int i=0;i<32;i++) bgGetTilesPtr(3)[i]=0x3F3F; // black color that is not on transparent index
	
	copyTiles256(piece_tilesTiles, piece_bg_addr.get_value(), piece_tilesTilesLen, piece_tilesPal, piece_tilesPalLen, 64);
	
	bgSetScroll(3, 8, 48);
	
	bgInit(0, Text256x256, Pal4bit, 2, 1);
	bgInit(1, Text256x256, Pal4bit, 3, 2);
	
	dmaCopy(dialog_bgTiles, bgGetTilesPtr(1), dialog_bgTilesLen);
	dmaCopy(dialog_bgMap, bgGetMapPtr(1)+32*14, dialog_bgMapLen);
	dmaCopy(dialog_bgPal, BG_PALETTE_BANK(8), dialog_bgPalLen);
	
	dmaCopy(dialog_frameTiles, frame_bg_addr.get_value(), dialog_frameTilesLen);
	dmaCopy(dialog_frameMap, bgGetMapPtr(0)+32*11, dialog_frameMapLen);
	dmaCopy(dialog_framePal, BG_PALETTE_BANK(7), dialog_framePalLen);
	
	vwf.set_render_space(dialog_txt_addr.get_value(),4,23);
	VwfEngine::prepare_map(vwf, bgGetMapPtr(0), 32, 6, 15);
			
	vwf.put_char('A', Pal4bit);
	vwf.put_char('B', Pal4bit);
	vwf.put_char('C', Pal4bit);
	
	bgSetAlpha(1, 6, 3, 8);
	
	bgUpdate();
		
	spawn_mesh(0,1,1);
	/*meshes[0].set_x(5);
	meshes[0].set_y(10);*/
	
	//spawn_mesh(2,5,10);	
}

int k=0;
void Board::frame()
{
	//greenSwapSwitch();	
	clear();
	draw_mesh(board_mesh);
	for(int i=0;i<meshes.size();i++)
		draw_mesh(meshes[i]);
	draw_mesh(*user_controllable_mesh);
	
	if(user_controllable_mesh && k==0)
	{				
		user_controllable_mesh->move(0,1);
		if(!ucm_in_bounds())
		{			
			user_controllable_mesh->move(0,-1);
			board_mesh+=*user_controllable_mesh;
			//if(board_mesh.coord_at(0,0)!=0)
				//FATAL_ERROR("Something happened");
			delete user_controllable_mesh;
			user_controllable_mesh = nullptr;
		}
		
			
		if(!user_controllable_mesh)
		{
			spawn_mesh(rand()%5, rand()%8, rand()%4);
		}
	}
	k++;
	if(k==10) k=0;
	//while(1) VBlankIntrWait();
}

void Board::on_key_down(int keys)
{
	if(keys & KEY_LEFT)
	{
		if(user_controllable_mesh)
		{			
			user_controllable_mesh->move(-1,0);
			if(!ucm_in_bounds())
			{
				user_controllable_mesh->move(+1,0);
			}
		}
	}	
	else if(keys & KEY_RIGHT)
	{
		if(user_controllable_mesh)
		{			
			user_controllable_mesh->move(+1,0);
			if(!ucm_in_bounds())
			{
				user_controllable_mesh->move(-1,0);
			}
		}
	}
	if(keys & KEY_DOWN)
	{
		if(user_controllable_mesh)
		{			
			user_controllable_mesh->move(0,1);
			if(!ucm_in_bounds())
			{
				user_controllable_mesh->move(0,-1);
			}
		}
	}
	else if(keys & KEY_UP)
	{
		if(user_controllable_mesh)
		{			
			user_controllable_mesh->move(0,-1);
			if(!ucm_in_bounds())
			{
				user_controllable_mesh->move(0,1);
			}
		}
	}
}

Board::~Board()
{
	
}