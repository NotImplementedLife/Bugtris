#include "scenes/board.hpp"

using namespace Astralbrew;
using namespace Astralbrew::Memory;
using namespace Astralbrew::Video;
using namespace Astralbrew::Objects;

#include <stdlib.h>

PieceGenerator default_piece_generator = PieceGenerator();

void Board::init()
{			
	Video::setMode(0);
	objEnable1D();	
	init_board_table();
	init_dialog_bg();	
	
	init_dialog_fg();	
	
	init_speed_panel();
	

	hide_dialog();
	
	bgSetAlpha(1, 6, 3, 8);
	
	bgUpdate();
		
	//spawn_mesh(0,1,1);
	
	set_goal(0);
	set_score(0);
	
	update_speed_stripes();
	
	clear();
}

void Board::frame()
{	
	if(process_dialog())
	{
		clear();
		draw_mesh(board_mesh);
		for(int i=0;i<meshes.size();i++)
			draw_mesh(meshes[i]);	
		if(user_controllable_mesh)
			draw_mesh(*user_controllable_mesh);
		return;
	}
	
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
				on_piece_placed();

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
			Piece piece = get_piece_generator()->next();
			spawn_mesh(piece);
			if(piece.g_id!=0xFF && !ucm_in_bounds())
			{
				on_board_overlap();
			}
		}
	}
	frame_key_control = 0;
	frame_cnt++;
	if(frame_cnt == update_rate) {
		frame_cnt=0;
	}
	OamPool::deploy();
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
	update_speed_stripes();
}

void Board::inc_score(int amount)
{
	set_score(score+amount);
}

void Board::on_key_down(int keys)
{
	if((keys & KEY_START))
	{
		set_score(1000);
	}
	if((keys & KEY_LEFT) && !(frame_key_control & KEY_LEFT))
	{
		frame_key_control |= KEY_LEFT;
		if(user_controllable_mesh)
		{
			user_controllable_mesh->move(-move_direction, 0);
			if(!ucm_in_bounds())
			{
				user_controllable_mesh->move(+move_direction, 0);
			}
		}
	}	
	else if(keys & KEY_RIGHT && !(frame_key_control & KEY_RIGHT))
	{
		frame_key_control |= KEY_RIGHT;
		if(user_controllable_mesh)
		{
			user_controllable_mesh->move(+move_direction, 0);
			if(!ucm_in_bounds())
			{
				user_controllable_mesh->move(-move_direction, 0);
			}
		}
	}
	if((keys & KEY_A) && !(frame_key_control & KEY_A) && !dialog_stream)
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
		if(ffw_enabled && user_controllable_mesh)
		{			
			user_controllable_mesh->move(0, move_direction);
			if(!ucm_in_bounds())
			{
				user_controllable_mesh->move(0, -move_direction);
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

void Board::update_speed_stripes()
{
	int bars_cnt = (33-update_rate)/3;
	
	u16* buff = (u16*)speed_stripes_addr.get_value();
	for(int y=57;y>=12;y--)
	{								
		int ty = y/8;		
		u16* base = buff + ty*64 + 16 + 2*(y&7);		
		if(!bars_cnt) 
		{
			base[1] = base[16] = 0x0000;
			continue;
		}
		if(y%4==2)
		{
			base[1] = base[16] = 0x0000;
			bars_cnt--;
		}
		else
			base[1] = base[16] = 0x1111;
		
	}
}

int Board::get_speed() const
{
	return update_rate;
}

void Board::hide_speed_panel()
{
	speed_panel->get_attribute()->set_priority(3);
	speed_stripes->get_attribute()->set_priority(3);
}

Board::~Board()
{
	delete speed_panel;
	delete speed_stripes;
	delete user_controllable_mesh;
}