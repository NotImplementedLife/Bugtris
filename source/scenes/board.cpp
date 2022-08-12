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
	
	show_dialog("Cappuccino", "Hi there!");	
	//show_dialog(nullptr, nullptr);
	
	bgSetAlpha(1, 6, 3, 8);
	
	bgUpdate();
		
	spawn_mesh(0,1,1);
}

void Board::frame()
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
	for(int k=9;k--;) VBlankIntrWait();
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