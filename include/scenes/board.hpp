#pragma once

#include <Astralbrew>
using namespace Astralbrew;

#include "data/mesh.hpp"
#include "data/piece.hpp"
#include "data/piece_gen.hpp"

#include "board_bg.h"
#include "dialog_bg.h"
#include "dialog_frame.h"
#include "piece_tiles.h"
#include "digits.h"

class Board : public GenericScene
{
private:	
	MapInfo* board_bg = require_map(3, &ROA_board_bg);
	MapInfo* dialog_bg = require_map(1, &ROA_dialog_bg);
	MapInfo* dialog_fg = require_map(0, &ROA_dialog_frame);
	
	ResourceInfo* pieces_gfx = require_asset(&ROA_piece_tiles, Video::Backgrounds(3));
	ResourceInfo* digits_gfx = require_asset(&ROA_digits, Video::Backgrounds(3));
	
	ResourceInfo* dialog_title_gfx = require_tiles(2*12, 4, Video::Backgrounds(0));
	ResourceInfo* dialog_body_gfx = require_tiles(4*23, 4, Video::Backgrounds(0));
	
	int vwf_color = reserve_background_color(Colors::White);
	
	Sprite* speed_bar;
	
	int speed_stripes_color = reserve_sprite_color(Colors::Cyan);		
	short* speed_stripes_ptr = nullptr;	
	Sprite* speed_stripes;

	Vector<Mesh> meshes;
	Mesh board_mesh = Mesh(0,0,10,18);
	Mesh* user_controllable_mesh = nullptr;
	
	int piece_tiles_start_id = 0;			
	int digit0_tile_id = 0;
	int board_bg_tile_id = 0;
	
	VwfEngine vwf_title { Resources::Fonts::default_8x16 };
	VwfEngine vwf_body { Resources::Fonts::default_8x16 };

	PieceGenerator* _piece_generator = &default_piece_generator;
	inline PieceGenerator* get_piece_generator() { return _piece_generator; }
	inline void set_piece_generator(PieceGenerator* piece_gen) { _piece_generator = piece_gen; }
	
	void on_key_down(void* sender, void* _keys);
	void on_key_held(void* sender, void* _keys);
	
	void update_speed_stripes();
protected:
	struct DialogLine 
	{ 
		const char* actor_name;
		const char* message;
		StaticEventHandler line_finished_handler;
		DialogLine* next = nullptr;
	};
	
	/*DialogLine dialogs[5] =
	{
		{"Cappuccino", "Hello World!", nullptr, &dialogs[1] },
		{"Cappuccino", "I missed you", nullptr, &dialogs[2]},
		{"Bob", "That's true", nullptr, &dialogs[3]},
		{"Bob", "How about a very long line that needs to be broken apart hahahahaha hell yeah my dick is as big as this text", nullptr, &dialogs[4]},
		{"Anya", "Guess that works too", nullptr, nullptr},
	};*/
	
	DialogLine* dialog_stream = nullptr;
	
	bool process_dialog();
	void show_dialog(DialogLine* dialog);
	
protected:	
	int move_direction = 1;		
	int mesh_spawn_x = 3;
	bool clear_lines = true;
	bool ffw_enabled = true;
	
protected:
	int frame_cnt=0;
	int frame_key_control = 0;
	
	int goal = 0;
	int score = 0;
	
	int update_rate = 30;
protected:
	void inc_score(int amount);	
	void vblank_skip(int frames_cnt);
	void set_speed(int frames);
	int get_speed() const;	
	
	void set_score(int val);
	void set_goal(int val);
	
	inline int get_goal() const { return goal; }
	inline int get_score() const { return score; }	
protected:
	Event full_lines_count;
	Event lines_cleared;
	Event score_changed;
	Event piece_placed;
	Event board_overlap;	
	
protected:
	void draw_mesh(const Mesh& mesh);
	void spawn_mesh(int gid, int color, int shape);
	void spawn_mesh(const Piece& piece);
	void clear();
	bool ucm_in_bounds() const;
	
protected:
	void hide_speed_panel();

protected:
	void hide_dialog();

public:
	Board() = default;
	virtual void init() override;
	virtual void frame() override;
	~Board();
	
	
	static PieceGenerator default_piece_generator;
};

