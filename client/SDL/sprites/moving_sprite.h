#ifndef MOVING_SPRITE_H
#define MOVING_SPRITE_H
#include <memory>

#include "common/direction.h"

#include "sprite.h"
#include "sprite_label.h"


class MovingSprite: public Sprite {
protected:
    SDL2pp::Point target_position;
    Direction direction;
    std::unique_ptr<SpriteLabel> label;

    int get_new_coordinate(const int& current_coordinate, const int& coordinate_diff);

    bool is_idle() const;

public:
    MovingSprite(SDL2pp::Point position, SDL2pp::Point size, Direction direction);

    void update_visual_position();

    void set_target_position(Direction new_direction, const SDL2pp::Point& new_target);

    SDL2pp::Point get_target_position() const;

    Direction get_direction() const;

    bool has_label() const;

    SpriteLabel& get_label() const;

    void set_label(std::unique_ptr<SpriteLabel> new_label);

    void render_overlay(const SDL2pp::Point& camera_offset) const;

    ~MovingSprite() override = default;

    MovingSprite(MovingSprite&&) = default;
    MovingSprite& operator=(MovingSprite&&) = default;

    MovingSprite(const MovingSprite&) = delete;
    MovingSprite& operator=(const MovingSprite&) = delete;
};


#endif  // MOVING_SPRITE_H
