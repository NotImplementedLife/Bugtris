#include "scenes/board.hpp"

using namespace Astralbrew;
using namespace Astralbrew::Video;

#include "board-bg.h"
#include "piece_tiles.h"

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
					map[32*y+x]=(val&0x7F)+0xB8;
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
	Mesh mesh(0,0,4,4,mesh_gfx[gid]);
	mesh.replace(1, mk_block(color, shape));
	meshes.push_back(_move_(mesh));
	user_controllable_mesh = &meshes[meshes.size()-1];
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
		
	return true;
}

void Board::init()
{	
	Video::setMode(0);
	bgInit(3, Text256x256, Pal8bit, 1, 0);
	bgWrap(3);
	dmaCopy(board_bgTiles, CHAR_BASE_ADR(1), board_bgTilesLen);
	dmaCopy(board_bgMap, MAP_BASE_ADR(0), board_bgMapLen);
	dmaCopy(board_bgPal, BG_PALETTE, board_bgPalLen);		
	copyTiles256(piece_tilesTiles, CHAR_BASE_OFFSET(1, board_bgTilesLen), piece_tilesTilesLen, piece_tilesPal, piece_tilesPalLen, 64);	
	
	bgSetScroll(3, 8, 48);
	bgUpdate();	
		
	spawn_mesh(0,1,1);
	meshes[0].set_x(5);
	meshes[0].set_y(10);
	
	spawn_mesh(2,5,10);
	
	user_controllable_mesh = new Mesh(meshes[1]);
	meshes.remove_at(1);
	/*meshes.push_back(_move_(Mesh(6,7,1,4,f2)));
	meshes.push_back(_move_(Mesh(2,2,4,4,example)));
	meshes.push_back(_move_(Mesh(8,7,1,4,f2)));*/
}

void Board::frame()
{
	//greenSwapSwitch();	
	clear();
	draw_mesh(board_mesh);
	for(int i=0;i<meshes.size();i++)
		draw_mesh(meshes[i]);
	draw_mesh(*user_controllable_mesh);
	
	if(meshes.size()>0 && user_controllable_mesh != &meshes[0] && ((*user_controllable_mesh) || meshes[0]))
	{
		*user_controllable_mesh += meshes[0];
		meshes.remove_at(0);
	}
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