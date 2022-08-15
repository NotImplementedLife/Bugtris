#include "scenes/board.hpp"

using namespace Astralbrew;
using namespace Astralbrew::Memory;
using namespace Astralbrew::Video;

#include <stdlib.h>

void Board::init()
{			
	Video::setMode(0);
	init_board_table();
	init_dialog_bg();
	init_dialog_fg();		

	hide_dialog();
	
	bgSetAlpha(1, 6, 3, 8);
	
	bgUpdate();
		
	spawn_mesh(0,1,1);
	
	set_goal(0);
	set_score(0);
}

void Board::frame()
{	
	process_dialog();
	if(frame_cnt==0) 
	{	
		//greenSwapSwitch();
		clear();
		draw_mesh(board_mesh);
		for(int i=0;i<meshes.size();i++)
			draw_mesh(meshes[i]);
		draw_mesh(*user_controllable_mesh);
		
		if(user_controllable_mesh)
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
	}
	frame_cnt++;
	if(frame_cnt==20) {
		frame_cnt=0;
	}
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

void Board::set_goal(int val)
{
	u16* map = bgGetMapPtr(3)+9*32+28;
	for(int i=0;i<4;i++) 
	{
		*(map--)=digit0tileid+val%10;
		val/=10;
	}
}

void Board::set_score(int val)
{
	u16* map = bgGetMapPtr(3)+11*32+28;
	for(int i=0;i<4;i++) 
	{
		*(map--)=digit0tileid+val%10;
		val/=10;
	}	
}

Board::~Board()
{
	
}