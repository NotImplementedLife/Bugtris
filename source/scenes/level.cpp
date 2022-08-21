#include "scenes/level.hpp"

#include <Astralbrew>
#include "level-chars.h"
#include <gba.h>
#include <stdio.h>
#include <stdlib.h>

using namespace Astralbrew;
using namespace Astralbrew::Video;
using namespace Astralbrew::Objects;

Level::Level(int id)
{			
	lvl_id = id;
	level_text_chars_addr.resize(13);	
}

void Level::init()
{	
	Board::init();
	objEnable1D();	
		
	Address transparent_tile;
	vram_obj.reserve(&transparent_tile, tiles_size_4bpp(1));
	vram_obj.reserve(&level_text_chars_base, tiles_size_4bpp(2*level_text_chars_addr.size()));
	
	level_text_chars_base.write(level_charsTiles, level_charsTilesLen);	
	
	SPRITE_PALETTE[0x01] = 0x7FFF;
	SPRITE_PALETTE[0x11] = 0x7FFF;
	
	assert(level_text_chars_addr.size()>0);
	for(int i=0;i<13;i++)
		level_text_chars_addr[i] = level_text_chars_base + tiles_size_4bpp(2*i);	                    	
			
	int pos_x = 0;
	if(lvl_id==0)
	{
		level_text_chars_spr.push_back(Sprite::quick16(&level_text_chars_addr[3], SIZE_8x16, ANCHOR_CENTER));
		level_text_chars_spr.back()->set_position(pos_x, 50);
		pos_x+=8;
	}
	level_text_chars_spr.clear();
	
	int _id = lvl_id;	
	
	while(_id!=0)
	{		
		int digit = _id % 10;
		_id/=10;	
		level_text_chars_spr.push_back(Sprite::quick16(&level_text_chars_addr[3 + digit], SIZE_8x16, ANCHOR_CENTER));
		level_text_chars_spr.back()->set_position(pos_x, 0);				
		pos_x+=8;		
	}	
	
	pos_x+=8; // space
		
	level_text_chars_spr.push_back(Sprite::quick16(&level_text_chars_addr[0], SIZE_8x16, ANCHOR_CENTER)); // L
	level_text_chars_spr.back()->set_position(pos_x, 0); pos_x+=8;
	level_text_chars_spr.push_back(Sprite::quick16(&level_text_chars_addr[1], SIZE_8x16, ANCHOR_CENTER)); // E
	level_text_chars_spr.back()->set_position(pos_x, 0); pos_x+=8;
	level_text_chars_spr.push_back(Sprite::quick16(&level_text_chars_addr[2], SIZE_8x16, ANCHOR_CENTER)); // V
	level_text_chars_spr.back()->set_position(pos_x, 0); pos_x+=8;
	level_text_chars_spr.push_back(Sprite::quick16(&level_text_chars_addr[1], SIZE_8x16, ANCHOR_CENTER)); // E
	level_text_chars_spr.back()->set_position(pos_x, 0); pos_x+=8;
	level_text_chars_spr.push_back(Sprite::quick16(&level_text_chars_addr[0], SIZE_8x16, ANCHOR_CENTER)); // L
	level_text_chars_spr.back()->set_position(pos_x, 0);

	int offset = 78 + (80-pos_x-8)/2;
	spr_velocity.clear();
					
	for(int i=0;i<level_text_chars_spr.size();i++)
	{		
		level_text_chars_spr[i]->set_position(
			offset + pos_x - level_text_chars_spr[i]->pos_x(), -16
		);
		level_text_chars_spr[i]->get_attribute()->set_priority(0);
		level_text_chars_spr[i]->update_position(nullptr);
		level_text_chars_spr[i]->update_visual();
				
		spr_velocity.push_back((rand()%10)*0.03f+0.1f);
	}	
	
	OamPool::deploy();	
	
	bgHide(0);
	bgHide(1);
}

#include "math.h"

int f(float v, int t)
{
	float dt=t*0.5f;
	return 80-80*cos(v*dt)/(1+dt);
}

void Level::level_display_animation()
{
	if(t<90)
	{
		for(int i=0;i<level_text_chars_spr.size();i++)
		{
			level_text_chars_spr[i]->set_position(
				level_text_chars_spr[i]->pos_x(),			
				f(spr_velocity[i],t)
			);
			level_text_chars_spr[i]->update_position(nullptr);
			level_text_chars_spr[i]->update_visual();
		}
		t++;
	}
	else if(t<100)
	{
		for(int i=0;i<level_text_chars_spr.size();i++)
		{
			level_text_chars_spr[i]->set_position(
				level_text_chars_spr[i]->pos_x(),			
				80
			);
			level_text_chars_spr[i]->update_position(nullptr);
			level_text_chars_spr[i]->update_visual();
		}
		t++;
	}
	else if(t<170)
	{
		for(int i=0;i<level_text_chars_spr.size();i++)
		{
			level_text_chars_spr[i]->set_position(
				level_text_chars_spr[i]->pos_x(), 
				level_text_chars_spr[i]->pos_y() + spr_velocity[i]
			);
			spr_velocity[i]+=rand()%10*0.02f;
			level_text_chars_spr[i]->update_position(nullptr);
			level_text_chars_spr[i]->update_visual();
		}
		t++;
	}
	else 
	{		
		level_animation_done = true;
		on_level_start();		
	}
}

void Level::frame() 
{	
	if(!level_animation_done)
	{
		level_display_animation();	
		OamPool::deploy();
	}
	else		
	{		
		Board::frame();	
	}
}

Level::~Level()
{
	for(int i=0;i<level_text_chars_spr.size();i++)
	{
		delete level_text_chars_spr[i];
	}
	objDisable();
}

void Level::on_level_start() { }