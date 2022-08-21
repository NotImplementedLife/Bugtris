#pragma once

#include <Astralbrew>
#include "data/mesh.hpp"

using Astralbrew::World::Scene;
using Astralbrew::Memory::Address;
using Astralbrew::Memory::VramManager;
using Astralbrew::Vector;
using Astralbrew::Pair;

class Title : public Scene
{
private:
	VramManager vram_chr_1 = VramManager::from_char_block(1);
	Address bg_addr;
	Address fg_addr;
	Address piece_bg_addr;
	Vector<Mesh> meshes;
	
	int piece_tiles_start_id;
	
	Vector<Pair<int,int>> moves;
	
	void draw_mesh(const Mesh& m);
	bool mesh_collides(int i);
	void build();
public:
	virtual void init() override;	
	
	virtual void frame() override;
};