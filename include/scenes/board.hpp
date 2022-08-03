#pragma once

#include <AstralbrewLib>
#include "data/mesh.hpp"

class Board : public Astralbrew::World::Scene
{
private:
	Astralbrew::Vector<Mesh> meshes;
	Mesh* m[3];
	
	void draw_mesh(const Mesh& mesh);
public:
	virtual void init() override;	
	
	virtual void frame() override;
	
	~Board();
};