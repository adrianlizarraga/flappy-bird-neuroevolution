#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "MLPNetwork.h"

using namespace std;


int main() {
    cout << "Flappy!" << endl;

    alai::MLPNetwork nn(1, {2, 2});
    cout << nn.compute(std::vector<double>{1})[0] << endl;

    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}