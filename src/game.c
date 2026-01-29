#include "game.h"
#include "config.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <stdint.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define unpack_color(color) (color.r), (color.g), (color.b), (color.a)
#define randColor() (rand() % COLOR_COUNT)
#define randRotation() (rand() % 4) // 4 rotations total so MAGIC NUMBER

// Temporary, only for testing
SandBlock SimulationBlock = {
        .x = (GAME_POS_X + GAME_WIDTH - PARTICLE_COUNT_IN_BLOCK_COLUMN) / 2.0f,
        .y = GAME_POS_Y + GAME_PADDING,
        .color = COLOR_GREEN,
        .velY = GRAVITY
};

// Return Color for colorcode
static SDL_Color enumToColor(ColorCode CC);

// One time Function
static inline void InitializeTetriminoCollection(TetrominoCollection* TC);
static inline void CleanUpTetriminoCollection(TetrominoCollection* TC);

// This function called to create new tetrimino
// For currentTetrimino once in init
// For every nextTetrimino determination
static void InitializeTetriminoData(TetrominoCollection* TC, TetrominoData* TD, unsigned x, unsigned y) {
        TD->shape = &TC->tetrominos[rand() % TC->count]; // Chosing 1 of random tetrimino from the collection

        TD->color = randColor();
        TD->rotation = randRotation();

        TD->x = x;
        TD->y = y;
}

static void InitializeBlocks(SandBlock* block, int x, int y) {
        block->x = x;
        block->y = y;
        block->velY = 0;
        block->color = randColor();
}

bool game_init(GameContext* GC) {
        srand(time(NULL)); // Seeding the random with current time
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
                fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
                return false;
        }

        SDL_Window* window = SDL_CreateWindow(
                "2P Sand Tetris",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                WINDOW_WIDTH,
                WINDOW_HEIGHT,
                SDL_WINDOW_RESIZABLE
        );

        if (!window) {
                fprintf(stderr, "Window error: %s\n", SDL_GetError());
                SDL_Quit();
                return false;
        }

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
                fprintf(stderr, "Renderer error: %s\n", SDL_GetError());
                SDL_DestroyWindow(window);
                SDL_Quit();
                return false;
        }
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        // Game Context Initialization
        GC->renderer = renderer;
        GC->window = window;
        GC->running = true;
        GC->last_time = SDL_GetTicks();
        GC->delta_time = 0.0f;

        // Game Data Initialization
        GameData* GD = &GC->gameData;
        GD->gameOver = false;
        GD->level = 0;
        GD->score = 0;

        for (int i = 0; i < GAME_HEIGHT; i++) {
                for (int j = 0; j < GAME_HEIGHT; j++) {
                        GD->colorGrid[i][j] = COLOR_NONE;
                }
        }

        // Initialize Collection (1 time function so inline)
        InitializeTetriminoCollection(&GD->tetrominoCollection);

        // Initialize Current and Next Tetrimono
        unsigned x = 100, y = 100; // TODO: fix this to be in accordance to where to spawn
        InitializeTetriminoData(&GD->tetrominoCollection, &GD->currentTetromino, x, y);
        InitializeTetriminoData(&GD->tetrominoCollection, &GD->nextTetromino, x, y);

        // Setting up virtual resolution
        SDL_RenderSetLogicalSize(GC->renderer, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
        SDL_RenderSetIntegerScale(GC->renderer, SDL_TRUE);
        return true;
}

void game_handle_events(GameContext* GC) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
                switch (event.type) {
                        case SDL_QUIT: {
                                GC->running = false;
                                break;
                        }

                        case SDL_KEYDOWN: {
                                switch (event.key.keysym.sym) {
                                        case SDLK_ESCAPE: {
                                                if (DEBUG) {
                                                        GC->running = false;
                                                }
                                                break;
                                        }

                                        case (SDLK_LEFT): {
                                                SimulationBlock.x = SDL_clamp(SimulationBlock.x - 2, GAME_POS_X, GAME_POS_X + GAME_WIDTH);
                                                break;
                                        }

                                        case (SDLK_RIGHT): {
                                                SimulationBlock.x = SDL_clamp(SimulationBlock.x + 2, GAME_POS_X, GAME_POS_X + GAME_WIDTH - PARTICLE_COUNT_IN_BLOCK_COLUMN);
                                                break;
                                        }
                                }
                                break;
                        }
                }
        }
}

void game_update(GameContext* GC) {
        SimulationBlock.velY += GRAVITY * GC->delta_time;
        SimulationBlock.y += SimulationBlock.velY * GC->delta_time;
        // printf("dt=%f velY=%f y=%f\n", GC->delta_time, SimulationBlock.velY, SimulationBlock.y);

        float floorY = VIRTUAL_HEIGHT - GAME_PADDING;
        if (SimulationBlock.y >= floorY - PARTICLE_COUNT_IN_BLOCK_ROW) {
                SimulationBlock.y = floorY - PARTICLE_COUNT_IN_BLOCK_ROW;

                if (fabsf(SimulationBlock.velY) < GRAVITY) {
                        SimulationBlock.velY = 0.0f;
                } else {
                        SimulationBlock.velY *= -0.5f;
                }
        }
}

