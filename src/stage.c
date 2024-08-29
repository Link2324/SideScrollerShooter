#include <stdio.h>

#include "defs.h"
#include "draw.h"
#include "sound.h"
#include "stage.h"
#include "structs.h"
#include "util.h"

// Initialize player
static void initPlayer();

// Resets stage
static void resetStage(void);

// Calls functions related to game logic
static void logic(void);

// The logic function calls these functions
static void doPlayer(void);
static void doFighters(void);
static void doBackground(void);
static void doBullets(void);
static void spawnEnemies(void);
static void doEnemies(void);
static void clipPlayer(void);
static void doExplosions(void);
static void doDebris(void);

// Sub functions for above function
static void fireBullet(void);
static void fireAlienBullet(Entity *e);
static int bulletHitFighter(Entity *b);
static void addExplosions(int x, int y, int num);
static void addDebris(Entity *e);

// Calls function related to drawing
static void draw(void);

// The draw function call these functions
static void drawBullets(void);
static void drawFighters(void);
static void drawBackground(void);
static void drawExplosions(void);
static void drawDebris(void);

// Pointer for Texture
static SDL_Texture *bulletTexture, *enemyTexture, *alienBulletTexture,
       *playerTexture, *background, *explosionTexture;

//
static int enemySpawnTimer, stageResetTimer, backgroundX;

static SDL_Rect back_rect;

void initStage(void)
{
    // Initialize stage

    app.delegate.logic = logic;
    app.delegate.draw = draw;

    memset(&stage, 0, sizeof(Stage));


    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;

    stage.explosionTail = &stage.explosionHead;
    stage.debrisTail = &stage.debrisHead;

    initPlayer();

    // Texture for drawing on surface
    alienBulletTexture = loadTexture("graphics/alienBullet.png");
    playerTexture = loadTexture("graphics/player.png");
    bulletTexture = loadTexture("graphics/playerBullet.png");
    enemyTexture = loadTexture("graphics/enemy.png");
    background = loadTexture("graphics/background.png");
    explosionTexture = loadTexture("graphics/explosion.png");

    SDL_QueryTexture(background, NULL, NULL, &back_rect.w, &back_rect.h);

    loadMusic("music/Mercury.ogg");

    playMusic(1);

    resetStage();
}

static void resetStage(void)
{
    // Resets stage

    Entity *e;
    Explosion *ex;
    Debris *d;

    while (stage.fighterHead.next)
    {
        e = stage.fighterHead.next;
        stage.fighterHead.next = e->next;
        free(e);
    }

    while (stage.bulletHead.next)
    {
        e = stage.bulletHead.next;
        stage.bulletHead.next = e->next;
        free(e);
    }

    while (stage.explosionHead.next)
    {
        ex = stage.explosionHead.next;
        stage.explosionHead.next = ex->next;
        free(ex);
    }

    while (stage.debrisHead.next)
    {
        d = stage.debrisHead.next;
        stage.debrisHead.next = d->next;
        free(d);
    }

    memset(&stage, 0, sizeof(Stage));

    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;

    stage.explosionTail = &stage.explosionHead;
    stage.debrisTail = &stage.debrisHead;

    initPlayer();

    enemySpawnTimer = 0;

    stageResetTimer = FPS * 3;
}

static void initPlayer()
{
    // Initializes player

    // Allocating memory for player
    player = malloc(sizeof(Entity));
    memset(player, 0, sizeof(Entity));
    stage.fighterTail->next = player;
    stage.fighterTail = player;

    player->x = 100;
    player->y = 100;
    player->health = 1;
    player->side = SIDE_PLAYER;
    player->texture = playerTexture;

    // Storing height and width of texture
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
}

static void logic(void)
{

    doBackground();

    doPlayer();

    doFighters();
    
    doBullets();

    spawnEnemies();

    doEnemies();

    clipPlayer();

    doExplosions();

    doDebris();
    if (player == NULL && --stageResetTimer <= 0)
    {
        resetStage();
    }
}

// This function contains logic related to player's action
static void doPlayer(void)
{
    if (player != NULL)
    {
        player->dx = player->dy = 0;
        if (player->reload > 0)
        {
            player->reload--;
        }

        if (app.keyboard[SDL_SCANCODE_UP])
        {
            player->dy = -PLAYER_SPEED;

        }

        if (app.keyboard[SDL_SCANCODE_DOWN])
        {
            player->dy = PLAYER_SPEED;
        }

        if (app.keyboard[SDL_SCANCODE_LEFT])
        {
            player->dx = -PLAYER_SPEED;
        }

        if (app.keyboard[SDL_SCANCODE_RIGHT])
        {
            player->dx = PLAYER_SPEED;
        }

        if (app.keyboard[SDL_SCANCODE_LCTRL] && player->reload == 0)
        {
            playSound(SND_PLAYER_FIRE, CH_PLAYER);
            fireBullet();
        }
        player->x += player->dx;
        player->y += player->dy;
    }
}

