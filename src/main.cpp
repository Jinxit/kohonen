
#include <iostream>
#include <vector>
#include <cmath>
#include "visualizer.hpp"
#include "kohonen.hpp"

int main(int argc, const char* argv[])
{
    struct point
    {
        float x;
        float y;
    };
    struct line
    {
        point a;
        point b;
    };
    std::vector<line> map;
    map.push_back(line{point{0, 0}, point{100, 0}});
    map.push_back(line{point{0, 0}, point{0, 100}});
    map.push_back(line{point{100, 0}, point{100, 100}});
    map.push_back(line{point{0, 100}, point{100, 100}});
    //map.push_back(line{point{50, 0}, point{50, 50}});
    //map.push_back(line{point{50, 50}, point{50, 75}});
    map.push_back(line{point{50, 25}, point{75, 25}});
    map.push_back(line{point{50, 0}, point{50, 75}});
    map.push_back(line{point{50, 75}, point{10, 75}});

    auto radius = [](unsigned int iteration, const point& winner, const point& other) {
        auto dx = winner.x - other.x;
        auto dy = winner.y - other.y;
        auto diff = std::sqrt(dx * dx + dy * dy);
        if (diff == 0)
            return 1.0f;
        return 0.0f; //-1.0f / (diff + 1);
    };

    auto distance = [](unsigned int iteration, const point& node, const line& input) {
        if (input.a.x == input.b.x)
        {
            if ((input.a.y < node.y && node.y < input.b.y)
             || (input.a.y > node.y && node.y > input.b.y))
            {
                auto diff = (node.x - input.a.x);
                return std::abs(diff);
            }
        }
        if (input.a.y == input.b.y)
        {
            if ((input.a.x < node.x && node.x < input.b.x)
             || (input.a.x > node.x && node.x > input.b.x))
            {
                auto diff = (node.y - input.a.y);
                return std::abs(diff);
            }
        }
        return 10000.0f;
    };

    auto attract = [&](unsigned int iteration, float influence, point& node, const line& input) {
        for (auto& line : map)
        {
            TSP::Visualizer::draw_line(10 + line.a.x * 3, 10 + line.a.y * 3, 10 + line.b.x * 3, 10 + line.b.y * 3);
        }
        if (input.a.x == input.b.x)
        {
            if ((input.a.y < node.y && node.y < input.b.y)
             || (input.a.y > node.y && node.y > input.b.y))
            {
                auto diff = (node.x - input.a.x) * influence;
                node.x += diff;
                return std::abs(diff);
            }
        }
        if (input.a.y == input.b.y)
        {
            if ((input.a.x < node.x && node.x < input.b.x)
             || (input.a.x > node.x && node.x > input.b.x))
            {
                auto diff = (node.y - input.a.y) * influence;
                node.y += diff;
                return std::abs(diff);
            }
        }
        return 0.0f;
    };

    auto repel = [&](unsigned int iteration, point& a, point& b) {
        /*for (auto& line : map)
        {
            for (auto& line2 : map)
            {
                if (line.a.x == line2.a.x && line.b.x == line2.b.x
                 && line.a.y == line2.a.y && line.b.y == line2.b.y)
                    continue;
                TSP::Visualizer::draw_circle(10 + ((line.a.x + line2.b.x) / 2.0f) * 3, 10 + ((line.a.y + line2.b.y) / 2.0f) * 3, 4, 5, 0, 100, 200);
                TSP::Visualizer::draw_circle(10 + ((line.b.x + line2.a.x) / 2.0f) * 3, 10 + ((line.b.y + line2.a.y) / 2.0f) * 3, 4, 5, 0, 100, 200);
                TSP::Visualizer::draw_circle(10 + ((line.a.x + line2.a.x) / 2.0f) * 3, 10 + ((line.a.y + line2.a.y) / 2.0f) * 3, 4, 5, 0, 100, 200);
                TSP::Visualizer::draw_circle(10 + ((line.b.x + line2.b.x) / 2.0f) * 3, 10 + ((line.b.y + line2.b.y) / 2.0f) * 3, 4, 5, 0, 100, 200);
            }
        }*/
        auto dx = b.x - a.x;
        auto dy = b.y - a.y;
        auto diff = std::sqrt(dx * dx + dy * dy);

        if (diff < 10)
        {
            a.x += dx * 0.001f * (diff - 10.0f);
            b.x -= dx * 0.001f * (diff - 10.0f);
            a.y += dy * 0.001f * (diff - 10.0f);
            b.y -= dy * 0.001f * (diff - 10.0f);
        }
    };

    std::mt19937 gen;
    std::uniform_real_distribution<float> dist(-5, 5);


    unsigned int num_nodes = 10;
    std::vector<point> nodes;
    for (unsigned int x = 1; x <= num_nodes; x++)
    {
        for (unsigned int y = 1; y <= num_nodes; y++)
        {

            nodes.push_back(point{dist(gen) + x * (100.0f / (num_nodes + 2)), dist(gen) + y * (100.0f / (num_nodes + 2))});
        }
    }

    annalisa::kohonen<point, line> som(nodes, radius, distance, attract, repel, 0.05f, 1.0f);
    TSP::Visualizer::initialize(400, 400);
    som.update(map, 100000, 0.0f);
}