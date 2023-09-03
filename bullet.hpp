#ifndef BULLET_HPP_
#define BULLET_HPP_
#pragma once

#include <SFML/Graphics.hpp>

class Bullet
{
public:
    Bullet(float posx, float posy, float speed, const sf::Texture& texture);

    void move();

    sf::FloatRect getBounds() const;

    void markForRemoval();

    bool shouldBeRemoved() const;

    void draw(sf::RenderWindow& window) const;

    bool hasCollided() const;

    void setCollided(bool value);

private:
    bool collided;
    float posx, posy;
    float speed;
    bool shouldRemove;
    sf::Sprite sprite;
};
#endif /* BULLET_HPP_ */
