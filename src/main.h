#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
#include "init.h"
#include "draw.h"
#include "input.h"
#include "common.h"
#include "structs.h"
#include "stage.h"
#include "util.h"
#include "sound.h"

static void cleanup(void);
static void capFrameRate(long *then, float *remainder);

#endif // MAIN_H_INCLUDED
