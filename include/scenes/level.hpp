#pragma once

#include "scenes/board.hpp"

using Astralbrew::Entity::Sprite;
using Astralbrew::Memory::Address;
using Astralbrew::Vector;
using Astralbrew::sf24;

class Level : public Board
{
private:
	Address level_text_chars_base;
	Vector<Address> level_text_chars_addr;
	Vector<Sprite*> level_text_chars_spr;		
	
	int t=0;
	Vector<float> spr_velocity;
	
	int lvl_id = 0;
	
	bool level_animation_done = false;
	void level_display_animation();
public:
	Level(int id=0);
	void init() override;
	void frame() override;
	~Level();	
	
	void next_level();
	
	virtual void on_board_overlap() override { on_game_over(); }
	virtual void on_game_over();
public:
	virtual void on_level_start();
};