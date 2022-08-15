#pragma once

#include <AstralbrewLib>
#include "data/mesh.hpp"

using Astralbrew::Text::VwfEngine;
using Astralbrew::Memory::Address;
using Astralbrew::Memory::VramManager;
using Astralbrew::Text::ReadOnlyFont;

class Board : public Astralbrew::World::Scene
{
private:
	char id[10]="Board";
	Astralbrew::Vector<Mesh> meshes;
	Mesh board_mesh = Mesh(0,0,10,18);
	Mesh* user_controllable_mesh = NULL;
	
	VramManager vram_chr_1 = VramManager::from_char_block(1);	
	VramManager vram_chr_2 = VramManager::from_char_block(2);	
	VramManager vram_chr_3 = VramManager::from_char_block(3);
	
	int piece_tiles_start_id = 0;	
	
	
	Address board_bg_addr;
	Address piece_bg_addr;
	Address dialog_bg_addr;
	Address frame_bg_addr;
	Address char_pic_addr;
	Address digits_addr;
	int digit0tileid=0;
	
	Address dialog_txt_addr;
	Address dialog_title_addr;
		
	VwfEngine vwf_title = VwfEngine(Astralbrew::Resources::Fonts::default_8x16);
	VwfEngine vwf_body = VwfEngine(Astralbrew::Resources::Fonts::default_8x16);
	
	void clear();
	void draw_mesh(const Mesh& mesh);
	void spawn_mesh(int gid, int color, int shape);
	
	bool ucm_in_bounds();
	
	void set_dialog_character(const void* tiles, const void* palette);
	
	void init_board_table();
	void init_dialog_bg();
	void init_dialog_fg();
	
	void show_dialog(const char* actor_name, const char* message);
	void process_dialog();
	void hide_dialog();
	
	int frame_cnt=0;
	int frame_key_control = 0;
	
	int score=0;
		
	const char* dialog_stream = nullptr;
	
private:
	void set_goal(int val);
	void set_score(int val);
	
private:
	int move_direction = 1;		
	bool clear_lines = false;
	
public:
	virtual void init() override;	
	
	virtual void frame() override;
	
	virtual void on_key_down(int keys) override;
	virtual void on_key_held(int keys) override;
	
	~Board();
};