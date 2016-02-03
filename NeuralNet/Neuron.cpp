#include "Neuron.h"

double Neuron::get_new_weight()
{
	return 1;
}

double Neuron::transform( double val )
{
	return tanh( val );
}

double Neuron::transform_derivative( double val )
{
	return 1.0 - val * val;
}

void Neuron::add_connection( Node &node )
{
	_input.push_back( Connection { get_new_weight(), 0, &node } );
	return;
}

void Neuron::feed_forward()
{
	double sum = 0;
	for ( unsigned i = 0; i < _input.size(); i++ )
	{
		sum += _input[i].node->output * _input[i].weight;
	}

	//sum += _bias;

	output = transform( sum );

	weight_corrections.clear();

	return;
}

void Neuron::calc_gradient()
{
	double sum = 0;
	for ( unsigned i = 0; i < weight_corrections.size(); i++ )
	{
		sum += weight_corrections[i];
	}

	_gradient = sum * transform_derivative( output );

	return;
}

void Neuron::send_weight_corrections()
{
	for ( unsigned i = 0; i < _input.size(); i++ )
	{
		_input[i].node->weight_corrections.push_back(
			_input[i].weight * _gradient );
	}

	return;
}

void Neuron::updateWeights()
{
	double new_delta;
	//double new_delta =
	//	_eta
	//	* _gradient
	//	+
	//	_alpha
	//	* _delta_bias;
	//_delta_bias = new_delta;
	//_bias += new_delta;
	for ( unsigned i = 0; i < _input.size(); i++ )
	{
		new_delta =
			_eta
			* _input[i].node->output
			* _gradient
			+
			_alpha
			* _input[i].delta_weight;
		_input[i].delta_weight = new_delta;
		_input[i].weight += new_delta;
	}
	return;
}

void Neuron::back_prop()
{
	calc_gradient();
	send_weight_corrections();
	updateWeights();
	return;
}

double Neuron::get_eta()
{
	return _eta;
}

void Neuron::set_eta( double eta )
{
	_eta = eta;

	return;
}

double Neuron::get_alpha()
{
	return _alpha;
}

void Neuron::set_alpha( double alpha )
{
	_alpha = alpha;

	return;
}

void Neuron::reset()
{
	output = 0;
	//_bias = get_new_weight();
	//_delta_bias = 0;
	_gradient = 0;

	for ( unsigned i = 0; i < _input.size(); i++ )
	{
		_input[i].weight = get_new_weight();
		_input[i].delta_weight = 0;
	}

	return;
}

void Neuron::print_weights( ofstream &fout )
{
	// Prints out the weights for the neuron
	for ( unsigned int i = 0; i < _input.size(); i++ )
		fout << _input[i].weight << " ";

	// Prints out the bias weight
	//fout << _bias << " ";

	return;
}
