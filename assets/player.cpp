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
        sprite.setScale(1.5f, 1.5f);
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
    Camera(const sf::RenderWindow& window, Player& player) : view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)), target(player.getSprite())
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

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Animação de Jogador SFML");

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/fundo.png"))
    {
        return 1;
    }
    sf::Sprite backgroundImage(backgroundTexture);
    backgroundImage.setScale(static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x, static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y);

    std::vector<std::vector<sf::Texture>> playerTextures(4); // 0: Front, 1: Back, 2: Left, 3: Right
    std::string directions[] = { "front", "back", "left", "right" };

    for (int dir = 0; dir < 4; ++dir)
    {
        for (int i = 0; i <= 5; ++i)
        {
            sf::Texture texture;
            if (!texture.loadFromFile("assets/" + directions[dir] + "/sprite_player_walking_" + directions[dir] + "_" + std::to_string(i) + ".png")) {
                return 1;
            }
            playerTextures[dir].push_back(texture);
        }
    }

    Player player(playerTextures, 200.0f);
    Camera camera(window, player);

    float zoomFactor = 1.0f;
    float zoomSpeed = 0.1f;
    float playerSpeed = 200.0f;

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        float deltaTime = clock.restart().asSeconds();

        // Movimento e animação do personagem
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            player.startMoving(2);
            player.move(-playerSpeed * deltaTime, 0.0f);
        }

        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            player.startMoving(3);
            player.move(playerSpeed * deltaTime, 0.0f);
        }

        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            player.startMoving(1);
            player.move(0.0f, -playerSpeed * deltaTime);
        }

        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            player.startMoving(0);
            player.move(0.0f, playerSpeed * deltaTime);
        }

        else
        {
            player.stopMoving();
        }

        //Zoom na visão do personagem
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
        {
            zoomFactor -= zoomSpeed * deltaTime;
            camera.zoom(1.0f - zoomSpeed * deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
        {
            zoomFactor += zoomSpeed * deltaTime;
            camera.zoom(1.0f + zoomSpeed * deltaTime);
        }

        //Atualiza view
        player.update(deltaTime);
        camera.update();

        window.clear();
        window.setView(camera.getView());
        window.draw(backgroundImage);
        window.draw(player.getSprite());
        window.display();
    }
    return 0;
}
