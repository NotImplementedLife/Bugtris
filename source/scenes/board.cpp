#include "scenes/board.hpp"

using namespace Astralbrew;
using namespace Astralbrew::Video;

#include "board-bg.h"
#include "piece_tiles.h"

void Board::init()
{
	Video::setMode(0);
	bgInit(3, Text256x256, Pal8bit, 1, 0);
	bgWrap(3);
	dmaCopy(board_bgTiles, CHAR_BASE_ADR(1), board_bgTilesLen);
	dmaCopy(board_bgMap, MAP_BASE_ADR(0), board_bgMapLen);
	dmaCopy(board_bgPal, BG_PALETTE, board_bgPalLen);	
	
	copyTiles256(piece_tilesTiles, CHAR_BASE_OFFSET(1, board_bgTilesLen), piece_tilesTilesLen, piece_tilesPal, piece_tilesPalLen, 64);	
	
	bgSetScroll(3, 8, 48);
	bgUpdate();	
}

void Board::frame()
{
	
}

Board::~Board()
{
	
}