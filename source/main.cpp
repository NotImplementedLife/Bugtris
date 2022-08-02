#include <AstralbrewLib>
#include <stdio.h>

using namespace Astralbrew;
using namespace Astralbrew::Video;

#include "board-bg.h"

class CustomScene : public Astralbrew::World::Scene
{
private:
	int k=0;
public:
	virtual void init() override
	{
		Video::setMode(0);
		bgInit(3, Text256x256, Pal8bit, 1, 0);
		bgWrap(3);
		dmaCopy(board_bgTiles, CHAR_BASE_ADR(1), board_bgTilesLen);
		dmaCopy(board_bgMap, MAP_BASE_ADR(0), board_bgMapLen);
		dmaCopy(board_bgPal, BG_PALETTE, board_bgPalLen);		
		
		bgSetScroll(3, 8, 48);
		bgUpdate();
	}
	
	virtual void frame() override
	{
		
	}
};

astralbrew_launch(CustomScene);

/*int main(void) 
{
	irqInit();
	irqEnable(IRQ_VBLANK);

	consoleDemoInit();	

	while (1) 
	{
		VBlankIntrWait();
	}
}*/
