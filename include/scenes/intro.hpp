#pragma once

#include <Astralbrew>

using namespace Astralbrew;

#include "intro_bg_galaxy.h"

class IntroScene : public GenericScene
{	
private:
	inline static constexpr int DIALOG_TILES_WIDTH  = 28;	
	MapInfo* bg = require_map(&ROA_intro_bg_galaxy);	
	VwfEngine vwf = VwfEngine(Resources::Fonts::default_8x16);
	int vwf_color = reserve_sprite_color(Colors::White);		
	short* dialog_fg_ptr;
	ObjFrame* dialog_fg_frames[DIALOG_TILES_WIDTH];
	
	struct DialogLine
	{
		const char* message;
		NonStaticEventHandler<IntroScene> line_finished_handler;
		DialogLine* next;
	};
	
	DialogLine* dialog_stream = nullptr;
	DialogLine dialog_lines[2] =
	{
		{ "Somewhere in a parallel universe, ...", &IntroScene::change_background, &dialog_lines[1] },
		{ "... a group of cats creates a Tetris game.", &IntroScene::end_scene, nullptr }
	};
	
	void change_background(void*, void*);
	void end_scene(void*, void*);
	void show_dialog(DialogLine* line);
	bool process_dialog();	
	
public:
	virtual void init() override;	
	virtual void frame() override;
	~IntroScene();
};