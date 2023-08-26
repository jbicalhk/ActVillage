#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <tmxlite/Map.hpp>

using namespace std;

#include "SFMLOrthogonalLayer.hpp"

int main(int argc, char **argv) {
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
	sf::View cameraView(sf::FloatRect(0,0,130,110));
	window.setView(cameraView);

	cout << "1" << endl;
	tmx::Map map;

	if (map.load("assets/demo.tmx")) {

		MapLayer chao(map, 0);
		MapLayer layerOne(map, 1);
		MapLayer layerTwo(map, 2);
		MapLayer layerThree(map, 3);

		sf::Clock globalClock;
		while (window.isOpen()) {
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
			}

			window.clear(sf::Color::Black);
			window.draw(chao);
			window.draw(layerOne);
			window.draw(layerTwo);
			window.draw(layerThree);
			window.display();
		}

	}
	return 0;
}
