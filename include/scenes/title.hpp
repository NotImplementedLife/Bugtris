#pragma once

#include <Astralbrew>

using namespace Astralbrew;

#include "data/mesh.hpp"

#include "title_bg.h"
#include "title_fg.h"
#include "piece_tiles.h"

class TitleScene : public GenericScene
{
private:
	MapInfo* title_bg = require_map(3, &ROA_title_bg);
	MapInfo* title_fg = require_map(2, &ROA_title_fg);
	MapInfo* dialog_bg = require_text_map(1, 256, 256, 4, 2*10);
	
	VwfEngine vwf = VwfEngine(Resources::Fonts::default_8x16);

	ResourceInfo* pieces_gfx = require_asset(&ROA_piece_tiles, Video::Backgrounds(3));
	int first_piece_tile_id;
	
	Vector<Mesh> meshes;
	void create_meshes();
	void draw_mesh(const Mesh& mesh);
	
	void switch_start_visibility(void*, void*);
	BindingObjectScheduledTask switch_start_visibility_task { &TitleScene::switch_start_visibility, this, 30, 0 };
	
	virtual void on_key_down(void* sender, void* _keys);
public:
	virtual void init() override;	
	virtual void frame() override;	
	~TitleScene();
};