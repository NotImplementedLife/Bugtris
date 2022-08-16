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
	
	clear();
	draw_mesh(board_mesh);
	for(int i=0;i<meshes.size();i++)
		draw_mesh(meshes[i]);
	draw_mesh(*user_controllable_mesh);
		
	if(frame_cnt==0) 
	{	
		//greenSwapSwitch();
		
		if(user_controllable_mesh)
		{				
			user_controllable_mesh->move(0,1);
			if(!ucm_in_bounds())
			{			
				user_controllable_mesh->move(0,-1);
				board_mesh+=*user_controllable_mesh;		

				if(clear_lines) 
				{
					score=board_mesh.clear_full_lines(10);				
					set_score(score);
				}
				else
				{					
					int lines=board_mesh.count_full_lines(10);
					on_full_lines_count(lines);
					//score=board_mesh.count_full_lines(10);
					//set_score(score);
				}					
				
				delete user_controllable_mesh;
				user_controllable_mesh = nullptr;
			}			
				
			if(!user_controllable_mesh)
			{
				spawn_mesh(rand()%6, rand()%8, rand()%4);
			}
		}	
	}	
	frame_key_control = 0;
	frame_cnt++;
	if(frame_cnt==30) {
		frame_cnt=0;
	}
}

void Board::on_key_down(int keys)
{	
	if((keys & KEY_LEFT) && !(frame_key_control & KEY_LEFT))
	{		
		frame_key_control |= KEY_LEFT;
		if(user_controllable_mesh)
		{			
			user_controllable_mesh->move(-move_direction,0);
			if(!ucm_in_bounds())
			{
				user_controllable_mesh->move(+move_direction,0);
			}
		}
	}	
	else if(keys & KEY_RIGHT && !(frame_key_control & KEY_RIGHT))
	{
		frame_key_control |= KEY_RIGHT;
		if(user_controllable_mesh)
		{			
			user_controllable_mesh->move(+move_direction,0);
			if(!ucm_in_bounds())
			{
				user_controllable_mesh->move(-move_direction,0);
			}
		}
	}
	if(keys & KEY_UP && !(frame_key_control & KEY_UP))
	{
		frame_key_control |= KEY_UP;
		if(user_controllable_mesh)
		{			
			user_controllable_mesh->rotate_cw();			
			if(!ucm_in_bounds())
			{
				user_controllable_mesh->rotate_ccw();
			}
		}
	}
}

void Board::on_key_held(int keys)
{
	if(keys & KEY_DOWN)
	{
		if(user_controllable_mesh)
		{			
			user_controllable_mesh->move(0,move_direction);
			if(!ucm_in_bounds())
			{
				user_controllable_mesh->move(0,-move_direction);
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