#include "TrainingData.h"

TrainingData::TrainingData( const string filename )
{
	_data_file.open( filename.c_str() );
}

TrainingData::~TrainingData()
{
	_data_file.close();
}

bool TrainingData::is_eof()
{
	return _data_file.eof();
}

unsigned TrainingData::get_topology( vector<unsigned> &topology )
{
	topology.clear();

	string line;
	string label;

	getline( _data_file, line );
	stringstream ss( line );
	ss >> label;
	if ( is_eof() || label.compare( "topology:" ) != 0 )
		abort();

	unsigned n;
	while ( ss >> n )
		topology.push_back( n );

	return topology.size();
}

unsigned TrainingData::get_next_inputs( vector<double> &input_vals )
{
	input_vals.clear();

	string line;
	string label;

	getline( _data_file, line );
	stringstream ss( line );
	ss >> label;
	if ( label.compare( "in:" ) == 0 )
	{
		unsigned n;
		while ( ss >> n )
			input_vals.push_back( n );
	}

	return input_vals.size();
}

unsigned TrainingData::get_expected_outputs( vector<double> &expected_outputs )
{
	expected_outputs.clear();

	string line;
	string label;

	getline( _data_file, line );
	stringstream ss( line );
	ss >> label;
	if ( label.compare( "out:" ) == 0 )
	{
		unsigned n;
		while ( ss >> n )
			expected_outputs.push_back( n );
	}

	return expected_outputs.size();
}