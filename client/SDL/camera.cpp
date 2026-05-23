#include "camera.h"


Camera::Camera(const int screen_width, const int screen_height, const Rect world_size, Sprite& user):
        view(Rect(Point(0, 0), Point(screen_width, screen_height))), world_size(world_size), user(user) {}

void Camera::update_position() {
    const Point user_position = user.get_position();

    view.SetX(user_position.GetX() - view.GetW() / 2);
    view.SetY(user_position.GetY() - view.GetH() / 2);

    if (view.GetX() < 0)
        view.SetX(0);
    if (view.GetY() < 0)
        view.SetY(0);
    if (view.GetX() > world_size.GetW() - view.GetW())
        view.SetX(world_size.GetW() - view.GetW());
    if (view.GetY() > world_size.GetH() - view.GetH())
        view.SetY(world_size.GetY() - view.GetH());
}

Rect Camera::get_world() const { return world_size; }

Rect Camera::get_view() const { return view; }
