#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

class Character
{
protected:
    sf::Sprite sprite;
    int currentDirection = 0; // 0: Front, 1: Back, 2: Left, 3: Right
    bool isMoving;
    float speed;
    sf::Clock animationClock;
    float animationDuration = 0.2f;

public:
    Character(const sf::Texture& texture, float speed)
        : speed(speed), isMoving(false)
    {
        sprite.setTexture(texture);
        sprite.setScale(0.8f, 0.8f);
    }
    virtual ~Character() {
        }

    virtual void update(float deltaTime)
    {

    }

    virtual void move(float dx, float dy)
    {
        sprite.move(dx, dy);
    }

    virtual void startMoving(int direction)
    {
        currentDirection = direction;
        isMoving = true;
    }

    virtual void stopMoving()
    {
        isMoving = false;
    }

    sf::Sprite& getSprite()
    {
        return sprite;
    }
};

class Player : public Character
{
private:
    std::vector<std::vector<sf::Texture>> playerTextures;
    int currentFrame = 0;

public:
    Player(const std::vector<std::vector<sf::Texture>>& textures, float speed)
        : Character(textures[0][0], speed), playerTextures(textures)
    {
        sprite.setPosition(400.0f - sprite.getGlobalBounds().width / 2.0f, 300.0f - sprite.getGlobalBounds().height / 2.0f);
    }

    void update(float deltaTime) override
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
};

class Enemy : public Character
{
private:
    std::vector<sf::Texture> enemyTextures;
    int currentFrame = 0;
    int health;
public:
	Enemy(const std::vector<sf::Texture> &textures, float speed, int initialHealth) :
			Character(textures[0], speed), enemyTextures(textures), health(initialHealth) {
	}

	void update(float deltaTime, const sf::Vector2f &playerPosition) {
		sf::Vector2f direction = playerPosition - sprite.getPosition();
		float length = std::sqrt(
				direction.x * direction.x + direction.y * direction.y);

		if (length != 0) {
			direction /= length;
		}

		move(direction.x * speed * deltaTime, direction.y * speed * deltaTime);

		if (direction.x > 0) {
			currentDirection = 3;
		} else if (direction.x < 0) {
			currentDirection = 2;
		}
		isMoving = true;

		if (isMoving) {
			if (animationClock.getElapsedTime().asSeconds()
					>= animationDuration) {
				currentFrame = (currentFrame + 1) % enemyTextures.size();
				sprite.setTexture(enemyTextures[currentFrame]);
				animationClock.restart();
			}
		} else {
			sprite.setTexture(enemyTextures[0]);
		}

		Character::update(deltaTime);
	}
	void decreaseHealth(int amount) {
		health -= amount;
		if (health <= 0) {
		}
	}
	int getHealth() const {
		return health;
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
