#ifndef SPRITE_H
#define SPRITE_H

#include <map>
#include <memory>

#include "../../../common/direction.h"
#include "../../client_constants.h"

#include "sprite_label.h"
#include "sprite_layer.h"

class Sprite {
private:
    SDL2pp::Point position;
    SDL2pp::Point target_position;

    std::optional<Direction> direction;
    SDL2pp::Point size;
    SDL2pp::Point render_offset;

    std::map<Layer, SpriteLayer> layers;

    std::unique_ptr<SpriteLabel> label;

public:
    Sprite(SpriteLayer&& base, SDL2pp::Point position, SDL2pp::Point size);

    Sprite(SpriteLayer&& body, SDL2pp::Point position, Direction action, SDL2pp::Point size);

    void add_layer(Layer layer_num, SpriteLayer&& layer);
    void remove_layer(Layer layer_num);

    const SDL2pp::Point& get_target_position() const;
    void set_target_position(Direction new_direction, const SDL2pp::Point& new_target);

    void update_visual_position();
    void update_frame(int iteration);

    void render(const SDL2pp::Point& camera_offset);

    SDL2pp::Point get_position() const;
    SDL2pp::Point get_size() const;

    bool is_idle() const;

    bool intersects(const SDL2pp::Rect& area, const SDL2pp::Point& offset) const;

    void remove_all_layers();

    SDL2pp::Point get_ground_position() const;

    bool has_label() const;

    SpriteLabel& get_label() const;

    void set_label(std::unique_ptr<SpriteLabel> new_label);

    void render_overlay(const SDL2pp::Point& camera_offset) const;

private:
    int get_new_coordinate(const int& current_coordinate, const int& coordinate_diff);

    // Transforma las coorddenadas multiplicándolas por la cte TILE_SIZE
    SDL2pp::Point to_sprite_point(const SDL2pp::Point& point);
};


#endif  // SPRITE_H