static void renderSandBlock(SDL_Renderer* renderer, SandBlock* SB) {
        // TODO: set value of color in accordance to sandblock's particle color
        SDL_SetRenderDrawColor(renderer, unpack_color(enumToColor(SB->color)));

        SDL_Rect SB_Rect = {
                .x = (int) SB->x,
                .y = (int) SB->y,
                .w = PARTICLE_COUNT_IN_BLOCK_COLUMN,
                .h = PARTICLE_COUNT_IN_BLOCK_ROW
        };
        SDL_RenderFillRect(renderer, &SB_Rect);
}

static void gameRenderDebug(SDL_Renderer* renderer) {
        if (!DEBUG) return;
        SDL_SetRenderDrawColor(renderer, unpack_color(enumToColor(COLOR_RED)));
        SDL_Rect r = { 0, 0, VIRTUAL_WIDTH, VIRTUAL_HEIGHT };
        SDL_RenderDrawRect(renderer, &r);

        r.w = (r.w / 3) * 2;
        SDL_RenderDrawRect(renderer, &r);

        r.x = GAME_POS_X - 1;
        r.y = GAME_POS_Y - 1;
        r.w = GAME_WIDTH + 2;
        r.h = GAME_HEIGHT + 2;
        SDL_RenderDrawRect(renderer, &r);
}

static void renderAllParticles(SDL_Renderer* renderer, int colorGrid[GAME_HEIGHT][GAME_WIDTH]) {
        // TODO: Optimize this!
        for (int y = 0; y < GAME_HEIGHT; y++) {
                for (int x = 0; x < GAME_WIDTH; x++) {
                        SDL_SetRenderDrawColor(renderer, unpack_color(enumToColor(colorGrid[y][x])));
                        SDL_RenderDrawPoint(renderer, GAME_POS_X + x, GAME_POS_Y + y);
                }
        }
}

void game_render(GameContext* GC) {
        // Clear to BLACK
        SDL_SetRenderDrawColor(GC->renderer, 0, 0, 0, 255);
        SDL_RenderClear(GC->renderer);

        gameRenderDebug(GC->renderer);

        renderAllParticles(GC->renderer, GC->gameData.colorGrid);
        renderSandBlock(GC->renderer, &SimulationBlock);

        // Display modified renderer
        SDL_RenderPresent(GC->renderer);
}

void game_cleanup(GameContext* GC) {
        CleanUpTetriminoCollection(&GC->gameData.tetrominoCollection);

        SDL_DestroyRenderer(GC->renderer);
        SDL_DestroyWindow(GC->window);
        SDL_Quit();
}

static inline void InitializeTetriminoCollection(TetrominoCollection* TC) {
        TC->capacity = 4; // 4 Tetriminos: | Shaped, Z Shaped, Square Shaped, L Shape
        TC->tetrominos = malloc(sizeof(struct Tetromino) * TC->capacity);
        TC->count = 0;
        TC->tetrominos[TC->count++] = (struct Tetromino) {
                .name = "Line Tetrimino", // Display Name!
                .shape = {
        { // Rotation 1: rotation left of rotation 4
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        },
        { // Rotation 2: rotation left of rotation 1
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        },
        { // Rotation 3: rotation left of rotation 2
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        },
        { // Rotation 4: rotation left of rotation 3
        {0, 0, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 1, 0},
        }
                }
        };
        // TODO: Other Blocks
}

static inline void CleanUpTetriminoCollection(TetrominoCollection* TC) {
        if (TC->tetrominos != NULL) {
                free(TC->tetrominos);
        }
}

static SDL_Color enumToColor(ColorCode CC){
        SDL_Color color = {0};

        switch (CC) {
                case COLOR_RED: {
                        color = (SDL_Color) {
                                .r = 255,
                                .g = 0,
                                .b = 0,
                                .a = 255
                        };
                        break;
                }


                case COLOR_GREEN: {
                        color = (SDL_Color) {
                                .r = 255,
                                .g = 0,
                                .b = 0,
                                .a = 255
                        };
                        break;
                }

                case COLOR_BLUE: {
                        color = (SDL_Color) {
                                .r = 0,
                                .g = 0,
                                .b = 255,
                                .a = 255
                        };
                        break;
                }

                case COLOR_NONE: {
                        color = (SDL_Color) {
                                .r = 0,
                                .g = 0,
                                .b = 0,
                                .a = 0
                        };
                };

                default: {
                        color = (SDL_Color) {
                                .r = 0,
                                .g = 0,
                                .b = 0,
                                .a = 255
                        };
                };
        }

        return color;
}
