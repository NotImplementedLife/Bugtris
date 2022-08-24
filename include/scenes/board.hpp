#pragma once

#include <Astralbrew>
#include "data/mesh.hpp"
#include "data/piece.hpp"
#include "data/piece_gen.hpp"

using Astralbrew::Text::VwfEngine;
using Astralbrew::Memory::Address;
using Astralbrew::Memory::VramManager;
using Astralbrew::Text::ReadOnlyFont;
using Astralbrew::Entity::Sprite;

extern PieceGenerator default_piece_generator;

class Board : public Astralbrew::World::Scene
{
protected:
	VramManager vram_chr_1 = VramManager::from_char_block(1);	
	VramManager vram_chr_2 = VramManager::from_char_block(2);	
	VramManager vram_chr_3 = VramManager::from_char_block(3);
	VramManager vram_obj = VramManager::from_char_block(4);
private:
	char id[10]="Board";
	Astralbrew::Vector<Mesh> meshes;
	Mesh board_mesh = Mesh(0,0,10,18);
public:
	Mesh* user_controllable_mesh = nullptr;
private:
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
	void spawn_mesh(const Piece& piece);
	
	bool ucm_in_bounds();
	
	void set_dialog_character(const void* tiles, const void* palette);
	
	void init_board_table();
	void init_dialog_bg();
	void init_dialog_fg();
	void init_speed_panel();
	
	int frame_cnt=0;
	int frame_key_control = 0;
	
	int goal = 0;
	int score = 0;
	
	int update_rate = 30;
		
	const char* dialog_stream = nullptr;
	
	bool process_dialog();
	
	void (*dialog_callback)(void*) = 0;
	
	PieceGenerator* _piece_generator = &default_piece_generator;
	
	Address speed_panel_tiles_addr;
	Sprite* speed_panel = nullptr;
	
	Address speed_stripes_addr;
	Sprite* speed_stripes = nullptr;
public:	
	void show_dialog(const char* actor_name, const char* message, void (*callback)(void*) = 0);	
	
	void hide_dialog();
	bool new_dialog = false;
public:
	void set_goal(int val);
	void set_score(int val);
	
	int get_goal() const { return goal; }
	int get_score() const { return score; }
	
	void hide_speed_panel();
protected:
	int move_direction = 1;		
	int mesh_spawn_x = 3;
	bool clear_lines = true;
	bool ffw_enabled = true;

public:	
	virtual void on_full_lines_count(int value) { }
	virtual void on_lines_cleared(int value) { }
	virtual void on_score_changed(int old_value) { }
public:
	void inc_score(int amount);
	void blank_skip(int frames_cnt) const;
	void set_speed(int frames);
	
	int get_speed() const;
	
	void update_speed_stripes();
public:
	inline PieceGenerator* get_piece_generator() { return _piece_generator; }
	inline void set_piece_generator(PieceGenerator* piece_gen) { _piece_generator = piece_gen; }

public:
	virtual void init() override;
	
	virtual void frame() override;	
	
	virtual void on_key_down(int keys) override;
	virtual void on_key_held(int keys) override;
	
	Board() = default;
	~Board();
};