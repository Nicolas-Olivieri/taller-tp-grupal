#include "camera.h"


Camera::Camera(const int screen_width, const int screen_height, const SDL2pp::Rect world_size, PlayerSprite& user):
        view(SDL2pp::Rect(SDL2pp::Point(0, 0), SDL2pp::Point(screen_width, screen_height))),
        world_size(world_size),
        user(user) {}

void Camera::update_position() {
    const SDL2pp::Point user_position = user.get_position();

    view.SetX((user_position.GetX() + user.get_size().GetX() / 2) - view.GetW() / 2);
    view.SetY((user_position.GetY() + user.get_size().GetY() / 2) - view.GetH() / 2);

    if (view.GetX() < 0)
        view.SetX(0);
    if (view.GetY() < 0)
        view.SetY(0);
    if (view.GetX() > world_size.GetW() - view.GetW())
        view.SetX(world_size.GetW() - view.GetW());
    if (view.GetY() > world_size.GetH() - view.GetH())
        view.SetY(world_size.GetH() - view.GetH());
}

SDL2pp::Rect Camera::get_world() const { return world_size; }

SDL2pp::Rect Camera::get_view() const { return view; }
