#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED
    
#define SIDE_PLAYER 0
#define SIDE_ALIEN  1
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define PLAYER_SPEED 2
#define PLAYER_BULLET_SPEED 16
#define MAX_KEYBOARD_KEYS 350
#define MAX(x,y) ((x>y)?x:y)
#define MIN(x,y) ((x>y)?y:x)
#define FPS 60
#define ALIEN_BULLET_SPEED 8
#define MAX_SND_CHANNELS 8


enum
{
	CH_ANY = -1,
	CH_PLAYER,
	CH_ALIEN_FIRE
};

enum
{
	SND_PLAYER_FIRE,
	SND_ALIEN_FIRE,
	SND_PLAYER_DIE,
	SND_ALIEN_DIE,
	SND_MAX
};

#endif // DEFS_H_INCLUDED
