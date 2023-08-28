#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

class Player
{
private:
    std::vector<std::vector<sf::Texture>> playerTextures;
    sf::Sprite sprite;
    int currentFrame = 0;
    int currentDirection = 0; // 0: Front, 1: Back, 2: Left, 3: Right
    bool isMoving;
    float speed;
    sf::Clock animationClock;
    float animationDuration = 0.2f;

public:
    Player(const std::vector<std::vector<sf::Texture>>& textures, float speed): playerTextures(textures), speed(speed), isMoving(false)
    {
        sprite.setTexture(playerTextures[0][0]);
        sprite.setScale(0.8f, 0.8f);
        sprite.setPosition(400.0f - sprite.getGlobalBounds().width / 2.0f, 300.0f - sprite.getGlobalBounds().height / 2.0f);
    }

    void update(float deltaTime)
    {
        if (isMoving)
        {
            if (animationClock.getElapsedTime().asSeconds() >= animationDuration)
            {
                currentFrame = (currentFrame + 1) % playerTextures[0].size();
                animationClock.restart();
            }
            sprite.setTexture(playerTextures[currentDirection][currentFrame]);
        }
    }

    void move(float dx, float dy)
    {
        sprite.move(dx, dy);
    }

    void startMoving(int direction)
    {
        currentDirection = direction;
        isMoving = true;
    }

    void stopMoving()
    {
        isMoving = false;
    }

    sf::Sprite& getSprite()
    {
        return sprite;
    }
};

class Camera
{
private:
    sf::View view;
    sf::Sprite& target;

public:
    Camera(const sf::RenderWindow& window, Player& player) : view(sf::FloatRect(0, 0, window.getSize().x/4, window.getSize().y/4)), target(player.getSprite())
    {
        view.setCenter(target.getPosition());
    }

    void update()
    {
        view.setCenter(target.getPosition());
    }

    void zoom(float factor)
    {
        view.zoom(factor);
    }

    const sf::View& getView() const
    {
        return view;
    }
};
