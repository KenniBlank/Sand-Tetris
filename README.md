# 2P-Sand-Tetris
Final Project of Computer Programming CT101
Source Code Location: https://github.com/KenniBlank/2P-Sand-Tetris

## Helpful Links

- Startup boiler plate template code: https://github.com/KenniBlank/sdl2-c-game-template/tree/main
- Why Virtual Resolution is set to what it is set at: https://codecreateplay.com/screen-resolutions-for-game-development/

- Font from: https://fonts.google.com/specimen/Pixelify+Sans

- Race Condition in update_sand_particles fixed! without another buffer (Not 100% sure but I think its correct)
- Flood fill algorithm: https://www.geeksforgeeks.org/dsa/flood-fill-algorithm/

## TODO

- [ ] Score, Level Implementation
        - [ ] Difficulty increase update!
- [X] PRIORITY: Switch to SDL_Texture for better rendering (It is closer to CPU for processing)
- [ ] Game Loss Condition
- [ ] Game UI Update: BG color, components...
- [ ] Documentation
- [ ] Score Board
- [ ] Audio Update
        - [ ] Game Sound
        - [ ] Default Sound
        - [ ] Score Sound (intensity according to score)
- [ ] Spawn Tetrimino at the center of Screen (In accordance with bitmap) (Both!: Next and Current)
- [ ] Better Tetrimino color (Lower the tetrimino,  higher the color (Basically saturation))
- [ ] Sand bounce physics?
- [ ] Network concept: update score globally

# Helpful Snippets:

```c
void* pixels;
int pitch;

SDL_LockTexture(tex, NULL, &pixels, &pitch);

// pixels = raw ARGB buffer
Uint32* p = (Uint32*)pixels;

for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
        p[y * (pitch / 4) + x] = 0xFF00FF00; // ARGB
    }
}

SDL_UnlockTexture(tex);
SDL_Rect dst = {
    GAME_POS_X,
    GAME_POS_Y,
    GAME_WIDTH,
    GAME_HEIGHT
};

SDL_RenderCopy(renderer, tex, NULL, &dst);
```

```c
static inline Uint32 color_to_pixel(SDL_PixelFormat* fmt, SDL_Color c) {
    return SDL_MapRGBA(fmt, c.r, c.g, c.b, c.a);
}

// Or store color in hex direct?
```

https://stackoverflow.com/questions/22886500/how-to-render-text-in-sdl2