// Fires player bullet
static void fireBullet(void)
{
    Entity *bullet;

    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    bullet->x = player->x + player->w;
    bullet->y = player->y;
    bullet->side = SIDE_PLAYER;
    bullet->dx = PLAYER_BULLET_SPEED;
    bullet->health = 1;
    bullet->texture = bulletTexture;

    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    bullet->y += (player->h / 2) - (bullet->h / 2);
    player->reload = 8;
}

// Spawm ememies
static void spawnEnemies(void)
{
    Entity *enemy;

    if (--enemySpawnTimer <= 0)
    {
        enemy = malloc(sizeof(Entity));
        memset(enemy, 0, sizeof(Entity));
        stage.fighterTail->next = enemy;
        stage.fighterTail = enemy;

        enemy->texture = enemyTexture;
        SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);

        enemy->x = SCREEN_WIDTH;
        enemy->y = rand() % (SCREEN_HEIGHT - enemy->h);
        enemy->health = 1;
        enemy->side = SIDE_ALIEN;
        enemy->reload = FPS * (1 + (rand() % 3));
        enemy->dx = -(2 + (rand() % 4));

        enemySpawnTimer = 0 + (rand() % 150);
    }
}

// Contain logic for enemies
static void doEnemies(void)
{
    Entity *e;

    for (e = stage.fighterHead.next ; e != NULL ; e = e->next)
    {
        if (e != player && player != NULL && --e->reload <= 0)
        {
            fireAlienBullet(e);
            playSound(SND_ALIEN_FIRE, CH_ALIEN_FIRE);
        }
    }
}

// Fires enemy bullet
static void fireAlienBullet(Entity *e)
{
    Entity *bullet;

    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    bullet->x = e->x;
    bullet->y = e->y;
    bullet->health = 1;
    bullet->texture = alienBulletTexture;
    bullet->side = e->side;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    bullet->x += (e->w / 2) - (bullet->w / 2);
    bullet->y += (e->h / 2) - (bullet->h / 2);

    calcSlope(player->x + (player->w / 2), player->y + (player->h / 2), e->x, e->y, &bullet->dx, &bullet->dy);

    bullet->dx *= ALIEN_BULLET_SPEED;
    bullet->dy *= ALIEN_BULLET_SPEED;

    bullet->side = SIDE_ALIEN;

    e->reload = (rand() % FPS * 2);
}

//
static void doBackground(void)
{
    if (--backgroundX < -back_rect.w)
    {
        backgroundX = 0;
    }
}

//
static void doExplosions(void)
{
    Explosion *e, *prev;

    prev = &stage.explosionHead;

    for (e = stage.explosionHead.next ; e != NULL ; e = e->next)
    {
        e->x += e->dx;
        e->y += e->dy;

        if (--e->a <= 0)
        {
            if (e == stage.explosionTail)
            {
                stage.explosionTail = prev;
            }

            prev->next = e->next;
            free(e);
            e = prev;
        }

        prev = e;
    }
}

// Debris logic
static void doDebris(void)
{
    Debris *d, *prev;

    prev = &stage.debrisHead;

    for (d = stage.debrisHead.next ; d != NULL ; d = d->next)
    {
        d->x += d->dx;
        d->y += d->dy;

        d->dy += 0.5;

        if (--d->life <= 0)
        {
            if (d == stage.debrisTail)
            {
                stage.debrisTail = prev;
            }

            prev->next = d->next;
            free(d);
            d = prev;
        }

        prev = d;
    }
}

static void doFighters(void)
{
    Entity *e, *prev;

    prev = &stage.fighterHead;

    for (e = stage.fighterHead.next ; e != NULL ; e = e->next)
    {
        e->x += e->dx;
        e->y += e->dy;

        if (e->side == SIDE_ALIEN && e->x < -e->w)
        {
            e->health = 0;
        }

        if (e->health == 0)
        {
            if (e->side == SIDE_PLAYER)
            {
                addDebris(player);
                player = NULL;
            }

            if (e->side == SIDE_ALIEN)
            {
                addExplosions(e->x, e->y, 10);
            }

            if (e == stage.fighterTail)
            {
                stage.fighterTail = prev;
            }

            prev->next = e->next;
            free(e);
            e = prev;
        }

        prev = e;
    }
}



