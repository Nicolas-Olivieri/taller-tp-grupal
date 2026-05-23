#ifndef CAMERA_H
#define CAMERA_H
#include "SDL2pp/SDL2pp.hh"
#include "sprites/sprite.h"

using namespace SDL2pp;

class Camera {
private:
    Rect view;
    Rect world_size;
    Sprite& user;

public:
    Camera(int screen_width, int screen_height, Rect world_size, Sprite& user);

    void update_position();

    Rect get_world() const;

    Rect get_view() const;
};


#endif  // CAMERA_H
