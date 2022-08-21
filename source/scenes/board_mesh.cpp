#include "scenes/board.hpp"

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

static const u8 mesh_gfx[6][16] = 
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
	{
		0,0,0,0,
		0,1,1,1,
		0,0,1,0,
		0,0,0,0,
	},
};

void Board::spawn_mesh(int gid, int color, int shape)
{		
	user_controllable_mesh = new Mesh(mesh_spawn_x,0,4,4,mesh_gfx[gid]);
	user_controllable_mesh->replace(1, mk_block(color, shape));
}

void Board::spawn_mesh(const Piece& piece)
{
	spawn_mesh(piece.g_id, piece.color, piece.shape);
}

void Board::clear()
{
	u16* map = ((u16*)MAP_BASE_ADR(0)) + 7*32 + 11;
	const int zero=0;
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