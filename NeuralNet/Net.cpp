/*
                    ***** Net.cpp *****

This file contains the definitions for the net class's member functions. See
the Net.h file for a description of the class.

CSC547 Artificial Intelligence - Spring 2016

Author: Hannah Aker, Scott Carda, Cassidy Vollmer
*/

/******************************************************************************/
/*                                Include Files                               */
/******************************************************************************/

#include "Net.h"

/******************************************************************************/
/*                         Net Class Member Functions                         */
/******************************************************************************/

// Constructor for neural net class
Net::Net( const vector<unsigned> &topology, double eta, double alpha )
	:
	_alpha( alpha ),
	_eta( eta ),
	_error( 0.0 ),
	_avg_error( 0.0 ),
	_back_prop_count( 0 )
{
	// Creates the input layer with the first number from topology
	for ( unsigned int i = 0; i < topology[0]; i++ )
	{
		_input_layer.push_back( Node() );
	}

	// Add bias neuron to the end of the input layer
	Neuron bias( _eta, _alpha );
	bias.output = 1;
	_input_layer.push_back( bias );

	// Creates the hidden and output layers with the rest of topology
	for ( unsigned int i = 1; i < topology.size(); i++ )
	{
		Layer layer;	// The layer to be added
		// Adds a number of neurons to the layer based on topology
		for ( unsigned int j = 0; j < topology[i]; j++ )
		{
			// Create a new neuron with the net's eta and alpha values
			Neuron neuron( _eta, _alpha );

			// Adds a connection to the neuron for
			// each neuron in the preceding layer
			if ( i == 1 )
			{
				// Special case for the first layer after the input layer
				for ( unsigned int k = 0; k < _input_layer.size(); k++ )
				{
					neuron.add_connection( _input_layer[k] );
				}
			}
			else
			{
				for ( unsigned int k = 0; k < _layers.back().size(); k++ )
				{
					neuron.add_connection( _layers.back()[k] );
				}
			}
			// Add neuron to layer
			layer.push_back( neuron );

		}
		// Add bias neuron to the end of each layer
		Neuron bias( _eta, _alpha );
		bias.output = 1;
		layer.push_back( bias );

		// Add layer to net
		_layers.push_back( layer );
	}

	// Seed rand for random weights
	srand( time( NULL ) );
}

// Trains the net's weighted connections by comparing
// the output values with the expected output
void Net::back_prop( const vector<double> &expected_outputs )
{
	_error = 0.0;
	_back_prop_count++;

	Layer &output_layer = _layers.back();	// Reference to the last layer

	// Loop through output_layer ( not bias ), setting the errors for each
	for ( unsigned int i = 0; i < output_layer.size() - 1; i++ )
	{
		double delta = expected_outputs[i] - output_layer[i].output;
		// Save the error for gradient calculations
		output_layer[i].weight_corrections.push_back( delta );
		// Sum the squared errors
		_error += delta * delta;
	}

	// Calculate RMSE
	_error /= output_layer.size() - 1;
	_error = sqrt( _error );

	// Updates average error with the new value
	_avg_error += ( _error - _avg_error ) / _back_prop_count;

	// Loop through each neuron and call their back_prop
	// Skip input layer, and bias neurons
	for ( int i = _layers.size() - 1; i >= 0; i-- )
		for ( unsigned int j = 0; j < _layers[i].size() - 1; j++ )
			_layers[i][j].back_prop();

	return;
}

// Sets the output values of the output layer given an
// input vector based on the net's weighted connections
void Net::feed_forward( const vector<double> &inputs )
{
	// Set the inputs for each input node ( not bias )
	// And clear the weight_corrections vectors
	for ( unsigned int i = 0; i < _input_layer.size() - 1; i++ )
	{
		_input_layer[i].output = inputs[i];
		_input_layer[i].weight_corrections.clear();
	}

	// Loop through each neuron and call their feed_forward
	// Skip bias neurons
	for ( unsigned int i = 0; i < _layers.size(); i++ )
		for ( unsigned int j = 0; j < _layers[i].size() - 1; j++ )
			_layers[i][j].feed_forward();

	return;
}

// Getter for _alpha
double Net::get_alpha()
{
	return _alpha;
}

