#pragma once

#include <vector>
#include <string>
#include <functional>

namespace annalisa
{
    template<class Node, class Input>
    class kohonen
    {
    public:
        // returns how much to move the neighbours of the node
        using radius_function = std::function<float(unsigned int,
                                                    const Node&, size_t,
                                                    const Input&, size_t);


        // returns the distance between an input and a node
        using distance_function = std::function<float(unsigned int,
                                                      const Node&, size_t,
                                                      const Input&, size_t);

        // moves a node towards an input, returns a scalar of how much it moved
        // the output scalar is compared with min_diff in the update
        using attract_function = std::function<float(unsigned int,
                                                     Node&, size_t,
                                                     const Input&, size_t);

        kohonen(std::vector<Node> nodes,
                radius_function radius,
                distance_function distance,
                attract_function attract,
                float learning_rate,
                float learning_decay)
            : nodes(nodes),
              radius(radius),
              distance(distance),
              attract(attract),
              learning_rate(learning_rate),
              learning_decay(learning_decay)
        { };

        void update(const std::vector<Input>& inputs,
                    unsigned int num_iterations, float min_diff)
        {
            // as iterations goes to 0
            for (int i = 0; i < num_iterations; i++)
            {
                double diff_sum = 0.0;

                TSP::Visualizer::clear();
                for (auto & data : dataset)
                {
                    TSP::Visualizer::draw_circle(data[0] * 0.14, data[1] * 0.14, 3, 8, 50, 80, 255);
                }
                for (unsigned int w = 0; w < weights.size(); w++)
                {
                    TSP::Visualizer::draw_circle(weights[w][0] * 0.14, weights[w][1] * 0.14, 4, 5, 255, 120, 0);
                    TSP::Visualizer::draw_line(weights[w][0] * 0.14, weights[w][1] * 0.14,
                        weights[(w + 1) % weights.size()][0] * 0.14,
                        weights[(w + 1) % weights.size()][1] * 0.14);
                }
                unsigned int dataIndex = unifRandInt(0, dataset.size() - 1);
                auto & data = dataset[dataIndex];
                auto winner = findBestMatchingNode(startIterations - iterations, dataIndex, data);
                TSP::Visualizer::draw_circle(weights[winner][0] * 0.14, weights[winner][1] * 0.14, 4, 6, 50, 255, 20);
                TSP::Visualizer::draw_circle(data[0] * 0.14, data[1] * 0.14, 3, 10, 255, 255, 0);
                TSP::Visualizer::render();

                for (unsigned int w = 0; w < weights.size(); w++)
                {
                    auto influence = radiusFunction(startIterations - iterations,
                        weights.size(), winner, weights[winner], w, weights[w]);
                    if (influence == 0)
                        continue;

                    for (unsigned int i = 0; i < weights[w].size(); i++)
                    {
                        auto diff = learningRate * influence * (data[i] - weights[w][i]);
                        weights[w][i] += diff;
                        if (check)
                        {
                            diffSum += abs(diff);
                        }
                    }
                }

                learningRate *= learningDecay;
            }

        }

        size_t size() const
        {
            return nodes.size();
        }

    private:
        std::vector<Data> nodes;
        radius_function radius;
        distance_function distance;
        attract_function attract;
        float learning_rate;
        float learning_decay;
    };
}
