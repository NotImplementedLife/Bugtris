#pragma once

#include <AstralbrewLib>

class Board : public Astralbrew::World::Scene
{
private:
	int k=0;	
public:
	virtual void init() override;	
	
	virtual void frame() override;
	
	~Board();
};