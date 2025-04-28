#include <SDL2/SDL.h>
#include <stdio.h>

#define MAX_LINE_COUNT 100
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct {
    Uint8 r, g, b;
} Color;

typedef struct {
    int x1, y1, x2, y2;
    Color color;
} Line;

typedef struct {
    SDL_Rect rect;
    Color color;
} ColorButton;

ColorButton palette[] = {
    {{10, 10, 30, 30}, {255, 0, 0}},   // piros
    {{50, 10, 30, 30}, {0, 255, 0}},   // zöld
    {{90, 10, 30, 30}, {0, 0, 255}}    // kék
};
int paletteSize = sizeof(palette) / sizeof(palette[0]);

Line lines[MAX_LINE_COUNT];
int lineCount = 0;
int selectingFirstPoint = 1;
int tempX, tempY; // ideiglenes tároló az első végpontra

Color currentColor = {255, 0, 0}; // alapértelmezett: piros

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Szakaszrajzolo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_bool running = SDL_TRUE;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = SDL_FALSE;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
            
                // Először nézzük meg, a palettán kattintottunk-e
                int paletteClicked = 0;
                for (int i = 0; i < paletteSize; i++) {
                    if (mouseX >= palette[i].rect.x && mouseX <= palette[i].rect.x + palette[i].rect.w &&
                        mouseY >= palette[i].rect.y && mouseY <= palette[i].rect.y + palette[i].rect.h) {
                        currentColor = palette[i].color;
                        paletteClicked = 1;
                        break;
                    }
                }
            
                if (!paletteClicked) {
                    // Ha nem palettán kattintottunk, akkor szakaszpont megadása
                    if (selectingFirstPoint) {
                        tempX = mouseX;
                        tempY = mouseY;
                        selectingFirstPoint = 0;
                    } else {
                        if (lineCount < MAX_LINE_COUNT) {
                            lines[lineCount].x1 = tempX;
                            lines[lineCount].y1 = tempY;
                            lines[lineCount].x2 = mouseX;
                            lines[lineCount].y2 = mouseY;
                            lines[lineCount].color = currentColor;
                            lineCount++;
                        }
                        selectingFirstPoint = 1;
                    }
                }
            }
        }

        // Rajzolás
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // fehér háttér
        SDL_RenderClear(renderer);

        // Paletta kirajzolása
        for (int i = 0; i < paletteSize; i++) {
        SDL_SetRenderDrawColor(renderer, palette[i].color.r, palette[i].color.g, palette[i].color.b, 255);
        SDL_RenderFillRect(renderer, &palette[i].rect);
        }


        for (int i = 0; i < lineCount; i++) {
            SDL_SetRenderDrawColor(renderer, lines[i].color.r, lines[i].color.g, lines[i].color.b, 255);
            SDL_RenderDrawLine(renderer, lines[i].x1, lines[i].y1, lines[i].x2, lines[i].y2);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
