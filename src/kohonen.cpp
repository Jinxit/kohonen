#include "kohonen.hpp"

#include "utils.hpp"
#include "visualizer.hpp"

#include <iostream>

namespace Annalisa
{
	Kohonen::Kohonen(unsigned int numInputs, unsigned int numOutputs, unsigned int outputDimensions,
			KohonenFunction distanceFunction, KohonenFunction radiusFunction,
			double learningRate, double learningDecay)
			:	inputs(numInputs),
				learningRate(learningRate),
				learningDecay(learningDecay),
				distanceFunction(distanceFunction),
				radiusFunction(radiusFunction)
	{
		weights = std::vector<std::vector<double>>(numOutputs, std::vector<double>(numInputs));
		for (auto & weight : weights)
		{
			for (auto & w : weight)
			{
				w = unifRand(0.0, 0.1);
			}
		}
	}

	Kohonen::Kohonen(unsigned int numInputs, unsigned int numOutputs, unsigned int outputDimensions,
			KohonenFunction distanceFunction, KohonenFunction radiusFunction,
			double learningRate, double learningDecay,
			std::vector<std::vector<double>> weights)
			:	weights(weights),
				inputs(numInputs),
				learningRate(learningRate),
				learningDecay(learningDecay),
				distanceFunction(distanceFunction),
				radiusFunction(radiusFunction)
	{
	}

	void Kohonen::update(const std::vector<std::vector<double>> & dataset, unsigned int iterations,
		double epsilon)
	{
        TSP::Visualizer::initialize(800, 600);

		// as iterations goes to 0
		auto startIterations = iterations;
		while (iterations --> 0)
		{
			bool check = iterations % 16 == 0;
			double diffSum = 0.0;

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

	unsigned int Kohonen::findBestMatchingNode(unsigned int iteration, 
		unsigned int dataIndex, std::vector<double> data)
	{
		unsigned int minIndex = 0;
		auto minDist = distanceFunction(iteration, weights.size(), dataIndex, data, 0, weights[0]);
		for (unsigned int i = 1; i < weights.size(); i++)
		{
			auto dist = distanceFunction(iteration, weights.size(), dataIndex, data, i, weights[i]);
			if (dist < minDist)
			{
				minIndex = i;
				minDist = dist;
			}
		}

		return minIndex;
	}

	// TODO
	std::string Kohonen::toString() const
	{
		return "";
	}
}
