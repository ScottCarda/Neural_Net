/*
                    ***** Neuron.h *****

Contains the definitions for the Connection structure, the Node class, and the
Neuron class, which inherits from the Node class. Non-neuron node objects are
lightweight objects that just provide an output without taking any inputs.
Non-neuron nodes are used in a neural net for the input nodes.

CSC547 Artificial Intelligence - Spring 2016

Author: Hannah Aker, Scott Carda, Cassidy Vollmer
*/

#ifndef __NEURON__H__
#define __NEURON__H__

/******************************************************************************/
/*                                Include Files                               */
/******************************************************************************/

#include <cmath>
#include <ctime>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

/******************************************************************************/
/*                               Node Definition                              */
/******************************************************************************/

class Node
{
public:
	double output;	// The value that the node outputs

	// The error values used to update the node's weights
	// Included in the node class to allow
	// for generalized behavior in the neuron class
	vector<double> weight_corrections;

	// Constructor and Destructor
	Node() : output( 0 ) {};
	~Node() {};
};

/******************************************************************************/
/*                            Connection Definition                           */
/******************************************************************************/

struct Connection
{
	double weight;
	// The amount the weight has changed in the last iteration
	double delta_weight;
	// Pointer to the node, whose output will be multiplied by the weight
	Node* node;
};

/******************************************************************************/
/*                              Neuron Definition                             */
/******************************************************************************/

class Neuron : public Node
{
public:
	// Constructor and Destructor
	Neuron( double eta, double alpha ) :
		Node(),
		_gradient( 0 ),
		_eta( eta ),
		_alpha( alpha )
	{};
	~Neuron() {};

	void add_connection( Node &node );
	void feed_forward();
	void back_prop();
	void reset();
	void print_weights( ofstream &fout );
	double get_eta();
	void set_eta( double eta );
	double get_alpha();
	void set_alpha( double alpha );
	void set_weight( const vector<double> &weights );

private:
	static double get_new_weight();
	static double transform( double val );
	static double transform_derivative( double val );
	void calc_gradient();
	void send_weight_corrections();
	void updateWeights();

	// The vector of inputs to the neuron, containing the
	// raw input, weights, and last changes in weights
	vector<Connection> _input;

	double _gradient;	// Error gradient for the neuron
	double _eta;	// The learning rate for this neuron
	double _alpha;	// The momentum rate for this neuron
};

#endif
