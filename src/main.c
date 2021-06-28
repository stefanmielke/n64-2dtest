#include <nusys.h>

#include "definitions.h"
#include "screens/stage00.h"

/* Declaration of the prototype */
void vsyncCallback(int);

/*------------------------
	Main
--------------------------*/
void mainproc(void) {
	/* The initialization of graphic  */
	nuGfxInit();

	stage00_init();

	/* Register call-back  */
	nuGfxFuncSet((NUGfxFunc)vsyncCallback);
	/* The screen display ON */
	nuGfxDisplayOn();

	while (1)
		;
}

/*-----------------------------------------------------------------------------
  The call-back function

  pendingGfx which is passed from Nusystem as the argument of the call-back
  function is the total number of RCP tasks that are currently processing
  and waiting for the process.
-----------------------------------------------------------------------------*/
void vsyncCallback(int pendingGfx) {
	stage00_update();
	/* It provides the display process if there is no RCP task that is processing. */
	if (pendingGfx < 1)
		stage00_draw();
}
