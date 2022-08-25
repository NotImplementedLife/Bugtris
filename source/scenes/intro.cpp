#include "scenes/intro.hpp"
#include "scenes/title.hpp"

using namespace Astralbrew;
using namespace Astralbrew::Video;
using namespace Astralbrew::Utils;

#include "intro-bg-galaxy.h"

void Intro::init()
{
	Video::setMode(3);
	
	bgInit(2, Text256x256, Pal4bit, 0, 0);
	
	dmaCopy(intro_bg_galaxyBitmap, (void*)0x06000000, intro_bg_galaxyBitmapLen);
}


void Intro::frame()
{
	if(ready)
	{
		close()->next(new Title());
	}
}

void Intro::on_key_down(int keys)
{
	if(keys & KEY_START)
	{
		ready=true;
	}
}

Intro::~Intro()
{	
	zeroize((void*)0x06000000, intro_bg_galaxyBitmapLen);
}