// Getter for _avg_error
double Net::get_avg_error()
{
	return _avg_error;
}

// Getter for _eta
double Net::get_eta()
{
	return _eta;
}

// Getter for _error
double Net::get_error()
{
	return _error;
}

// Fills the vector argument with the output values of the net
void Net::get_output( vector<double> &outputs )
{
	Layer &output_layer = _layers.back();	// Reference to the last layer

	// Fill the output vector
	outputs.clear();
	// Loop through the output layer ( not bias )
	for ( unsigned int i = 0; i < output_layer.size() - 1; i++ )
		outputs.push_back( output_layer[i].output );
	return;
}

// Creates a file with the given filename containing the 
void Net::print_weights( string filename )
{
	// The file stream
	ofstream fout;
	fout.open( filename );

	// Catch file not opened
	if ( !fout )
		return;

	// Prints number of weight layers
	fout << _layers.size() << " ";

	// Prints number of nodes in input layer
	fout << _input_layer.size() << " ";

	// Prints number of nodes in each layer
	for ( unsigned int i = 0; i < _layers.size(); i++ )
	{
		fout << _layers[i].size() << " ";
	}
	fout << endl;

	// Print weights for each neuron ( not bias )
	for ( unsigned int i = 0; i < _layers.size(); i++ )
		for ( unsigned int j = 0; j < _layers[i].size() - 1; j++ )
			_layers[i][j].print_weights( fout );

	fout << endl;

	fout.close();

	return;
}

// Reads in the weights and put the value in the node
// Returns false if the weights file could not be opened
bool Net::read_in_weights( string weightsFileName, const vector<unsigned int> &nodesPerLayer )
{
	ifstream _weightsfile;
	double currweight = 0.0;
	vector<double> weightsPerNeuron;

	_weightsfile.open( weightsFileName );

	if ( _weightsfile.is_open() )
	{
		// Skip the header information
		string line;
		getline( _weightsfile, line );

		// Loops through each neuron in each layer ( not bias )
		for ( unsigned int i = 0; i < _layers.size(); i++ )
		{
			for ( unsigned int j = 0; j < _layers.at( i ).size() - 1; j++ )
			{
				// Gets the weight values from the file for the neuron
				weightsPerNeuron.clear();
				// Add one to number of nodes for the bias node' weight
				for ( unsigned int k = 0; k < nodesPerLayer.at( i ) + 1; k++ )
				{
					_weightsfile >> currweight;
					weightsPerNeuron.push_back( currweight );
				}
				// Gives the new weights to the neuron
				_layers.at( i ).at( j ).set_weight( weightsPerNeuron );
			}
		}
		_weightsfile.close();

		return true;
	}

	return false;
}

// Resets the running average error for the net
void Net::reset_avg_error()
{
	_avg_error = 0.0;
	_back_prop_count = 0.0;

	return;
}

// Sets the state of the net a new net
void Net::reset()
{
	// Set the output for each input node ( not bias )
	// And clear the weight_corrections vectors
	for ( unsigned int i = 0; i < _input_layer.size() - 1; i++ )
	{
		_input_layer[i].output = 0;
		_input_layer[i].weight_corrections.clear();
	}

	// Loop through each neuron and call their reset
	// Skip bias neurons
	for ( unsigned int i = 0; i < _layers.size(); i++ )
		for ( unsigned int j = 0; j < _layers[i].size() - 1; j++ )
			_layers[i][j].reset();

	// Clear data members for the net
	_error = 0.0;
	_avg_error = 0.0;
	_back_prop_count = 0;

	return;
}

// Setter for _alpha
void Net::set_alpha( double alpha )
{
	_alpha = alpha;

	// Set the _alpha for each neuron in the net
	for ( unsigned int i = 0; i < _layers.size(); i++ )
		for ( unsigned int j = 0; j < _layers[i].size(); j++ )
			_layers[i][j].set_alpha( alpha );

	return;
}

// Setter for _eta
void Net::set_eta( double eta )
{
	_eta = eta;

	// Set the _eta for each neuron in the net
	for ( unsigned int i = 0; i < _layers.size(); i++ )
		for ( unsigned int j = 0; j < _layers[i].size(); j++ )
			_layers[i][j].set_eta( eta );

	return;
}
