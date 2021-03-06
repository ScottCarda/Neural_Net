/*
                    ***** Neuron.cpp *****

This file contains the definitions for the neuron class's member functions. See
the Neuron.h file for a description of the class.

CSC547 Artificial Intelligence - Spring 2016

Author: Hannah Aker, Scott Carda, Cassidy Vollmer
*/

/******************************************************************************/
/*                                Include Files                               */
/******************************************************************************/

#include "Neuron.h"

/******************************************************************************/
/*                       Neuron Class Member Functions                        */
/******************************************************************************/

// Adds a new connection to the node provided into the _input vector
void Neuron::add_connection( Node &node )
{
	_input.push_back( Connection { get_new_weight(), 0, &node } );
	return;
}

// Describes the job of an individual neuron during network back propagation
void Neuron::back_prop()
{
	calc_gradient();	// Calculate the neuron's gradient
	// Use the gradient to produce error
	// values for the layer behind this neuron
	send_weight_corrections();
	updateWeights();	// Calculate the new weights for this neuron
	return;
}

// Describes the job of an individual neuron during network feed forward
void Neuron::feed_forward()
{
	// Gets the dot product of the input values with the weight values
	double sum = 0;
	for ( unsigned int i = 0; i < _input.size(); i++ )
		sum += _input[i].node->output * _input[i].weight;

	// Map the summed inputs to the neuron's output
	output = transform( sum );

	// Erase data from previous back propagations
	weight_corrections.clear();

	return;
}

// Getter for _alpha
double Neuron::get_alpha()
{
	return _alpha;
}

// Getter for _eta
double Neuron::get_eta()
{
	return _eta;
}

// Prints the weights for the neuron to a file stream
void Neuron::print_weights( ofstream &fout )
{
	// Loops through the weights
	for ( unsigned int i = 0; i < _input.size(); i++ )
		fout << _input[i].weight << " ";

	return;
}

// Sets the state of the neuron a new neuron
void Neuron::reset()
{
	// Reset data members
	output = 0;
	_gradient = 0;

	// Clears out the error values
	weight_corrections.clear();

	// Resets weights
	for ( unsigned int i = 0; i < _input.size(); i++ )
	{
		_input[i].weight = get_new_weight();
		_input[i].delta_weight = 0;
	}

	return;
}

// Setter for _alpha
void Neuron::set_alpha( double alpha )
{
	_alpha = alpha;

	return;
}

// Setter for _eta
void Neuron::set_eta( double eta )
{
	_eta = eta;

	return;
}

// Sets the weights for each of the neuron's inputs
void Neuron::set_weight( const vector<double> &weights )
{
	// Loops through each input connection on the neuron
	for ( unsigned int i = 0; i < weights.size(); i++ )
		_input.at( i ).weight = weights.at( i );

	return;
}

// Uses weight_corrections vector and output to calculate the _gradient value
void Neuron::calc_gradient()
{
	// Sum the error values of the neuron
	double sum = 0;
	for ( unsigned int i = 0; i < weight_corrections.size(); i++ )
		sum += weight_corrections[i];

	// Calculate the gradient for the neuron
	_gradient = sum * transform_derivative( output );

	return;
}

// Provides weight values for new connections
double Neuron::get_new_weight()
{
	double scale_value = ( RAND_MAX ) / 4.0;
	return double( rand() ) / scale_value - 2;
}

// Populates the weight_corrections vector for the
// neurons whose output are feed into this neuron
void Neuron::send_weight_corrections()
{
	// Send weight * gradient back
	// Skip bias neurons
	for ( unsigned int i = 0; i < _input.size() - 1; i++ )
		_input[i].node->weight_corrections.push_back(
		_input[i].weight * _gradient );

	return;
}

// The derivative of the mathematical function uses as the activation function
double Neuron::transform_derivative( double val )
{
	// derivative of the transform function
	return val * ( 1 - val );
}

// The neuron's function for determining if the neuron activates or not
double Neuron::transform( double val )
{
	// sigmoid function
	return 1.0 / ( 1.0 + exp( -val ) );
}

// Implements the delta learning rule
void Neuron::updateWeights()
{
	double new_delta;	// The amount the weight will change

	// For each input to the neuron
	for ( unsigned int i = 0; i < _input.size(); i++ )
	{
		// Calculate the new delta weight
		new_delta =
			_eta * _input[i].node->output * _gradient
			+
			_alpha * _input[i].delta_weight;
		// Store this as the old delta weight
		_input[i].delta_weight = new_delta;
		// Update weight
		_input[i].weight += new_delta;
	}
	return;
}
