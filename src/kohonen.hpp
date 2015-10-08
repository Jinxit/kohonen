#pragma once

#include <vector>
#include <string>
#include <functional>

#include "utils.hpp"

namespace Annalisa
{	
	class Kohonen
	{
	public:
		typedef std::function<double(unsigned int, unsigned int,
			unsigned int, std::vector<double>, 
			unsigned int, std::vector<double>)> KohonenFunction;

		Kohonen(unsigned int numInputs, unsigned int numOutputs, unsigned int outputDimensions,
			KohonenFunction distanceFunction, KohonenFunction radiusFunction,
			double learningRate, double learningDecay
		);

		Kohonen(unsigned int numInputs, unsigned int numOutputs, unsigned int outputDimensions,
			KohonenFunction distanceFunction, KohonenFunction radiusFunction,
			double learningRate, double learningDecay,
			std::vector<std::vector<double>> weights
		);

		unsigned int inputCount() const { return inputs.size(); };
		virtual void update(const std::vector<std::vector<double>> & data, unsigned int iterations,
			double epsilon);
		virtual std::string toString() const;

		std::vector<std::vector<double>> weights;
	protected:
		std::vector<double> inputs;

		double learningRate, learningDecay;

		KohonenFunction distanceFunction;
		KohonenFunction radiusFunction;

		virtual unsigned int findBestMatchingNode(unsigned int iterations, 
			unsigned int dataIndex, std::vector<double> data);
	};
}
