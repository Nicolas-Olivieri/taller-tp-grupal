#ifndef PLAYER_SPRITE_H
#define PLAYER_SPRITE_H
#include <map>

#include "client/client_constants.h"

#include "moving_sprite.h"
#include "sprite_layer.h"


class PlayerSprite: public MovingSprite {
    friend class SpriteCreator;

private:
    std::map<Layer, SpriteLayer> layers;

public:
    PlayerSprite(SpriteLayer&& head, SpriteLayer&& body, SDL2pp::Point position, SDL2pp::Point size,
                 Direction direction);

    virtual void update_frame(int iteration) override;

    virtual void render(const SDL2pp::Point& camera_offset) override;

    virtual bool intersects(const SDL2pp::Rect& area, const SDL2pp::Point& offset) const override;

    void add_layer(Layer layer_num, SpriteLayer&& layer);

    void remove_layer(Layer layer_num);

    void remove_all_layers();

    bool layer_is_different(Layer layer, int id) const;

    Direction get_last_direction() const;

    ~PlayerSprite() override = default;
};


#endif  // PLAYER_SPRITE_H
