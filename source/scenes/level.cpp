#include "scenes/level.hpp"
#include <stdlib.h>
#include <math.h>

Level::Level(int id) : Board(), lvl_id(id)
{ }

Level::Level(int id, Instantiator::FunctionType<Level> this_level_instantiator, Instantiator::FunctionType<Level> next_level_instantiator)
	: Board(), lvl_id(id)
	, this_level_instantiator(this_level_instantiator)
	, next_level_instantiator(next_level_instantiator)
{ }
	

void Level::init()
{
	Board::init();
	
	int pos_x = 0;	
	
	if(lvl_id==0)
	{
		level_text_chars_spr.push_back(create_sprite(SIZE_8x16, new ObjFrame(&ROA_level_chars,0,3),{pos_x,50},{128,128}));
		pos_x+=8;
	}
	
	int _id = lvl_id;	
	
	while(_id!=0)
	{		
		int digit = _id % 10; 
		_id/=10;	
		level_text_chars_spr.push_back(create_sprite(SIZE_8x16, new ObjFrame(&ROA_level_chars,0,3+digit),{pos_x,50},{128,128}));
		pos_x+=8;
	}
	pos_x+=8; // space
	
	level_text_chars_spr.push_back(create_sprite(SIZE_8x16, new ObjFrame(&ROA_level_chars,0, 0),{pos_x,50},{128,128})); pos_x+=8; // L
	level_text_chars_spr.push_back(create_sprite(SIZE_8x16, new ObjFrame(&ROA_level_chars,0, 1),{pos_x,50},{128,128})); pos_x+=8; // E
	level_text_chars_spr.push_back(create_sprite(SIZE_8x16, new ObjFrame(&ROA_level_chars,0, 2),{pos_x,50},{128,128})); pos_x+=8; // V
	level_text_chars_spr.push_back(create_sprite(SIZE_8x16, new ObjFrame(&ROA_level_chars,0, 1),{pos_x,50},{128,128})); pos_x+=8; // E
	level_text_chars_spr.push_back(create_sprite(SIZE_8x16, new ObjFrame(&ROA_level_chars,0, 0),{pos_x,50},{128,128})); // L
	
	
	int offset = 78 + (80-pos_x-8)/2;
	
	for(int i=0;i<level_text_chars_spr.size();i++)
	{		
		level_text_chars_spr[i]->set_position(offset + pos_x - (int)level_text_chars_spr[i]->x(), -16);
		level_text_chars_spr[i]->set_priority(0);
		level_text_chars_spr[i]->update_position();		
				
		spr_velocity.push_back((rand()%10)*0.03f+0.1f);
	}
	
	board_overlap.add_event(&Level::on_board_overlap, this);
		
}

namespace
{
	int f(float v, int t)
	{
		float dt=t*0.5f;
		return 80-80*cos(v*dt)/(1+dt);
	}
}

void Level::frame()
{
	if(!level_animation_done)
	{
		level_display_animation();	
		GenericScene::frame();
	}
	else		
	{		
		Board::frame();			
	}	
}

Level::~Level()
{
	board_overlap.remove_event(&Level::on_board_overlap, this);	
}

void Level::level_display_animation()
{
	if(t<90)
	{
		for(int i=0;i<level_text_chars_spr.size();i++)
		{
			level_text_chars_spr[i]->set_position(
				level_text_chars_spr[i]->x(),			
				f(spr_velocity[i],t)
			);
			level_text_chars_spr[i]->update_position();			
		}
		t++;
	}
	else if(t<100)
	{
		for(int i=0;i<level_text_chars_spr.size();i++)
		{
			level_text_chars_spr[i]->set_position(
				level_text_chars_spr[i]->x(),			
				80
			);
			level_text_chars_spr[i]->update_position();			
		}
		t++;
	}
	else if(t<170)
	{
		for(int i=0;i<level_text_chars_spr.size();i++)
		{
			level_text_chars_spr[i]->set_position(
				level_text_chars_spr[i]->x(), 
				level_text_chars_spr[i]->y() + spr_velocity[i]
			);
			spr_velocity[i]+=rand()%10*0.02f;
			level_text_chars_spr[i]->update_position();			
		}
		t++;
	}
	else 
	{		
		level_animation_done = true;
		level_start.trigger(this, nullptr);		
	}
}

void Level::on_board_overlap(void* sender, void*)
{
	game_over.trigger(sender, nullptr);
	
	show_dialog(&game_over_dialog);
}


void Level::on_game_over_dialog_finished(void* sender, void*)
{
	Level* level = (Level*) sender;
	if(level->this_level_instantiator == nullptr)
		level->close()->next(nullptr);
	level->close()->next(level->this_level_instantiator());
}

void Level::next_level()
{
	if(next_level_instantiator == nullptr)
		close()->next(nullptr);
	close()->next(next_level_instantiator());
}