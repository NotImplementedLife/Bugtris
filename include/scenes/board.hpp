#pragma once

#include <AstralbrewLib>
#include "data/mesh.hpp"

class Board : public Astralbrew::World::Scene
{
private:
	Astralbrew::Vector<Mesh> meshes;
	Mesh board_mesh;
	Mesh* user_controllable_mesh = NULL;
	
	void clear();
	void draw_mesh(const Mesh& mesh);
	void spawn_mesh(int gid, int color, int shape);
	
	bool ucm_in_bounds();
public:
	virtual void init() override;	
	
	virtual void frame() override;
	
	virtual void on_key_down(int keys) override;
	
	~Board();
};