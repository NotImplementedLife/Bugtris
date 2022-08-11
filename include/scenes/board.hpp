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
	Astralbrew::Vector<Mesh> meshes;
	Mesh board_mesh;
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
	
	Address dialog_txt_addr;
		
	VwfEngine vwf = VwfEngine(Astralbrew::Resources::Fonts::default_8x16);
	
	void clear();
	void draw_mesh(const Mesh& mesh);
	void spawn_mesh(int gid, int color, int shape);
	
	bool ucm_in_bounds();
	
	void set_dialog_character(const void* tiles, const void* palette);
	
	void init_board_table();
	void init_dialog_bg();
	void init_dialog_fg();
public:
	virtual void init() override;	
	
	virtual void frame() override;
	
	virtual void on_key_down(int keys) override;
	
	~Board();
};