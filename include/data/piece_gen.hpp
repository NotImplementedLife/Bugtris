#pragma once

#include "data/piece.hpp"

class PieceGenerator
{
public:
	virtual Piece next() const;
	~PieceGenerator() = default;
};