#include "Net.h"

Net::Net( const vector<unsigned> &topology, double eta, double alpha )
	:
	_eta(eta),
	_alpha( alpha ),
	_error( 0.0 ),
	_recent_average_error( 0.0 ),
	_recent_average_smoothing_factor( 0.25 )
{
	for ( unsigned i = 0; i < topology[0]; i++ )
	{
		_input_layer.push_back( Node() );
	}

	// Add bias neuron to the end of the input layer
	Neuron bias( _eta, _alpha );
	bias.output = 1;
	_input_layer.push_back( bias );

	for ( unsigned i = 1; i < topology.size(); i++ )
	{
		Layer layer;
		for ( unsigned j = 0; j < topology[i]; j++ )
		{
			Neuron neuron( _eta, _alpha );
			if ( i == 1 )
			{
				for ( unsigned k = 0; k < _input_layer.size(); k++ )
				{
					neuron.add_connection( _input_layer[k] );
				}
			}
			else
			{
				for ( unsigned k = 0; k < _layers.back().size(); k++ )
				{
					neuron.add_connection( _layers.back()[k] );
				}
			}
			layer.push_back( neuron );

		}
		// Add bias neuron to the end of each layer
		Neuron bias( _eta, _alpha );
		bias.output = 1;
		layer.push_back( bias );

		_layers.push_back( layer );
	}
}

void Net::feed_forward( const vector<double> &inputs )
{
	// Set the inputs for each input node ( not bias )
	// And clear the weight_corrections vectors
	for ( unsigned i = 0; i < _input_layer.size() - 1; i++ )
	{
		_input_layer[i].output = inputs[i];
		_input_layer[i].weight_corrections.clear();
	}

	// Also, clear the weight_corrections vector for the bias neuron
	_input_layer.back().weight_corrections.clear();

	// Loop through each neuron and call their feed_forward
	for ( unsigned i = 0; i < _layers.size(); i++ )
	{
		// Skip bias neurons, as they are handled differently
		for ( unsigned j = 0; j < _layers[i].size() - 1; j++ )
			_layers[i][j].feed_forward();

		// Clear the weight_corrections vector for the bias neuron
		_layers[i].back().weight_corrections.clear();
	}

	return;
}

void Net::back_prop( const vector<double> &expected_outputs )
{
	_error = 0.0;

	Layer &output_layer = _layers.back();	// Reference to the last layer

	// Loop through output_layer ( not bias ), setting the errors for each
	for ( unsigned i = 0; i < output_layer.size() - 1; i++ )
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

	// Keeps track of recent average error
	_recent_average_error =
		( _recent_average_error * _recent_average_smoothing_factor + _error )
		/ ( _recent_average_smoothing_factor + 1.0 );

	// Loop through each neuron and call their back_prop
	// Skip output layer, input layer, and bias neurons
	for ( int i = _layers.size() - 1; i >= 0; i-- )
		for ( unsigned j = 0; j < _layers[i].size() - 1; j++ )
			_layers[i][j].back_prop();

	return;
}

void Net::get_output( vector<double> &outputs )
{
	Layer &output_layer = _layers.back();	// Reference to the last layer

	// Fill the output vector
	outputs.clear();
	// Loop through the output layer ( not bias )
	for ( unsigned i = 0; i < output_layer.size() - 1; i++ )
		outputs.push_back( output_layer[i].output );
	return;
}

double Net::get_recent_average_error()
{
	return _recent_average_error;
}

double Net::get_eta()
{
	return _eta;
}

void Net::set_eta( double eta )
{
	_eta = eta;

	for ( unsigned i = 0; i < _layers.size(); i++ )
		for ( unsigned j = 0; j < _layers[i].size(); j++ )
			_layers[i][j].set_eta( eta );

	return;
}

double Net::get_alpha()
{
	return _alpha;
}

void Net::set_alpha( double alpha )
{
	_alpha = alpha;

	for ( unsigned i = 0; i < _layers.size(); i++ )
		for ( unsigned j = 0; j < _layers[i].size(); j++ )
			_layers[i][j].set_alpha( alpha );

	return;
}

void Net::reset()
{
	// Set the output for each input node ( not bias )
	// And clear the weight_corrections vectors
	for ( unsigned i = 0; i < _input_layer.size() - 1; i++ )
	{
		_input_layer[i].output = 0;
		_input_layer[i].weight_corrections.clear();
	}

	// Also, clear the weight_corrections vector for the bias neuron
	_input_layer.back().weight_corrections.clear();


	// Loop through each neuron and call their reset
	// Skip bias neurons
	for ( unsigned i = 0; i < _layers.size(); i++ )
		for ( unsigned j = 0; j < _layers[i].size() - 1; j++ )
			_layers[i][j].reset();

	return;
}

void Net::print_weights( string filename )
{
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
