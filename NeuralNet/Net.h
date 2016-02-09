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
	Net( const vector<unsigned> &topology, double eta, double alpha );
	~Net() {};

	void feed_forward( const vector<double> &inputs );
	void back_prop( const vector<double> &expected_outputs );
	void get_output( vector<double> &outputs );
	double get_avg_error();
	double get_eta();
	void set_eta( double eta );
	double get_alpha();
	void set_alpha( double alpha );
	void reset();

	void print_weights( string filename );

private:
	vector<Node> _input_layer;	// The inputs into the network
	// The layers of the network, not including the input layer
	vector<Layer> _layers;

	double _eta;	// Network learning rate
	double _alpha;	// Network momentum

	double _error;	// Difference between network output and expected output
	double _avg_error;	// Average error
	// Smoothing factor used in calculating avg_error
	double _error_smoothing_factor;
};

#endif