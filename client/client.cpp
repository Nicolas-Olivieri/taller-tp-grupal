#include "client.h"

#include <exception>
#include <iostream>

// TODO agregar al README
//  Copyright (C) 1997-2024 Sam Lantinga <slouken@libsdl.org>
#include <SDL2/SDL.h>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Surface.hh>
#include <SDL2pp/Texture.hh>
#include <SDL2pp/Window.hh>

Client::Client(const char* hostname, const char* servname):
        protocol(Socket(hostname, servname)) {}

void Client::run() {
    const DataDTO msg = {Command::HANDSHAKE, "Hola mundo! Atte. Cliente"};
    protocol.send_msg(msg);

    const DataDTO response = protocol.recv_msg();
    std::cout << response.msg << std::endl;

    // Si se quiere probar SDL
    // poc_sdl();
    // o
    // poc_sdl_2()
}


int Client::poc_sdl() {
    try {
        // Initialize SDL library
        SDL2pp::SDL sdl(SDL_INIT_VIDEO);

        // Create main window: 640x480 dimensions, resizable, "SDL2pp demo"
        // title
        SDL2pp::Window window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, 640, 480,
                              SDL_WINDOW_RESIZABLE);

        // Create accelerated video renderer with default driver
        SDL2pp::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

        // Clear screen
        renderer.Clear();

        // Show rendered frame
        renderer.Present();

        // 5 second delay
        SDL_Delay(5000);

        // Here all resources are automatically released and library
        // deinitialized
        return 0;
    } catch (std::exception& e) {
        // If case of error, print it and exit with error
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

int Client::poc_sdl_2() {
    try {
        // Initialize SDL library
        SDL2pp::SDL sdl(SDL_INIT_VIDEO);

        // Create main window: 640x480 dimensions, resizable, "SDL2pp demo"
        // title
        SDL2pp::Window window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, 640, 480,
                              SDL_WINDOW_RESIZABLE);

        // Create accelerated video renderer with default driver
        SDL2pp::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

        //    SDL2pp::Texture sprite(renderer,  SDL_PIXELFORMAT_ARGB8888,
        //    SDL_TEXTUREACCESS_STATIC, 4, 4);

        //    sprite.Update(SDL2pp::NullOpt, pixels, 4 * 4);
        //    sprite.SetBlendMode(SDL_BLENDMODE_BLEND);

        //    renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);

        int offset = 0;
        int max = 5;
        while (true) {
            // Process input
            SDL_Event event;
            while (SDL_PollEvent(&event))
                if (event.type == SDL_QUIT ||
                    (event.type == SDL_KEYDOWN &&
                     (event.key.keysym.sym == SDLK_ESCAPE ||
                      event.key.keysym.sym == SDLK_q)))
                    return 0;

                    // Clear screen
#define RGB(r, g, b) r, g, b
#define VERDE RGB(0, 32, 32)
            //        renderer.SetDrawColor(0, 32, 32);
            //        renderer.SetDrawColor(RGB(0, 32, 32));
            renderer.SetDrawColor(VERDE);
            renderer.Clear();

            // TODO ingresar path del png spritesheet
            SDL2pp::Surface surface("");
            SDL2pp::Texture texture(renderer, surface);

            // 27!!! es el numero importante q engloba toda una armadura

            renderer.Copy(texture, SDL2pp::Rect(27 * offset, 5, 27, 46),
                          SDL2pp::Rect(80, 20, 240, 240));

            offset = (offset + 1) % max;

            /*
            // Simple copy
            renderer.Copy(texture, SDL2pp::Rect(8, 8, 24, 38), SDL2pp::Rect(80,
            0, 240, 240));

            // Copy with modulation
            renderer.Copy(sprite, SDL2pp::NullOpt, SDL2pp::Rect(400, 0, 120,
            120)); sprite.SetAlphaMod(92); renderer.Copy(sprite,
            SDL2pp::NullOpt, SDL2pp::Rect(400 + 120, 0, 120, 120));
            sprite.SetColorMod(255, 0, 0);
            renderer.Copy(sprite, SDL2pp::NullOpt, SDL2pp::Rect(400, 0 + 120,
            120, 120)); sprite.SetAlphaMod(); renderer.Copy(sprite,
            SDL2pp::NullOpt, SDL2pp::Rect(400 + 120, 0 + 120, 120, 120));
            sprite.SetColorMod();

            // Copy with rotation
            renderer.Copy(sprite, SDL2pp::NullOpt, SDL2pp::Rect(80, 240, 240,
            240), -1.0 * SDL_GetTicks() / 5000.0 * 360.0, SDL2pp::NullOpt,
            SDL_FLIP_NONE);

            // Rotation around another point
            renderer.Copy(sprite, SDL2pp::NullOpt, SDL2pp::Rect(520, 360, 120,
            120), -1.0 * SDL_GetTicks() / 5000.0 * 360.0, SDL2pp::Point(0, 0),
            SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);

             */

            renderer.Present();

            // Frame limiter
            SDL_Delay(125);
        }

    } catch (std::exception& e) {
        // If case of error, print it and exit with error
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
