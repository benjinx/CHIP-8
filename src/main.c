#include <string.h>

#include "cpu.h"
#include "log.h"
#include "registers.h"

#include <SDL.h>

// argc = number of arguments, argv = the arguments
int main(int argc, char* argv[])
{

    // Error check for too many arguments or too few
    //if (argc < 2)
    //{
    //    LogError("No filename specified.\n");
    //    return 1;
    //}
    //else if (argc > 2)
    //{
    //    LogError("Too many arguments.\n");
    //    return 1;
    //}
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window;
    int SCALE = 3;

    window = SDL_CreateWindow(
        "CHIP-8",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        160 * SCALE,
        144 * SCALE,
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

    for (unsigned y = 0; y < 144; ++y) {
        for (unsigned x = 0; x < 160; ++x) {
            unsigned off = (y * 256 * 3) + (x * 3);
            pixels[off + 0] = 0xFF; // R
            pixels[off + 1] = 0x00; // G
            pixels[off + 2] = 0x00; // B
        }
    }

    SDL_UnlockTexture(tex);



    // Rect
    
    SDL_Rect src = { .x = 0, .y = 0, .w = 160, .h = 144 };
    SDL_Rect dst = { .x = 0, .y = 0, .w = 160 * SCALE, .h = 144 * SCALE, };

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