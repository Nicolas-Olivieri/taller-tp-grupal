#ifndef PLAYER_SPRITE_H
#define PLAYER_SPRITE_H
#include <map>
#include <vector>

#include "../base/sprite_layer.h"
#include "client/client_constants.h"

#include "moving_sprite.h"


class PlayerSprite final: public MovingSprite {
    friend class SpriteCreator;

private:
    std::map<Layer, SpriteLayer> layers;
    std::map<Direction, std::vector<Layer>> render_order;
    bool is_client_player;

public:
    PlayerSprite(SpriteLayer&& head, SpriteLayer&& body, SDL2pp::Point position, SDL2pp::Point size,
                 Direction direction, bool is_client_player_);

    virtual void update_frame(int iteration) override;

    virtual void render(const SDL2pp::Point& camera_offset) override;

    virtual bool intersects(const SDL2pp::Rect& area, const SDL2pp::Point& offset) const override;

    void add_layer(Layer layer_num, SpriteLayer&& layer);

    void remove_layer(Layer layer_num);

    void remove_all_layers();

    bool layer_is_different(Layer layer, int id) const;

    Direction get_last_direction() const;

    ~PlayerSprite() override = default;

    PlayerSprite(PlayerSprite&&) = default;
    PlayerSprite& operator=(PlayerSprite&&) = default;

    PlayerSprite(const PlayerSprite&) = delete;
    PlayerSprite& operator=(const PlayerSprite&) = delete;
};


#endif  // PLAYER_SPRITE_H
