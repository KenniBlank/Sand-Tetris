#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Audio Specific
#define BG_MUSIC "./assets/Audio/Music/Bg_music.mp3"
#define MAX_CACHED_MUSIC 1

#define SFX_COUNTER_SOUND "./assets/Audio/SFX/Counter_Sound.wav"
#define SFX_GAME_OVER "./assets/Audio/SFX/Game_Over.wav"
#define SFX_SAND_CLEAR "./assets/Audio/SFX/Sand_clear.wav"
#define MAX_CACHED_SFX 3

// Audio constants
// 0-128
#define DEFAULT_MUSIC_VOLUME 20
#define DEFAULT_SFX_VOLUME 64

// Structure to hold cached audio
typedef struct CachedAudio {
        char* path;
        Mix_Chunk* sfx;
        Mix_Music* music;
        int is_music;
        struct CachedAudio* next; // Linked List
} CachedAudio;

typedef struct {
        int music_volume;
        int sfx_volume;

        CachedAudio* audio_cache;
        int is_initialized;
} AudioData;

typedef struct {
        int x, y;       // top-left of slider bar
        int w, h;       // width & height of slider bar
        int handleW;    // width of handle
        int handleX;    // current handle position
        int volume;     // 0 to MIX_MAX_VOLUME
} AudioSlider;

int audio_init(AudioData* audio);
void audio_playMusic(AudioData* audio, const char* path);
void audio_playSFX(AudioData* audio, const char* path);
void audio_stopMusic(AudioData* audio);
void audio_setMusicVolume(AudioData* audio, int volume);
void audio_setSFXVolume(AudioData* audio, int volume);
void audio_cleanup(AudioData* audio);

void updateSliderMusic(AudioSlider *slider, AudioData *audio, int mouseX, int mouseY, int mouseState);
void updateSliderSFX(AudioSlider *slider, AudioData *audio, int mouseX, int mouseY, int mouseState);
void renderSlider(SDL_Renderer *renderer, AudioSlider *slider);

#endif