static void doBullets(void)
{
    Entity *b, *prev;

    prev = &stage.bulletHead;

    for (b = stage.bulletHead.next ; b != NULL ; b = b->next)
    {
        b->x += b->dx;
        b->y += b->dy;

        if (bulletHitFighter(b) || b->x < -b->w || b->y < -b->h || b->x > SCREEN_WIDTH || b->y > SCREEN_HEIGHT)
        {
            if (b->side == SIDE_PLAYER)
            {
                playSound(SND_PLAYER_DIE, CH_PLAYER);
            }

            else
            {
                playSound(SND_ALIEN_DIE, CH_ANY);
            }

            if (b == stage.bulletTail)
            {
                stage.bulletTail = prev;
            }

            prev->next = b->next;
            free(b);
            b = prev;
        }

        prev = b;
    }
}

static int bulletHitFighter(Entity *b)
{
    Entity *e;

    for (e = stage.fighterHead.next ; e != NULL ; e = e->next)
    {
        if (e->side != b->side && collision(b->x, b->y, b->w, b->h, e->x, e->y, e->w, e->h))
        {
            b->health = 0;
            e->health--;

            return 1;
        }
    }

    return 0;
}

static void draw(void)
{
    drawBackground();

    drawDebris();

    drawExplosions();

    drawBullets();

    drawFighters();
}

static void drawBullets(void)
{
    Entity *b;

    for (b = stage.bulletHead.next ; b != NULL ; b = b->next)
    {
        blit(b->texture, b->x, b->y);
    }
}

static void drawFighters(void)
{
    Entity *e;

    for (e = stage.fighterHead.next ; e != NULL ; e = e->next)
    {
        blit(e->texture, e->x, e->y);
    }
}

static void clipPlayer(void)
{
    if (player != NULL)
    {
        if (player->x < 0)
        {
            player->x = 0;
        }

        if (player->y < 0)
        {
            player->y = 0;
        }

        if (player->x > SCREEN_WIDTH / 2)
        {
            player->x = SCREEN_WIDTH / 2;
        }

        if (player->y > SCREEN_HEIGHT - player->h)
        {
            player->y = SCREEN_HEIGHT - player->h;
        }
    }
}

static void addExplosions(int x, int y, int num)
{
    Explosion *e;
    int i;

    for (i = 0 ; i < num ; i++)
    {
        e = malloc(sizeof(Explosion));
        memset(e, 0, sizeof(Explosion));
        stage.explosionTail->next = e;
        stage.explosionTail = e;

        e->x = x + (rand() % 32) - (rand() % 32);
        e->y = y + (rand() % 32) - (rand() % 32);
        e->dx = (rand() % 10) - (rand() % 10);
        e->dy = (rand() % 10) - (rand() % 10);

        e->dx /= 10;
        e->dy /= 10;

        switch (rand() % 4)
        {
        case 0:
            e->r = 255;
            break;

        case 1:
            e->r = 255;
            e->g = 128;
            break;

        case 2:
            e->r = 255;
            e->g = 255;
            break;

        default:
            e->r = 255;
            e->g = 255;
            e->b = 255;
            break;
        }

        e->a = rand() % FPS * 3;
    }
}

static void addDebris(Entity *e)
{
    Debris *d;
    int x, y, w, h;

    w = e->w / 2;
    h = e->h / 2;

    for (y = 0 ; y <= h ; y += h)
    {
        for (x = 0 ; x <= w ; x += w)
        {
            d = malloc(sizeof(Debris));
            memset(d, 0, sizeof(Debris));
            stage.debrisTail->next = d;
            stage.debrisTail = d;

            d->x = e->x + e->w / 2;
            d->y = e->y + e->h / 2;
            d->dx = (rand() % 5) - (rand() % 5);
            d->dy = -(5 + (rand() % 12));
            d->life = FPS * 2;
            d->texture = e->texture;

            d->rect.x = x;
            d->rect.y = y;
            d->rect.w = w;
            d->rect.h = h;
        }
    }
}

static void drawBackground(void)
{
    SDL_Rect dest, src;
    int x;

    for (x = backgroundX ; x < SCREEN_WIDTH ; x += SCREEN_WIDTH)
    {
        dest.x = x;
        dest.y = 0;
        dest.w = SCREEN_WIDTH;
        dest.h = SCREEN_HEIGHT;

        SDL_RenderCopy(app.renderer, background, NULL, &dest);
    }
}

static void drawDebris(void)
{
    Debris *d;

    for (d = stage.debrisHead.next ; d != NULL ; d = d->next)
    {
        blitRect(d->texture, &d->rect, d->x, d->y);
    }
}

static void drawExplosions(void)
{
    Explosion *e;

    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
    SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_ADD);

    for (e = stage.explosionHead.next ; e != NULL ; e = e->next)
    {
        SDL_SetTextureColorMod(explosionTexture, e->r, e->g, e->b);
        SDL_SetTextureAlphaMod(explosionTexture, e->a);

        blit(explosionTexture, e->x, e->y);
    }

    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
}
