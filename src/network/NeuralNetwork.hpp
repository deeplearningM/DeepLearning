#ifndef QLEARNING_NEURALNETWORK_H
#define QLEARNING_NEURALNETWORK_H
#include <vector>
#include "Node.hpp"
#include <tuple>

class NeuralNetwork {
	friend class Learning;
public:
	NeuralNetwork();
	int getInputSize() const;
	int getOutputSize() const;
	int getLayerSize(int& layer) const;
	std::vector<double>& getOutputValues() const;
	std::vector<double>& getOutputValuesFromInputs(std::vector<double> values);

	void setInput(const int index, const double value);
	void setInputs(std::vector<double>& values);
	void addNode(const Node& node, const int type);
	void addNodes(std::vector<Node>& nodes, const int type);

	void connectAll();
	void calcAll();

protected:
	std::vector<std::vector<Node>> nodes;
	std::vector<int> sizes; //size of each Node-vector

private:
	
};

#endif