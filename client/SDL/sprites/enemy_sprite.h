#ifndef ENEMY_SPRITE_H
#define ENEMY_SPRITE_H
#include "moving_sprite.h"
#include "sprite_layer.h"


class EnemySprite : public MovingSprite {
private:
    SpriteLayer body;

public:
    EnemySprite(SpriteLayer&& body, SDL2pp::Point position, Direction direction, SDL2pp::Point size);

    virtual void update_frame(int iteration) override;

    virtual void render(const SDL2pp::Point& camera_offset) override;

    virtual bool intersects(const SDL2pp::Rect& area, const SDL2pp::Point& offset) const override;

};


#endif //ENEMY_SPRITE_H
