#include <nusys.h>

#include "../graphic.h"
#include "../definitions.h"

#include "../data/spr_pyoro_walk1.h"

u16 pyoro_pos_x, pyoro_pos_y;
u8 b;

void clear_background(u8 r, u8 g, u8 b);
void draw_rect(u16 x, u16 y, u16 w, u16 h, u8 r, u8 g, u8 b);
void draw_pyoro(u16 x, u16 y);

void stage00_init(void) {
	b = 255;
	pyoro_pos_x = 32;
	pyoro_pos_y = 32;
}

void stage00_update(void) {
	b -= 1;
}

void stage00_draw(void) {
	glistp = glist;
	rcp_init(glistp);

	clear_background(32, 32, b);
	draw_pyoro(pyoro_pos_x, pyoro_pos_y);

	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);
	nuGfxTaskStart(glist, (s32)(glistp - glist) * sizeof(Gfx), NU_GFX_UCODE_F3DEX2,
				   NU_SC_SWAPBUFFER);
}

void clear_background(u8 r, u8 g, u8 b) {
	gDPSetCycleType(glistp++, G_CYC_FILL);
	gDPSetDepthImage(glistp++, nuGfxCfb_ptr);
	gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
					 osVirtualToPhysical(nuGfxCfb_ptr));
	gDPSetFillColor(glistp++, (GPACK_RGBA5551(r, g, b, 1) << 16 | GPACK_RGBA5551(r, g, b, 1)));
	gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
	gDPPipeSync(glistp++);
}

void draw_rect(u16 x, u16 y, u16 w, u16 h, u8 r, u8 g, u8 b) {
	gDPSetCycleType(glistp++, G_CYC_FILL);
	gDPSetDepthImage(glistp++, nuGfxCfb_ptr);
	gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
					 osVirtualToPhysical(nuGfxZBuffer));
	gDPSetFillColor(glistp++, (GPACK_RGBA5551(r, g, b, 1) << 16 | GPACK_RGBA5551(r, g, b, 1)));
	gDPFillRectangle(glistp++, x, y, x + w, y + h);
	gDPPipeSync(glistp++);
}

void draw_pyoro(u16 x, u16 y) {
	gDPSetCycleType(glistp++, G_CYC_1CYCLE);
	gDPSetCombineMode(glistp++, G_CC_DECALRGBA, G_CC_DECALRGBA);
	gDPSetRenderMode(glistp++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE);
	gDPSetDepthSource(glistp++, G_ZS_PRIM);
	gDPSetPrimDepth(glistp++, 0, 0);
	gDPSetTexturePersp(glistp++, G_TP_NONE);
	gDPLoadTextureBlock(glistp++, spr_pyoro_walk1, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0,
						G_TX_WRAP, G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
	gSPTextureRectangle(glistp++, x - 8 << 2, y - 8 << 2, (x + 8) << 2, (y + 8) << 2,
						G_TX_RENDERTILE, 0 << 5, 0 << 5, 1 << 10, 1 << 10);
	gDPPipeSync(glistp++);
}
