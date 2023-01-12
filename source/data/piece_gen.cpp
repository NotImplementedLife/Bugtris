#include "data/piece_gen.hpp"

#include <stdlib.h>

Piece PieceGenerator::next() const
{
	return { (unsigned char)(rand()%7), (unsigned char)(rand()%8), (unsigned char)(rand()%4) };
}

unsigned char PieceGenerator::rand_color()
{
	return rand()%8;
}

unsigned char PieceGenerator::rand_shape()
{
	return rand()%4;
}

unsigned char PieceGenerator::rand_piece()
{
	return rand()%7;
}