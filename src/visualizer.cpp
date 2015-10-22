#include "visualizer.hpp"

namespace TSP
{
    std::unique_ptr<sf::RenderWindow> Visualizer::window;

    void Visualizer::initialize(unsigned int width, unsigned int height)
    {
        //sf::RenderWindow(sf::VideoMode(width, height), "TSP Visualizer")
        Visualizer::window = std::unique_ptr<sf::RenderWindow>(
            new sf::RenderWindow(sf::VideoMode(width, height), "TSP Visualizer"));
    }

    void Visualizer::clear()
    {
        window->clear();
    }

    void Visualizer::draw_circle(unsigned int x, unsigned int y,
        unsigned int vertices, unsigned int size,
        unsigned int r, unsigned int g, unsigned int b)
    {
        sf::CircleShape shape(size, vertices);
        shape.setFillColor(sf::Color(r, g, b));
        shape.setPosition(x - size, y - size);
        window->draw(shape);
    }

    void Visualizer::draw_line(unsigned int x1, unsigned int y1,
        unsigned int x2, unsigned int y2)
    {
        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(x1, y1)),
            sf::Vertex(sf::Vector2f(x2, y2))
        };

        window->draw(line, 2, sf::Lines);
    }

    void Visualizer::render()
    {
        window->display();
    }
}
