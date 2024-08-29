#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED

#include <SDL_image.h>

void prepareScene(void);
void presentScene(void);
SDL_Texture *loadTexture(char *filename);
void blit(SDL_Texture *texture, int x, int y);
void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y);

#endif // DRAW_H_INCLUDED
