#include "data/piece_gen.hpp"

#include <stdlib.h>

Piece PieceGenerator::next() const
{
	return { (u8)(rand()%7), (u8)(rand()%8), (u8)(rand()%4) };
}