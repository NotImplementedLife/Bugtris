#pragma once

#include "scenes/board.hpp"

#include "level_chars.h"

#include "utils/instantiator.hpp"

class Level : public Board
{
private:
	
	Vector<Sprite*> level_text_chars_spr;	
	
	int t=0;
	Vector<float> spr_velocity;
	
	int lvl_id = 0;
	
	bool level_animation_done = false;
	void level_display_animation();

public:
	Event level_start;
	Event game_over;

public:
	Level(int id=0);
	Level(int id, Instantiator::FunctionType<Level> this_level_instantiator, Instantiator::FunctionType<Level> next_level_instantiator);
	void init() override;
	void frame() override;
	~Level();	
	
	void on_board_overlap(void* sender, void*);
	
	void next_level();
	
	Instantiator::FunctionType<Level> this_level_instantiator = nullptr;
	Instantiator::FunctionType<Level> next_level_instantiator = nullptr;	
	
	DialogLine game_over_dialog = 
	{
		"Cappuccino",
		"Game over... Still, our objective has yet to be achieved. Keep your head up and let's try again!",
		&Level::on_game_over_dialog_finished, 
		nullptr};
	
	static void on_game_over_dialog_finished(void*, void*);
	
	static void next_level_handler(void*, void*);
	static void restart_level_handler(void*, void*);
};