#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED
#include "common.h"
#include "defs.h"

typedef struct {
	void (*logic)(void);
	void (*draw)(void);
} Delegate;

typedef struct {
	SDL_Renderer *renderer;
	SDL_Window *window;
	Delegate delegate;
	int keyboard[MAX_KEYBOARD_KEYS];
} App;

typedef struct Entity Entity;

struct Entity {
	float x;
	float y;
	int w;
	int h;
	float dx;
	float dy;
	int side;
	int health;
	int reload;
	SDL_Texture *texture;
	Entity *next;
};

typedef struct Explosion Explosion;

struct Explosion {
	float x;
	float y;
	float dx;
	float dy;
	int r, g, b, a;
	Explosion *next;
};

typedef struct Debris Debris;

struct Debris {
	float x;
	float y;
	float dx;
	float dy;
	SDL_Rect rect;
	SDL_Texture *texture;
	int life;
	Debris *next;
};

typedef struct
{
	Entity fighterHead, *fighterTail;
	Entity bulletHead, *bulletTail;
	Explosion explosionHead, *explosionTail;
	Debris debrisHead, *debrisTail;
} Stage;

typedef struct {
	int x;
	int y;
	int speed;
} Star;

App app;
Stage stage;
Entity *player;

#endif // STRUCTS_H_INCLUDED

