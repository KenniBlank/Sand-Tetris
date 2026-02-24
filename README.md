# Sand-Tetris
Final Project of Computer Programming CT101
Source Code Location: https://github.com/KenniBlank/2P-Sand-Tetris

## Helpful Links

- Startup boiler plate template code: https://github.com/KenniBlank/sdl2-c-game-template/tree/main
- Why Virtual Resolution is set to what it is set at: https://codecreateplay.com/screen-resolutions-for-game-development/

- Font from: https://fonts.google.com/specimen/Pixelify+Sans
- Helpful: https://www.studyplan.dev/sdl2/sdl2-timers
- Race Condition in update_sand_particles fixed! without another buffer (Not 100% sure but I think its correct)
- Flood fill algorithm: https://www.geeksforgeeks.org/dsa/flood-fill-algorithm/
- Audio: https://www.studyplan.dev/sdl2/building-sdl/q/sdl2-audio-playback

- GameOverSound from freesound_community
- Background music: by <a href="https://pixabay.com/users/nocopyrightsound633-47610058/?utm_source=link-attribution&utm_medium=referral&utm_campaign=music&utm_content=323176">NoCopyrightSound633</a> from <a href="https://pixabay.com/music//?utm_source=link-attribution&utm_medium=referral&utm_campaign=music&utm_content=323176">Pixabay</a>
- Sand Clear Sound Effect by <a href="https://pixabay.com/users/freesound_community-46691455/?utm_source=link-attribution&utm_medium=referral&utm_campaign=music&utm_content=95847">freesound_community</a> from <a href="https://pixabay.com/sound-effects//?utm_source=link-attribution&utm_medium=referral&utm_campaign=music&utm_content=95847">Pixabay</a>


#  Compiling and Running

#  Prerequisites
Before run the program  ensure you have a C compiler and  sdl2  installed:</br>

Windows: MinGW-w64 or Visual Studio.</br>

macOS: Xcode Command Line Tools (xcode-select --install).</br>

Linux: gcc or clang.</br>

1.  Clone the Repository</br>
Open your terminal or command prompt and run:</br>

```Bash
git clone https://github.com/KenniBlank/Sand-Tetris.git
cd Sand-Tetris
```

2. Compiling and Running</br>
<Linux / macOS 🐧🍎 </br>
Use pkg-config to automatically handle the include paths and linking:</br>

```bash
gcc main.c -o my_program `sdl2-config --cflags --libs
./my_program
```

Windows🪟 (MinGW)</br>
You need to manually link the libraries and include the path:</br>

```bash
gcc main.c -o my_program -I/path/to/SDL2/include -L/path/to/SDL2/lib -lmingw32 -lSDL2main -lSDL2
./my_program</br>
```


> If you have make, a makefile is included for direct compilation + play
```bash
make
```
