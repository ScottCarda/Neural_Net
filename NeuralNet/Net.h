/*
					***** Net.h *****

Contains the definitions for the Net class and the Layer typedef. Net's contain
a vector of nodes that represent the input layer of the network and a vector of
Layers for the hidden and output layers of the network. The input layer is
unique because its nodes' outputs are simply set, not calculated from the
outputs of other nodes. For this reason, the input layer contains Nodes, which
are lightweight versions of Neurons, instead of fully fledged Neurons.

CSC547 Artificial Intelligence - Spring 2016

Author: Hannah Aker, Scott Carda, Cassidy Vollmer
*/

#ifndef __NET__H__
#define __NET__H__

/******************************************************************************/
/*                                Include Files                               */
/******************************************************************************/

#include "Neuron.h"

/******************************************************************************/
/*                              Layer Definition                              */
/******************************************************************************/

typedef vector<Neuron> Layer;

/******************************************************************************/
/*                                Net Definition                              */
/******************************************************************************/

class Net
{
public:
	// Constructor and Destructor
	Net(const vector<unsigned> &topology, double eta, double alpha);
	~Net() {};

	void back_prop(const vector<double> &expected_outputs);
	void feed_forward(const vector<double> &inputs);
	double get_alpha();
	double get_avg_error();
	double get_eta();
	double get_error();
	void get_output(vector<double> &outputs);
	void print_weights(string filename);
	bool read_in_weights(string weightsFileName, const vector<unsigned int> &nodesPerLayer);
	void reset_avg_error();
	void reset();
	void set_alpha(double alpha);
	void set_eta(double eta);

private:
	vector<Node> _input_layer;	// The inputs into the network
	// The layers of the network, not including the input layer
	vector<Layer> _layers;

	double _alpha;	// Network momentum
	double _eta;	// Network learning rate

	double _error;	// RMS error from most recent back propagation
	double _avg_error;	// Average RMS since start of the net, or last reset

	// Number of times back_prop has been called since last reset
	int _back_prop_count;
};

#endif