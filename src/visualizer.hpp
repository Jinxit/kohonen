#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP

#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace TSP
{
    class Visualizer
    {
    public:
        static void initialize(unsigned int width, unsigned int height);
        static void clear();
        static void draw_circle(unsigned int x, unsigned int y,
            unsigned int vertices, unsigned int size,
            unsigned int r, unsigned int g, unsigned int b);
        static void draw_line(unsigned int x1, unsigned int y1,
            unsigned int x2, unsigned int y2);
        static void render();
    private:
        static std::unique_ptr<sf::RenderWindow> window;
    };
}

#endif
