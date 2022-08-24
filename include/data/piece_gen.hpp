#pragma once

#include "data/piece.hpp"

class PieceGenerator
{
public:
	virtual Piece next() const;
	virtual ~PieceGenerator() = default;
	
	static unsigned char rand_color();
	static unsigned char rand_shape();
};