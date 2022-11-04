#include "gba_jam_scene.hpp"

#include "intro.hpp"

#include "gba_jam_logo.h"
#include "gba_jam_grid.h"

using namespace Astralbrew::Video;

void GBAJamScene::init() 
{
	#ifdef GBAJAM
	Video::setMode(0);
	bgInit(0, BgSize::Text256x256, BgPaletteType::Pal4bit, 0, 31);
	dmaCopy(gba_jam_logoTiles, (void*)0x06000000, gba_jam_logoTilesLen);
	dmaCopy(gba_jam_logoPal, BG_PALETTE, gba_jam_logoPalLen);
	dmaCopy(gba_jam_logoMap, bgGetMapPtr(0), gba_jam_logoMapLen);
	
	bgInit(1, BgSize::Text256x256, BgPaletteType::Pal4bit, 1, 30);
	dmaCopy(gba_jam_gridTiles, (void*)0x06004000, gba_jam_gridTilesLen);
	dmaCopy(gba_jam_gridPal, &BG_PALETTE[16], gba_jam_gridPalLen);
	dmaCopy(gba_jam_gridMap, bgGetMapPtr(1), gba_jam_gridMapLen);
	
	BG_PALETTE[0] = Drawing::Colors::White;
	#endif
}

#include <math.h>

void GBAJamScene::frame()
{	
	#ifdef GBAJAM
	bgSetScroll(0,0,(int)(5*sin(count*0.05f)));
	bgScroll(1,1,1);
	bgUpdate();
	count++;	
	if(count==180)
	{
	#endif
		BG_PALETTE[0] = Drawing::Colors::Black;
		bgSetScroll(0,0,0);
		bgSetScroll(1,0,0);
		close()->next(new Intro());
	#ifdef GBAJAM
	}
	#endif
}