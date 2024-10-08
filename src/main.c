#include <string.h>

#include "draw.h"
#include "init.h"
#include "input.h"
#include "sound.h"
#include "stage.h"
#include "structs.h"
#include "util.h"

static void cleanup(void);
static void capFrameRate(long* then, float* remainder);

App app;
Stage stage;
Entity *player;

int main(int argc, char *argv[])
{
    long then;

    float remainder;

	memset(&app, 0, sizeof(App));

	initSDL();

	atexit(cleanup);

	initStage();

	initSounds();

	then = SDL_GetTicks();

	remainder = 0;

	while (1)
	{
	    prepareScene();

		doInput();

		app.delegate.logic();
		
		app.delegate.draw();

		presentScene();

		capFrameRate(&then, &remainder);
	}
	return 0;
}

static void cleanup(void)
{
    SDL_Quit();
}

static void capFrameRate(long *then, float *remainder)
{
	long wait, frameTime;

	wait = 16 + *remainder;

	*remainder -= (int)*remainder;

	frameTime = SDL_GetTicks() - *then;

	wait -= frameTime;

	if (wait < 1)
	{
		wait = 1;
	}

	SDL_Delay(wait);

	*remainder += 0.667;

	*then = SDL_GetTicks();
}

