#include "scenes/board.hpp"

#include "speed_panel.h"

PieceGenerator Board::default_piece_generator = PieceGenerator();


void Board::init() 
{
	GenericScene::init();	
	
	speed_stripes_ptr = reserve_sprite_vram(Measure().bpp(4).tiles(4*8).value());
	speed_stripes = create_sprite(SIZE_32x64, new ObjFrame(speed_stripes_ptr, 4), {10,10});
	speed_stripes->set_palette_number(speed_stripes_color>>4);
	
	speed_bar = create_sprite(SIZE_32x64, new ObjFrame(&ROA_speed_panel, 0, 0), {10,10});
	
	piece_tiles_start_id = translate_tile_id(3, pieces_gfx, 0);
	digit0_tile_id = translate_tile_id(3, digits_gfx, 0);
	
	board_bg_tile_id = Video::bgGetMapPtr(3)[0x7*32+0xB]; // get tile to clear the board with 
	
	Video::bgSetAlpha(1, 6, 3, 8);
	Video::bgSetScroll(3, 8, 48);	
	Video::bgUpdate();
	
	key_down.add_event(&Board::on_key_down, this);
	
	set_score(0);
	set_goal(0);
	set_speed(20);	
	
	vwf_title.set_render_space(dialog_title_gfx->offset(),2,12);
	VwfEngine::prepare_map(vwf_title, (unsigned short*)Video::bgGetMapPtr(0), 32, 1, 12, vwf_color>>4);	
	
	vwf_body.set_render_space(dialog_body_gfx->offset(), 4,23);
	VwfEngine::prepare_map(vwf_body, (unsigned short*)Video::bgGetMapPtr(0), 32, 6, 15, vwf_color>>4);
	
	hide_dialog();
	
	//Debug::tty_log("INIT ADDRESS ====== %X", reinterpret_cast<void*>(&dialogs[0]));
}

void Board::frame()
{		
	clear();
	draw_mesh(board_mesh);
	for(int i=0;i<meshes.size();i++)
		draw_mesh(meshes[i]);	
	if(user_controllable_mesh)
		draw_mesh(*user_controllable_mesh);
	
	if(process_dialog()) return;
	
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
				piece_placed.trigger(this, nullptr);

				if(clear_lines) 
				{					
					int lines = board_mesh.clear_full_lines(10);
					if(lines)
						lines_cleared.trigger(this, (void*)lines);
				}
				else
				{					
					int lines=board_mesh.count_full_lines(10);
					if(lines)
						full_lines_count.trigger(this, (void*)lines);
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
				board_overlap.trigger(this, nullptr);
			}
		}
	}
	frame_key_control = 0;
	frame_cnt++;
	if(frame_cnt == update_rate) {
		frame_cnt=0;
	}
	
	GenericScene::frame();
}

Board::~Board()
{
	key_down.remove_event(&Board::on_key_down, this);
	delete user_controllable_mesh;
}

