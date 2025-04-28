#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define SCREEN_W        800
#define SCREEN_H        600
#define MAX_RECTS       100
#define PALETTE_SIZE    8
#define PALETTE_H       40   // palette row height
#define PALETTE_W       40   // each color slot width

typedef struct { Uint8 r, g, b; } Color;
typedef struct {
    int x, y, w, h;
    Color color;
} Rect;

/* global SDL objects */
SDL_Window*   win = NULL;
SDL_Renderer* ren = NULL;

/* storage */
Rect   rects[MAX_RECTS];
int    rectCount = 0;

/* palette and current */
Color  palette[PALETTE_SIZE] = {
    {255, 0,   0},   // red
    {  0,255,   0},  // green
    {  0,  0,255},  // blue
    {255,255,  0},   // yellow
    {  0,255,255},   // cyan
    {255,  0,255},   // magenta
    {255,255,255},   // white
    {  0,  0,  0}    // black
};
Color  curColor = {255,0,0};

/* rectangle‐drawing state */
bool  selecting = true;
int   startX, startY;

static bool initSDL(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return false;
    }
    win = SDL_CreateWindow("Rectangles + Palette",
                           SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                           SCREEN_W, SCREEN_H,
                           SDL_WINDOW_SHOWN);
    if (!win) {
        fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
        return false;
    }
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (!ren) {
        fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

static void cleanupSDL(void) {
    if (ren) SDL_DestroyRenderer(ren);
    if (win) SDL_DestroyWindow(win);
    SDL_Quit();
}

static void drawAll(void) {
    /* clear to white */
    SDL_SetRenderDrawColor(ren, 255,255,255,255);
    SDL_RenderClear(ren);

    /* draw rectangles */
    for (int i = 0; i < rectCount; i++) {
        SDL_SetRenderDrawColor(ren,
            rects[i].color.r,
            rects[i].color.g,
            rects[i].color.b,
            255);
        SDL_Rect r = { rects[i].x, rects[i].y,
                       rects[i].w, rects[i].h };
        SDL_RenderFillRect(ren, &r);
    }

    /* draw palette row at top */
    for (int i = 0; i < PALETTE_SIZE; i++) {
        SDL_SetRenderDrawColor(ren,
            palette[i].r,
            palette[i].g,
            palette[i].b,
            255);
        SDL_Rect pr = { i * PALETTE_W, 0,
                        PALETTE_W,    PALETTE_H };
        SDL_RenderFillRect(ren, &pr);
        /* optional black border */
//        SDL_SetRenderDrawColor(ren,0,0,0,255);
//        SDL_RenderDrawRect(ren,&pr);
    }

    SDL_RenderPresent(ren);
}

int main(int argc, char* argv[]) {
    if (!initSDL()) {
        return 1;
    }

    SDL_Event ev;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_MOUSEBUTTONDOWN: {
                int mx = ev.button.x, my = ev.button.y;
                /* palette click? */
                if (my < PALETTE_H) {
                    int idx = mx / PALETTE_W;
                    if (idx >= 0 && idx < PALETTE_SIZE) {
                        curColor = palette[idx];
                    }
                }
                else {
                    /* drawing area click */
                    if (selecting) {
                        startX = mx;
                        startY = my;
                        selecting = false;
                    } else {
                        if (rectCount < MAX_RECTS) {
                            int ex = mx, ey = my;
                            Rect *R = &rects[rectCount++];
                            /* compute top-left & size */
                            R->x = (startX < ex)? startX : ex;
                            R->y = (startY < ey)? startY : ey;
                            R->w = abs(ex - startX);
                            R->h = abs(ey - startY);
                            R->color = curColor;
                        }
                        selecting = true;
                    }
                }
                break;
            }
            } // end switch
        } // end event loop

        drawAll();
        SDL_Delay(16);  /* ~60 fps */
    }

    cleanupSDL();
    return 0;
}

