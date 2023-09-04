#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

class Projectile
{
private:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float lifetime;
    bool hasCollided;
public:
    Projectile(const sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& direction, float speed, float lifetime)
        : sprite(texture), velocity(direction * speed), lifetime(lifetime)
    {
        sprite.setPosition(position);
    }

    void update(float deltaTime)
    {
    	if (!hasCollided) {
    	        sprite.move(velocity * deltaTime);

    	        lifetime -= deltaTime;
    	    }
    }

    bool isAlive() const
    {
        return lifetime > 0;
    }

    sf::Sprite& getSprite()
    {
        return sprite;
    }

    const sf::Sprite& getSprite() const
        {
            return sprite;
        }
    bool hasCollision() const {
    		return hasCollided;
    	}

    	void setCollision(bool collided) {
    		hasCollided = collided;
    	}
protected:
	void shoot(std::vector<Projectile> &projectiles,
			const sf::Texture &projectileTexture,
			const sf::Vector2f &direction) {
		projectiles.emplace_back(projectileTexture, sprite.getPosition(),
				direction, 500.0f, 2.0f);
	}
};

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
    int life;
    std::vector<Projectile> projectiles;
    sf::Texture playerProjectileTexture;
    bool spaceKeyReleased = true;

public:
    Player(const std::vector<std::vector<sf::Texture>>& textures, float speed, int initialLife,const sf::Texture &projectileTexture)
        : Character(textures[0][0], speed), playerTextures(textures), life(initialLife), playerProjectileTexture(projectileTexture)
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
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				if (spaceKeyReleased) {

				sf::Vector2f shootDirection;
				switch (currentDirection) {
				case 0: // Front
					shootDirection = sf::Vector2f(0, 1);
					break;
				case 1: // Back
					shootDirection = sf::Vector2f(0, -1);
					break;
				case 2: // Left
					shootDirection = sf::Vector2f(-1, 0);
					break;
				case 3: // Right
					shootDirection = sf::Vector2f(1, 0);
					break;
				default:
					break;
				}

			shoot(projectiles, playerProjectileTexture);
			spaceKeyReleased = false;
			}
		}else{
			spaceKeyReleased = true;
		}
	}
}
void updateProjectiles(float deltaTime)
{
            for (auto it = projectiles.begin(); it != projectiles.end();)
            {
                it->update(deltaTime);
                if (!it->isAlive())
                {
                    it = projectiles.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }

	int getLife() const {
		return life;
	}
	void decreaseLife(int amount) {
		life -= amount;
	}

	void shoot(std::vector<Projectile> &projectiles,
			const sf::Texture &projectileTexture) {
		if (isMoving) {

			sf::Vector2f shootDirection;
			switch (currentDirection) {
			case 0: // Front
				shootDirection = sf::Vector2f(0, 1);
				break;
			case 1: // Back
				shootDirection = sf::Vector2f(0, -1);
				break;
			case 2: // Left
				shootDirection = sf::Vector2f(-1, 0);
				break;
			case 3: // Right
				shootDirection = sf::Vector2f(1, 0);
				break;
			default:
				break;
			}

			projectiles.emplace_back(projectileTexture, sprite.getPosition(),
					shootDirection, 500.0f, 2.0f);
		}
	}
	const std::vector<Projectile>& getProjectiles() const {
		return projectiles;
	}

};

class Enemy : public Character
{
private:
    std::vector<sf::Texture> enemyTextures;
    int currentFrame = 0;
    int health;
	std::vector<Projectile> projectiles;
	sf::Clock shootTimer;
	float shootInterval = 4.0f;
	sf::Texture enemyProjectileTexture;
	float projectileSpeed;
	bool isAttacking = true;
	sf::Clock spawnTimer;
	float spawnInterval = 5.0f;
public:
public:
    Enemy(const std::vector<sf::Texture> &textures, float speed, int initialHealth,
          const sf::Texture &projectileTexture, float projectileSpeed)
        : Character(textures[0], speed), enemyTextures(textures), health(initialHealth),
          enemyProjectileTexture(projectileTexture), projectileSpeed(projectileSpeed)
    {
    	spawnTimer.restart();
    	spawnInterval = 5.0f + (rand() % 5);
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

		if (isAttacking
				&& shootTimer.getElapsedTime().asSeconds() >= shootInterval) {
			sf::Vector2f direction = playerPosition - sprite.getPosition();
			float length = std::sqrt(
					direction.x * direction.x + direction.y * direction.y);

			if (length != 0) {
				direction /= length;
			}

			projectiles.emplace_back(enemyProjectileTexture,
					sprite.getPosition(), direction, projectileSpeed, 3.0f);

			shootTimer.restart();
		}

		for (auto it = projectiles.begin(); it != projectiles.end();) {
			it->update(deltaTime);

			if (!it->isAlive()) {
				it = projectiles.erase(it);
			} else {
				++it;
			}
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
	const std::vector<Projectile>& getProjectiles() const
	    {
	        return projectiles;
	    }
	void stopAttacking()
	    {
	        isAttacking = false;
     }

	void setEnemyPosition(float x, float y) {
	        sprite.setPosition(x, y);
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