void Board::draw_mesh(const Mesh& mesh)
{
	if(mesh.width()==0 || mesh.height()==0)
		return;
	short* map = Video::bgGetMapPtr(3) + 7*32 + 11;	
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

void Board::spawn_mesh(int gid, int color, int shape)
{		
	user_controllable_mesh = new Mesh(mesh_spawn_x,0,4,4,mesh_gfx[gid]);
	user_controllable_mesh->replace(1, mk_block(color, shape));
}

void Board::spawn_mesh(const Piece& piece)
{
	if(piece.g_id != 0xFF)
		spawn_mesh(piece.g_id, piece.color, piece.shape);
}

void Board::clear()
{
	short* map = Video::bgGetMapPtr(3) + 7*32 + 11;	
	for(int y=0;y<18;y++)
	{	
		Memory::fill_16(&map[32*y], board_bg_tile_id, 10);
	}
}

bool Board::ucm_in_bounds() const
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

void Board::on_key_down(void* sender, void* _keys)
{
	int keys = (int)_keys;
	if((keys & Keys::Start))
	{
		set_score(1000);
	}
	if((keys & Keys::Left) && !(frame_key_control & Keys::Right))
	{
		frame_key_control |= Keys::Left;
		if(user_controllable_mesh)
		{
			user_controllable_mesh->move(-move_direction, 0);
			if(!ucm_in_bounds())
			{
				user_controllable_mesh->move(+move_direction, 0);
			}
		}
	}	
	else if((keys & Keys::Right) && !(frame_key_control & Keys::Right))
	{
		frame_key_control |= Keys::Right;
		if(user_controllable_mesh)
		{
			user_controllable_mesh->move(+move_direction, 0);
			if(!ucm_in_bounds())
			{
				user_controllable_mesh->move(-move_direction, 0);
			}
		}
	}
	if((keys & Keys::A) && !(frame_key_control & Keys::A) && !dialog_stream)
	{
		frame_key_control |= Keys::A;
		if(user_controllable_mesh)
		{
			user_controllable_mesh->rotate_cw();			
			if(!ucm_in_bounds())
			{
				user_controllable_mesh->rotate_ccw();
			}
		}
	}
	else if((keys & Keys::B) && !(frame_key_control & Keys::B))
	{
		frame_key_control |= Keys::B;
		if(user_controllable_mesh)
		{
			user_controllable_mesh->rotate_ccw();			
			if(!ucm_in_bounds())
			{
				user_controllable_mesh->rotate_cw();
			}
		}		
	}
}

int Board::get_speed() const
{
	return update_rate;
}

void Board::set_score(int val)
{
	int old_val = val;
	score = val;
	short* map = Video::bgGetMapPtr(3)+11*32+28;
	for(int i=0;i<4;i++) 
	{
		*(map--)=digit0_tile_id+val%10;
		val/=10;
	}	
	if(old_val!=val)
	{
		score_changed.trigger(this, (void*)old_val);
	}
}

void Board::set_goal(int val)
{	
	goal = val;
	short* map = Video::bgGetMapPtr(3)+9*32+28;
	for(int i=0;i<4;i++) 
	{
		*(map--)=digit0_tile_id+val%10;
		val/=10;
	}
}

void Board::vblank_skip(int frames_cnt)
{
	for(int i=frames_cnt;i--;)
	{		
		force_wait_vblank();
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


void Board::update_speed_stripes()
{
	int bars_cnt = (33-update_rate)/3;
	
	short* buff =  speed_stripes_ptr;
	for(int y=57;y>=12;y--)
	{								
		int ty = y/8;		
		short* base = buff + ty*64 + 16 + 2*(y&7);
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
			base[1] = base[16] = (speed_stripes_color & 0xF) * 0x1111;
		
	}
}

void Board::hide_speed_panel()
{
	speed_bar->hide();
	speed_stripes->hide();
}

void Board::hide_dialog()
{
	Video::bgHide(0);
	Video::bgHide(1);
}

void Board::show_dialog(DialogLine* dialog)
{
	Video::bgShow(0);
	Video::bgShow(1);
	vwf_title.put_text(dialog->actor_name, Video::Pal4bit, SolidColorBrush(vwf_color&0xF));
	
	dialog_stream = dialog;
}

bool Board::process_dialog()
{
	if(!dialog_stream) 
		return false;

	vwf_body.clear(Video::Pal4bit);
	int count = vwf_body.put_text(dialog_stream->message, Video::Pal4bit, SolidColorBrush(vwf_color&0xF));
	dialog_stream->message+=count;
	
	if(count==0)
	{			
		if(dialog_stream->line_finished_handler != nullptr)
		{
			(*(dialog_stream->line_finished_handler))(this, nullptr);
		}
		dialog_stream = dialog_stream->next;
		if(dialog_stream)
		{
			vwf_title.clear(Video::Pal4bit);
			vwf_title.put_text(dialog_stream->actor_name, Video::Pal4bit, SolidColorBrush(vwf_color&0xF));
		}
		else
		{
			hide_dialog();
		}
		return false;
	}
	else
	{
		//for(int i=0;i<10;i++) force_wait_vblank();
		wait_for_key_pressed(Keys::A | Keys::Start);
		return true;
	}
	return true;
}	
