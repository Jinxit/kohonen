#pragma once

#include <vector>
#include <functional>
#include <random>

namespace annalisa
{
    template<class Node, class Input>
    class kohonen
    {
    public:
        // returns how much a node influences a neighbouring node
        using radius_function = std::function<float(unsigned int, // current iteration
                                                    const Node&, // winner node
                                                    const Node&)>; // other node


        // returns the distance between an input and a node
        using distance_function = std::function<float(unsigned int, // current iteration
                                                      const Node&,
                                                      const Input&)>;

        // moves a node towards an input, returns a scalar of how much it moved
        // the output scalar is compared with min_diff in the update
        using attract_function = std::function<float(unsigned int, // current iteration
                                                     float, // influence from radius
                                                            // and learning_rate
                                                     Node&, // node to be moved
                                                     const Input&)>; // input to move towards

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
            std::uniform_int_distribution<size_t> random(0, inputs.size() - 1);
            for (unsigned int i = 0; i < num_iterations; i++)
            {
                auto diff_sum = 0.0f;

                auto& chosen_input = inputs[random(gen)];

                auto winner = nodes[0];
                {
                    auto winner_distance = distance(i, winner, chosen_input);

                    for (size_t j = 1; j < nodes.size(); i++)
                    {
                        auto dist = distance(i, nodes[j], chosen_input);
                        if (dist < winner_distance)
                        {
                            winner = nodes[j];
                            winner_distance = dist;
                        }
                    }
                }


                for (size_t k = 0; k < nodes.size(); k++)
                {
                    auto influence = radius(i, winner, nodes[k]);
                    if (influence == 0)
                        continue;

                    diff_sum += attract(i, learning_rate * influence, winner, chosen_input);
                }

                if (diff_sum < min_diff)
                {
                    return;
                }

                learning_rate *= learning_decay;
            }

        }

        const std::vector<Node>& get_nodes() const
        {
            return nodes;
        }

        size_t size() const
        {
            return nodes.size();
        }

    private:
        std::vector<Node> nodes;
        radius_function radius;
        distance_function distance;
        attract_function attract;
        float learning_rate;
        float learning_decay;
        std::mt19937 gen;
    };
}
