#include "scenes/board.hpp"

using namespace Astralbrew;
using namespace Astralbrew::Video;

#include "board-bg.h"
#include "piece_tiles.h"

void Board::draw_mesh(const Mesh& mesh)
{
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


const u8 example[16] = {
	0x80, 0x84, 0x88, 0x8C, 
	0x81, 0x85, 0x89, 0x8D, 
	0x90, 0x94, 0x98, 0x9C, 
	0x91, 0x95, 0x99, 0x9D, 
};

const u8 f2[4] = {
	0x80, 0x81, 0x82, 0x83
};

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

	/*m[0] = new Mesh(0,0,4,4,example);
	m[1] = new Mesh(6,7,1,4,f2);
	m[2] = new Mesh(8,7,1,4,f2);*/
	Mesh m0 = Mesh(2,2,4,4,example);
	Mesh m1 = Mesh(6,7,1,4,f2);
	Mesh m2 = Mesh(8,7,1,4,f2);
	meshes.push_back((Mesh&&)(m0));
	meshes.push_back((Mesh&&)(m1));
	meshes.push_back((Mesh&&)(m2));
}

void Board::frame()
{
	//greenSwapSwitch();	
	for(int i=0;i<meshes.size();i++)
		draw_mesh(meshes[i]);	
	
	/*draw_mesh(*m[0]);
	draw_mesh(*m[1]);
	draw_mesh(*m[2]);*/
	while(1) VBlankIntrWait();
	
}

Board::~Board()
{
	
}