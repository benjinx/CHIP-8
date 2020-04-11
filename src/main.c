#include <string.h>

#include "cpu.h"
#include "log.h"
#include "registers.h"
#include "mmu.h"
#include "video.h"

#include <SDL.h>

// argc = number of arguments, argv = the arguments
int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        LoadROM(argv[1]);
    }

    PC = 0x200;

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window;
    int SCALE = 10;

    window = SDL_CreateWindow(
        "CHIP-8",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH * SCALE,
        HEIGHT * SCALE,
        0);

    SDL_Renderer* r = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
    if (!r)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", "Renderer not created.", window);
        exit(1);
    }

    SDL_Texture* tex = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 256, 256);

    
    uint8_t* pixels = NULL;
    int pitch = 0;
    SDL_LockTexture(tex, NULL, (void**)&pixels, &pitch);

    for (unsigned y = 0; y < HEIGHT; ++y) {
        for (unsigned x = 0; x < WIDTH; ++x) {
            unsigned off = (y * pitch) + (x * 3);
            pixels[off + 0] = 0xFF; // R
            pixels[off + 1] = 0xFF; // G
            pixels[off + 2] = 0xFF; // B
        }
    }

    SDL_UnlockTexture(tex);
    
    // Rect
    
    SDL_Rect src = { .x = 0, .y = 0, .w = WIDTH, .h = HEIGHT };
    SDL_Rect dst = { .x = 0, .y = 0, .w = WIDTH * SCALE, .h = HEIGHT * SCALE, };

    SDL_Event ev;

    for (;;)
    {
        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
            {
                exit(0);
            }
        }

        for (int i = 0; i < 1000; ++i)
        {
            for (int v = 0; v < 16; ++v) {
                printf("V[%d] = %02X ", v, V[v]);
            }
            printf("\n");
            execute(fetch());
        }
        return;

        SDL_LockTexture(tex, NULL, (void**)&pixels, &pitch);

        for (unsigned y = 0; y < HEIGHT; ++y) {
            for (unsigned x = 0; x < WIDTH; ++x) {
                unsigned off = (y * pitch) + (x * 3);
                pixels[off + 0] = VRAM[y][x]; // R
                pixels[off + 1] = VRAM[y][x]; // G
                pixels[off + 2] = VRAM[y][x]; // B
            }
        }

        SDL_UnlockTexture(tex);
        
        //SDL_RenderClear(r);
        SDL_RenderCopy(r, tex, &src, &dst);
        SDL_RenderPresent(r);
    }

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}