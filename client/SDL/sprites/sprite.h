#ifndef SPRITE_H
#define SPRITE_H
#include <map>

#include "../../../common/direction.h"
#include "../../client_constants.h"

#include "sprite_layer.h"

class Sprite {
private:
    SDL2pp::Point position;
    SDL2pp::Point target_position;

    Direction direction;
    SDL2pp::Point size;

    std::map<Layer, SpriteLayer> layers;

public:
    Sprite(SpriteLayer&& body, SDL2pp::Point position, Direction action, SDL2pp::Point size);

    void add_layer(Layer layer_num, SpriteLayer&& layer);
    void remove_layer(Layer layer_num);

    void set_target_position(Direction new_direction, const SDL2pp::Point& new_target);

    void update_visual_position();
    void update_frame(int iteration);

    void render(const SDL2pp::Point& camera_offset);

    SDL2pp::Point get_position() const;
    SDL2pp::Point get_size() const;

    bool is_idle() const;

    bool intersects(const SDL2pp::Rect& area, const SDL2pp::Point& offset) const;
};


#endif  // SPRITE_H
