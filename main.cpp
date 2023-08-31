#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <tmxlite/Map.hpp>
#include "player.hpp"
#include <tmxlite/Object.hpp>

using namespace std;

#include "SFMLOrthogonalLayer.hpp"

int main(int argc, char **argv) {
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
	//sf::View cameraView(sf::FloatRect(0,0,130,110));
	int number= 0;
	bool fase1Unlocked = false;
	bool fase2Unlocked = false;
	bool fase3Unlocked = false;
	bool fase4Unlocked = false;
	bool fase5Unlocked = false;
	bool fase6Unlocked = false;
	bool fase7Unlocked = false;
	bool fase8Unlocked = false;
//	std::cout << "digite um numero";
//	std::cin >> number;

	//hitbox superior
	sf::RectangleShape hitbox(sf::Vector2f(1120.f, 25.f));
	hitbox.setFillColor(sf::Color::Transparent);
	hitbox.setPosition(0, 55);
	//hitbox esquerda
	sf::RectangleShape hitbox2(sf::Vector2f(25.f, 905.f));
		hitbox2.setFillColor(sf::Color::Transparent);
		hitbox2.setPosition(145, 55);
	//hitbox direita
		sf::RectangleShape hitbox3(sf::Vector2f(25.f, 905.f));
		hitbox3.setFillColor(sf::Color::Transparent);
		hitbox3.setPosition(960, 55);
	//hitbox inferior
		sf::RectangleShape hitbox4(sf::Vector2f(1120.f, 25.f));
		hitbox4.setFillColor(sf::Color::Transparent);
		hitbox4.setPosition(0, 870);




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

	    Player player(playerTextures, 170.0f);
	    Camera camera(window, player);
	    float playerSpeed = 170.0f;
	    sf::Clock clock1, clock2;


	cout << "1" << endl;
	tmx::Map map;

	if (map.load("assets/demo.tmx")) {

		MapLayer chao(map, 0);
		MapLayer layerOne(map, 1);
		MapLayer layerTwo(map, 2);
		MapLayer Fase1_casinhas(map, 3);
		MapLayer Fase2_casinhas(map, 4);
		MapLayer Fase3_comercio(map, 5);
		MapLayer Fase4_terraCastelo(map, 6);
		MapLayer Fase5_castelo1(map, 7);
		MapLayer Fase6_castelo2(map, 8);
		MapLayer Fase7_decoracaoExtra(map, 9);
		MapLayer Fase8_final(map, 10);

		sf::Clock globalClock;
		while (window.isOpen()) {
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
			}



			//teste do personagem
			float deltaTime = clock1.restart().asSeconds();
			sf::Time Temp;
			Temp = clock2.getElapsedTime();

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

			player.update(deltaTime);
			camera.update();

			if(hitbox.getGlobalBounds().intersects(player.getSprite().getGlobalBounds())){
				player.getSprite().move(0, playerSpeed * deltaTime);
			}
			if(hitbox2.getGlobalBounds().intersects(player.getSprite().getGlobalBounds())){
				player.getSprite().move(playerSpeed * deltaTime,0);
			}
			if(hitbox3.getGlobalBounds().intersects(player.getSprite().getGlobalBounds())){
				player.getSprite().move(-playerSpeed * deltaTime,0);
			}
			if(hitbox4.getGlobalBounds().intersects(player.getSprite().getGlobalBounds())){
				player.getSprite().move(0, -playerSpeed * deltaTime);
			}


			//atualizar visao do jogador
			//cameraView.setCenter(player.getPosition());

			// Verifica se 30 segundos se passaram
			if (Temp.asSeconds() >= 10) {
				Temp = sf::Time::Zero;
				clock2.restart();
				number++; // Incrementa a variável
			}
			if (number >= 1) {
				fase1Unlocked = true;
			}
			if (number >= 2) {
				fase2Unlocked = true;
			}
			if (number >= 3) {
				fase3Unlocked = true;
			}
			if (number >= 4) {
				fase4Unlocked = true;
			}
			if (number >= 5) {
				fase5Unlocked = true;
			}
			if (number >= 6) {
				fase6Unlocked = true;
			}

			if (number >= 7) {
				fase7Unlocked = true;
			}
			if (number >= 8) {
				fase8Unlocked = true;
			}

			window.clear(sf::Color::Black);
			window.setView(camera.getView());
			window.draw(chao);
			window.draw(layerOne);
			window.draw(layerTwo);

			if(fase1Unlocked)
			window.draw(Fase1_casinhas);
			if(fase2Unlocked)
			window.draw(Fase2_casinhas);
			if(fase3Unlocked)
			window.draw(Fase3_comercio);
			if(fase4Unlocked){
			window.draw(Fase4_terraCastelo);
			window.draw(Fase5_castelo1);
			window.draw(Fase6_castelo2);
			}
			if(fase5Unlocked)
			window.draw(Fase7_decoracaoExtra);
			if(fase6Unlocked)
			window.draw(Fase8_final);

			window.draw(player.getSprite());
			window.draw(hitbox);
			window.draw(hitbox2);
			window.draw(hitbox3);
			window.draw(hitbox4);
			window.display();
		}

	}
	return 0;
}
