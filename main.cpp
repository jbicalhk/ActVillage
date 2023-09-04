#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <tmxlite/Map.hpp>
#include "player.hpp"
#include <tmxlite/Object.hpp>
#include <random>

using namespace std;

#include "SFMLOrthogonalLayer.hpp"

void spawnNewEnemy(std::vector<Enemy> &enemies,
			const std::vector<sf::Texture> &enemyTextures, float speed,
			int initialHealth, const sf::Texture &projectileTexture,
			float projectileSpeed) {
		Enemy newEnemy(enemyTextures, speed, initialHealth, projectileTexture,
				projectileSpeed);
		float x = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * 800.0f;
		float y = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * 800.0f;

		    newEnemy.setEnemyPosition(x, y);

		enemies.push_back(newEnemy);
	}
int main(int argc, char **argv) {
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
	//sf::View cameraView(sf::FloatRect(0,0,130,110));
	int number = 0;
	int lifePlayer = 1000000;
	bool fase1Unlocked = false;
	bool fase2Unlocked = false;
	bool fase3Unlocked = false;
	bool fase4Unlocked = false;
	bool fase5Unlocked = false;
	bool fase6Unlocked = false;
	int maxSpawns = 3;
//	std::cout << "digite um numero";
//	std::cin >> number;
	//bool isAlive = true;
	sf::Clock spawnTimer;
	float spawnInterval = 5.0f;
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
	bool keyReleased = true;

	std::vector<std::vector<sf::Texture>> playerTextures(4); // 0: Front, 1: Back, 2: Left, 3: Right
	std::string directions[] = { "front", "back", "left", "right" };

	for (int dir = 0; dir < 4; ++dir) {
		for (int i = 0; i <= 5; ++i) {
			sf::Texture texture;
			if (!texture.loadFromFile(
					"assets/" + directions[dir] + "/sprite_player_walking_"
							+ directions[dir] + "_" + std::to_string(i)
							+ ".png")) {
				return 1;
			}
			playerTextures[dir].push_back(texture);
		}
	}
	sf::Texture playerProjectileTexture;
		playerProjectileTexture.loadFromFile("assets/sprite_boomerang1.png");

	Player player(playerTextures, 170.0f, lifePlayer, playerProjectileTexture);
	std::vector<Projectile> PlayerProjectiles;

	Camera camera(window, player);

	float playerSpeed = 170.0f;
	sf::Clock clock1, clock2;

	std::vector<sf::Texture> enemyTextures1;
	sf::Texture Enemytexture;
	for (int i = 0; i <= 5; ++i) {
		sf::Texture texture;
		if (!Enemytexture.loadFromFile(
				"assets/sprite_spider_walking_" + std::to_string(i) + ".png")) {
			return 1;
		}
		enemyTextures1.push_back(Enemytexture);
	}
	sf::Texture enemyProjectileTexture1;
	enemyProjectileTexture1.loadFromFile("assets/sprite_web_6.png");

	std::vector<Enemy> enemies;
	const std::vector<sf::Texture> enemyTextures = enemyTextures1;
	const float speed = 90.0f;
	const int initialHealth = 50;
	const sf::Texture enemyProjectileTexture = enemyProjectileTexture1;
	const float projectileSpeed = 190.0f;

	    enemies.emplace_back(enemyTextures, speed, initialHealth, enemyProjectileTexture, projectileSpeed);

	//Enemy enemy(enemyTextures, 90.0f, 50, enemyProjectileTexture, 190.0f);







	//enemy.getSprite().setPosition(200.0f, 200.0f);

	//Enemy* enemy2 = new Enemy(enemyTextures, 90.0f, 50);
	//enemy2->getSprite().setPosition(100.0f, 100.0f);

	//cout << "1" << endl;
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
			for (auto& enemy : enemies) {
			if (enemy.getHealth() <= 0)
				enemy.stopAttacking();
			}
			if (spawnTimer.getElapsedTime().asSeconds() >= spawnInterval && enemies.size() < maxSpawns && number  < 18) {
					 spawnNewEnemy(enemies, enemyTextures1, 90, 50, enemyProjectileTexture, 190);
					 spawnTimer.restart();

				 }
			//teste do personagem
			float deltaTime = clock1.restart().asSeconds();
			sf::Time Temp;
			Temp = clock2.getElapsedTime();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				player.startMoving(2);
				player.move(-playerSpeed * deltaTime, 0.0f);
			}

			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				player.startMoving(3);
				player.move(playerSpeed * deltaTime, 0.0f);
			}

			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				player.startMoving(1);
				player.move(0.0f, -playerSpeed * deltaTime);
			}

			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				player.startMoving(0);
				player.move(0.0f, playerSpeed * deltaTime);
			}

			else {
				player.stopMoving();
			}
			//std::cout << enemy.getHealth() << endl;
			//std::cout << player.getLife() << endl;
			if (event.key.code == sf::Keyboard::Space && keyReleased) {
			player.shoot(PlayerProjectiles,enemyProjectileTexture);
			keyReleased = false;
		}

			if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::Space) {
					keyReleased = true;
				}
			}
			std::vector<const Projectile*> collidedProjectiles;

			for (const Enemy &enemy : enemies) {
			    for (const Projectile &projectile : enemy.getProjectiles()) {
			        if (projectile.getSprite().getGlobalBounds().intersects(player.getSprite().getGlobalBounds())) {
			            if (std::find(collidedProjectiles.begin(), collidedProjectiles.end(), &projectile) == collidedProjectiles.end()) {
			                player.decreaseLife(10);
			                collidedProjectiles.push_back(&projectile);
			            }
			        }
			    }
			}

			std::vector<const Projectile*> collidedProjectilesPlayer;

			for (const Projectile &projectilePlayer : player.getProjectiles()) {
			    for (Enemy &enemy : enemies) {
			        if (projectilePlayer.getSprite().getGlobalBounds().intersects(enemy.getSprite().getGlobalBounds())) {
			            if (std::find(collidedProjectilesPlayer.begin(),
			                collidedProjectilesPlayer.end(),
			                &projectilePlayer)
			                == collidedProjectilesPlayer.end()) {
			                enemy.decreaseHealth(50);
			                collidedProjectilesPlayer.push_back(&projectilePlayer);

			            }
			        }
			    }
			}



			for (auto it = enemies.begin(); it != enemies.end();) {
						    if (it->getHealth() == 0) {
						    	number++;
						    	it = enemies.erase(it); // Remove o inimigo do vetor
						    } else {
						        ++it;
						    }
						}

			sf::Vector2f playerPosition = player.getSprite().getPosition();

			player.update(deltaTime);
			camera.update();
			player.updateProjectiles(deltaTime);
			std::cout<<number<< endl;

			for (auto& enemy : enemies) {
			        enemy.update(deltaTime, playerPosition);
			}
			if (hitbox.getGlobalBounds().intersects(
					player.getSprite().getGlobalBounds())) {
				player.getSprite().move(0, playerSpeed * deltaTime);
			}
			if (hitbox2.getGlobalBounds().intersects(
					player.getSprite().getGlobalBounds())) {
				player.getSprite().move(playerSpeed * deltaTime, 0);
			}
			if (hitbox3.getGlobalBounds().intersects(
					player.getSprite().getGlobalBounds())) {
				player.getSprite().move(-playerSpeed * deltaTime, 0);
			}
			if (hitbox4.getGlobalBounds().intersects(
					player.getSprite().getGlobalBounds())) {
				player.getSprite().move(0, -playerSpeed * deltaTime);
			}


			if (number >= 3) {
				fase1Unlocked = true;
			}
			if (number >= 6) {
				fase2Unlocked = true;
			}
			if (number >= 9) {
				fase3Unlocked = true;
			}
			if (number >= 12) {
				fase4Unlocked = true;
			}
			if (number >= 15) {
				fase5Unlocked = true;
			}
			if (number >= 18) {
				fase6Unlocked = true;
			}

			window.clear(sf::Color::Black);
			window.setView(camera.getView());
			window.draw(chao);
			window.draw(layerOne);
			window.draw(layerTwo);

			if (fase1Unlocked)
				window.draw(Fase1_casinhas);

			if (fase2Unlocked)
				window.draw(Fase2_casinhas);

			if (fase3Unlocked)
				window.draw(Fase3_comercio);

			if (fase4Unlocked) {
				window.draw(Fase4_terraCastelo);
				window.draw(Fase5_castelo1);
				window.draw(Fase6_castelo2);
			}
			if (fase5Unlocked)
				window.draw(Fase7_decoracaoExtra);

			if (fase6Unlocked)
				window.draw(Fase8_final);

			window.draw(player.getSprite());
			window.draw(hitbox);
			window.draw(hitbox2);
			window.draw(hitbox3);
			window.draw(hitbox4);
			for (auto& enemy : enemies) {
			if (enemy.getHealth() > 0) {
				window.draw(enemy.getSprite());
				}
			}
			for (auto& enemy : enemies) {
			for (const auto &projectile : enemy.getProjectiles()) {
				window.draw(projectile.getSprite());
			}
			}
			for (const auto& projectilee : player.getProjectiles())
			        {
			            window.draw(projectilee.getSprite());
			        }
			window.display();
		}

	}
	return 0;
}
