#include <nusys.h>

#include "../graphic.h"
#include "../definitions.h"

u16 square_pos_x, square_pos_y;
s8 square_speed;

void clear_background(u8 r, u8 g, u8 b);
void draw_rect(u16 x, u16 y, u16 w, u16 h, u8 r, u8 g, u8 b);

void stage00_init(void) {
	square_pos_x = 0;
	square_pos_y = (SCREEN_HT / 2) - 32;
	square_speed = 2;
}

void stage00_update(void) {
	square_pos_x += square_speed;

	// bounce
	// if (square_pos_x >= SCREEN_WD - 64 || square_pos_x <= 0) {
	// 	square_speed *= -1;
	// }

	// warp
	if (square_pos_x >= SCREEN_WD - 64) {
		square_pos_x = 0;
	}
}

void stage00_draw(void) {
	glistp = glist;
	rcp_init(glistp);

	clear_background(0, 0, 255);

	draw_rect(square_pos_x, square_pos_y, 64, 64, 255, 0, 0);

	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);
	nuGfxTaskStart(glist, (s32)(glistp - glist) * sizeof(Gfx), NU_GFX_UCODE_F3DEX,
				   NU_SC_SWAPBUFFER);
}

void clear_background(u8 r, u8 g, u8 b) {
	gDPSetCycleType(glistp++, G_CYC_FILL);
	gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
					 osVirtualToPhysical(nuGfxCfb_ptr));
	gDPSetFillColor(glistp++, (GPACK_RGBA5551(r, g, b, 1) << 16 | GPACK_RGBA5551(r, g, b, 1)));
	gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
	gDPPipeSync(glistp++);
}

void draw_rect(u16 x, u16 y, u16 w, u16 h, u8 r, u8 g, u8 b) {
	gDPSetCycleType(glistp++, G_CYC_FILL);
	gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
					 osVirtualToPhysical(nuGfxCfb_ptr));
	gDPSetFillColor(glistp++, (GPACK_RGBA5551(r, g, b, 1) << 16 | GPACK_RGBA5551(r, g, b, 1)));
	gDPFillRectangle(glistp++, x, y, x + w, y + h);
	gDPPipeSync(glistp++);
}

/*
[/] Draw a red 64x64 square on top of the blue background, centered in the screen
[/] Make the square move left and right, bouncing on the sides of the screen
[ ] Make the square, instead of bouncing off the screen, wrap around to the other side.
*/