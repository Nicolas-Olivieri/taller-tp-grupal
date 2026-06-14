#ifndef CAMERA_H
#define CAMERA_H
#include "SDL2pp/SDL2pp.hh"
#include "sprites/player_sprite.h"

class Camera {
private:
    SDL2pp::Rect view;
    SDL2pp::Rect world_size;
    PlayerSprite& user;

public:
    Camera(int screen_width, int screen_height, SDL2pp::Rect world_size, PlayerSprite& user);

    void update_position();

    SDL2pp::Rect get_world() const;

    SDL2pp::Rect get_view() const;
};


#endif  // CAMERA_H
