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

    std::map<Layer, SpriteLayer> layers;

public:
    Sprite(SpriteLayer&& body, SDL2pp::Point position, Direction action);

    void add_layer(Layer layer_num, SpriteLayer&& layer);
    void remove_layer(Layer layer_num);

    void set_target_position(Direction new_direction, const SDL2pp::Point& new_target);

    void update_visual_position();
    void update_frame(int iteration);

    void render();

    SDL2pp::Point get_position() const;

    bool is_idle() const;

private:
    int get_new_coordinate(const int& current_coordinate, const int& coordinate_diff);
};


#endif  // SPRITE_H
