#include "bullet.hpp"

Bullet::Bullet(float posx, float posy, float speed, const sf::Texture& texture)
    : posx(posx), posy(posy), speed(speed), shouldRemove(false), collided(false)
{
    sprite.setTexture(texture);
    sprite.setPosition(posx, posy);
    sprite.setScale(0.05f, 0.05f);
}

void Bullet::move()
{
    posx += speed;
    sprite.setPosition(posx, posy);
}

sf::FloatRect Bullet::getBounds() const
{
    return sprite.getGlobalBounds();
}

void Bullet::markForRemoval()
{
    shouldRemove = true;
}

bool Bullet::shouldBeRemoved() const
{
    return shouldRemove;
}

void Bullet::draw(sf::RenderWindow& window) const
{
    window.draw(sprite);
}

bool Bullet::hasCollided() const
{
    return collided;
}

void Bullet::setCollided(bool value)
{
    collided = value;
}
