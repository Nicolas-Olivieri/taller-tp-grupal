#include "client.h"

#include <exception>
#include <iostream>
#include <utility>

#include "client/qt/lobby.h"

#include "client_game.h"

// TODO agregar al README
//  Copyright (C) 1997-2024 Sam Lantinga <slouken@libsdl.org>
#include <QApplication>
#include <QCoreApplication>

#include <SDL2/SDL.h>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Surface.hh>
#include <SDL2pp/Texture.hh>
#include <SDL2pp/Window.hh>

Client::Client(int argc, char* argv[]): app(argc, argv) {}

int Client::run() {
    lobby.show();

    int error = app.exec();

    if (error)
        return error;

    lobby.close();
    Socket socket = lobby.finish();

    ClientGame game(std::move(socket));
    game.run();

    return 0;
}
