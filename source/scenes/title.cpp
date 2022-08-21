#include "scenes/title.hpp"

#include "title-bg.h"
#include "title-fg.h"
#include "piece_tiles.h"
#include <stdlib.h>

using namespace Astralbrew;
using namespace Astralbrew::Video;

void Title::init()
{
	Video::setMode(0);

	Address transparent_tile;
	vram_chr_1.reserve(&transparent_tile, tiles_size_8bpp(1));
	vram_chr_1.reserve(&bg_addr, title_bgTilesLen);
	vram_chr_1.reserve(&fg_addr, title_fgTilesLen);		
	vram_chr_1.reserve(&piece_bg_addr, piece_tilesTilesLen);			
	
	bgInit(2, Text256x256, Pal8bit, 1, 0);
	bgInit(3, Text256x256, Pal8bit, 1, 1);
	
	piece_tiles_start_id = ((int)piece_bg_addr.get_value() - (int)bgGetTilesPtr(3))/64;
	
	dmaCopy(title_bgMap, bgGetMapPtr(3), title_bgMapLen);
	dmaCopy(title_fgMap, bgGetMapPtr(2), title_fgMapLen);
		
	
	copyTiles256(title_bgTiles, bg_addr.get_value(), title_bgTilesLen, title_bgPal, title_bgPalLen, 1);
	copyTiles256(title_fgTiles, fg_addr.get_value(), title_fgTilesLen, title_fgPal, title_fgPalLen, 33);
	copyTiles256(piece_tilesTiles, piece_bg_addr.get_value(), piece_tilesTilesLen, piece_tilesPal, piece_tilesPalLen, 64);
	
	meshes.clear();
	
	meshes.push_back(Mesh(0,2,4,4,mesh_gfx[4]));
	meshes.back().rotate_cw();
	meshes.back().replace(1, mk_block(3,3));	
	
	meshes.push_back(Mesh(0,4,4,4,mesh_gfx[6]));
	meshes.back().rotate_ccw();
	meshes.back().replace(1, mk_block(1,2));
	
	meshes.push_back(Mesh(0,5,4,4,mesh_gfx[5]));
	meshes.back().rotate_cw();
	meshes.back().replace(1, mk_block(4,1));
	
	
	meshes.push_back(Mesh(4,4,4,4,mesh_gfx[5]));
	meshes.back().rotate_cw();
	meshes.back().rotate_cw();
	meshes.back().replace(1, mk_block(2,0));
		
	meshes.push_back(Mesh(6,4,4,4,mesh_gfx[0]));	
	meshes.back().replace(1, mk_block(5,1));
	
	
	meshes.push_back(Mesh(9,4,4,4,mesh_gfx[5]));
	meshes.back().rotate_ccw();
	meshes.back().replace(1, mk_block(7,3));
	
	meshes.push_back(Mesh(9,5,4,4,mesh_gfx[6]));
	meshes.back().rotate_cw();
	meshes.back().replace(1, mk_block(6,2));
		
	meshes.push_back(Mesh(8,8,4,4,mesh_gfx[0]));
	meshes.back().rotate_cw();
	meshes.back().replace(1, mk_block(3,1));
	
	
	meshes.push_back(Mesh(14,5,4,4,mesh_gfx[0]));	
	meshes.back().replace(1, mk_block(2,0));
	
	meshes.push_back(Mesh(14,2,4,4,mesh_gfx[6]));	
	meshes.back().rotate_cw();
	meshes.back().rotate_cw();
	meshes.back().replace(1, mk_block(5,3));
	
	
	meshes.push_back(Mesh(17,5,4,4,mesh_gfx[0]));	
	meshes.back().replace(1, mk_block(3,1));
	
	meshes.push_back(Mesh(18,4,4,4,mesh_gfx[3]));	
	meshes.back().rotate_cw();
	meshes.back().replace(1, mk_block(4,2));
	
	
	meshes.push_back(Mesh(21,6,4,4,mesh_gfx[4]));	
	meshes.back().replace(1, mk_block(1,0));	
	
	meshes.push_back(Mesh(21,2,4,4,mesh_gfx[1]));	
	meshes.back().replace(1, mk_block(7,1));
	
	
	meshes.push_back(Mesh(25,3,4,4,mesh_gfx[5]));
	meshes.back().rotate_ccw();
	meshes.back().replace(1, mk_block(2,0));
	
	meshes.push_back(Mesh(25,5,4,4,mesh_gfx[0]));
	meshes.back().rotate_cw();
	meshes.back().replace(1, mk_block(3,1));
	
	meshes.push_back(Mesh(25,6,4,4,mesh_gfx[5]));
	meshes.back().rotate_cw();
	meshes.back().replace(1, mk_block(6,3));	
	
	/*int max_y = 0;	
	do
	{
		for(int i=0;i<meshes.size();i++)
		{
			if(rand()%2==1)
			{
				if(rand()%3==0)
				{
					meshes[i].set_y(meshes[i].y()-1);
					if(mesh_collides(i))
					{
						meshes[i].set_y(meshes[i].y()+1);
					}
					else
					{
						moves.push_back({i,1});
					}				
				}
			}			
		}
		
		max_y = -100;
		for(int i=0;i<meshes.size();i++)
		{
			max_y = max(max_y, meshes[i].y());
		}
	} while(max_y>-4);*/
	
	bgSetScroll(3, 8, 0);
	bgUpdate();	
}

bool Title::mesh_collides(int i)
{
	for(int j=0;j<meshes.size();j++)
	{
		if(j!=i)
			if(meshes[i] && meshes[j])
				return true;		
	}
	return false;
}

void Title::build()
{
	
}

void Title::draw_mesh(const Mesh& mesh)
{
	if(mesh.width()==0 || mesh.height()==0)
		return;
	u16* map = ((u16*)MAP_BASE_ADR(1)) + 3*32 + 1;
	for(int y=mesh.y();y<mesh.y()+mesh.height();y++)
	{
		for(int x=mesh.x();x<mesh.x()+mesh.width();x++)
		{
			if(0<=x && x<30 && 0<=y && y<18)
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

void Title::frame()
{
	for(int i=0;i<meshes.size();i++)
	{
		draw_mesh(meshes[i]);		
	}
	
	bgScroll(2, 1, 0);
	bgUpdate();
}
