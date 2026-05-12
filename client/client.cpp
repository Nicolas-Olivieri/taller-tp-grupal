#include "client.h"

// TODO agregar al README
//  Copyright (C) 1997-2024 Sam Lantinga <slouken@libsdl.org>
#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

Client::Client(Socket&& skt) : protocol(std::move(skt)) {}

void Client::run() {
    const DataDTO msg = {Command::HANDSHAKE, "Hola mundo! Atte. Cliente"};
    protocol.send_msg(msg);

    const DataDTO response = protocol.recv_msg();
    std::cout << response.msg << std::endl;

    // Si se quiere probar SDL
    // poc_sdl();
}


int Client::poc_sdl() {
    try {
        // Initialize SDL library
        SDL2pp::SDL sdl(SDL_INIT_VIDEO);

        // Create main window: 640x480 dimensions, resizable, "SDL2pp demo" title
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

        // Here all resources are automatically released and library deinitialized
        return 0;
    } catch (std::exception& e) {
        // If case of error, print it and exit with error
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

