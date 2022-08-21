#include "scenes/board.hpp"

using namespace Astralbrew;
using namespace Astralbrew::Memory;
using namespace Astralbrew::Video;

#include <stdlib.h>

PieceGenerator default_piece_generator = PieceGenerator();

void Board::init()
{			
	Video::setMode(0);
	init_board_table();
	init_dialog_bg();
	init_dialog_fg();		

	hide_dialog();
	
	bgSetAlpha(1, 6, 3, 8);
	
	bgUpdate();
		
	//spawn_mesh(0,1,1);
	
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
	
	if(user_controllable_mesh)
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
					int lines = board_mesh.clear_full_lines(10);
					if(lines)
						on_lines_cleared(lines);
				}
				else
				{					
					int lines=board_mesh.count_full_lines(10);
					if(lines)
						on_full_lines_count(lines);
				}					
				
				delete user_controllable_mesh;
				user_controllable_mesh = nullptr;
			}						
		}	
		if(!user_controllable_mesh)
		{
			spawn_mesh(get_piece_generator()->next());
		}
	}	
	frame_key_control = 0;
	frame_cnt++;
	if(frame_cnt == update_rate) {
		frame_cnt=0;
	}
}

void Board::blank_skip(int frames_cnt) const
{
	for(int i=frames_cnt;i--;)
	{		
		VBlankIntrWait();
	}
}

void Board::set_speed(int frames)
{
	update_rate = frames;
}

void Board::inc_score(int amount)
{
	set_score(score+amount);
}

void Board::on_key_down(int keys)
{	
	if((keys & KEY_START))
	{		
		this->close()->next(new Board());
		FATAL_ERROR("Entrypoint jump missed");
	}
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
	goal = val;
	u16* map = bgGetMapPtr(3)+9*32+28;
	for(int i=0;i<4;i++) 
	{
		*(map--)=digit0tileid+val%10;
		val/=10;
	}
}

void Board::set_score(int val)
{
	int old_val = val;
	score = val;
	u16* map = bgGetMapPtr(3)+11*32+28;
	for(int i=0;i<4;i++) 
	{
		*(map--)=digit0tileid+val%10;
		val/=10;
	}	
	if(old_val!=val)
	{
		on_score_changed(old_val);
	}
}

Board::~Board()
{
	
}