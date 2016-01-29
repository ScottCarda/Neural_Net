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
		_layers.push_back( layer );
	}
}

void Net::feed_forward( const vector<double> &inputs )
{
	for ( unsigned i = 0; i < _input_layer.size(); i++ )
	{
		_input_layer[i].weight_corrections.clear();
		_input_layer[i].output = inputs[i];
	}

	for ( unsigned i = 0; i < _layers.size(); i++ )
		for ( unsigned j = 0; j < _layers[i].size(); j++ )
			_layers[i][j].feed_forward();

	return;
}

void Net::back_prop( const vector<double> &expected_outputs )
{
	_error = 0.0;

	Layer &output_layer = _layers.back();
	for ( unsigned i = 0; i < output_layer.size(); i++ )
	{
		double delta = expected_outputs[i] - output_layer[i].output;
		output_layer[i].weight_corrections.push_back( delta );
		_error += delta * delta;
	}

	_error /= output_layer.size();
	_error = sqrt( _error );

	_recent_average_error =
		( _recent_average_error * _recent_average_smoothing_factor + _error )
		/ ( _recent_average_smoothing_factor + 1.0 );

	for ( int i = _layers.size() - 1; i >= 0; i-- )
		for ( unsigned j = 0; j < _layers[i].size(); j++ )
			_layers[i][j].back_prop();

	return;
}

void Net::get_output( vector<double> &outputs )
{
	Layer &output_layer = _layers.back();
	outputs.clear();
	for ( unsigned i = 0; i < output_layer.size(); i++ )
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
	for ( unsigned i = 0; i < _input_layer.size(); i++ )
	{
		_input_layer[i].weight_corrections.clear();
		_input_layer[i].output = 0;
	}

	for ( unsigned i = 0; i < _layers.size(); i++ )
		for ( unsigned j = 0; j < _layers[i].size(); j++ )
			_layers[i][j].reset();

	return;
}

void Net::print_weights( string filename )
{
	ofstream fout;
	fout.open( filename );

	// catch file not opened
	if ( !fout )
		return;

	// prints number of weight layers
	fout << _layers.size() - 1 << " ";

	// prints number of nodes in each layer
	for ( unsigned int i = 0; i < _layers.size(); i++ )
	{
		fout << _layers[i].size() << " ";
	}
	fout << endl;

	// print weights for each neuron
	for ( unsigned int i = 0; i < _layers.size(); i++ )
		for ( unsigned int j = 0; j < _layers[i].size(); j++ )
			_layers[i][j].print_weights( fout );

	fout.close();

	return;
}
