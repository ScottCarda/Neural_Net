#include "Neuron.h"
#include <fstream>

typedef vector<Neuron> Layer;

class Net
{
public:
	Net( const vector<unsigned> &topology, double eta, double alpha );
	~Net() {};

	void feed_forward( const vector<double> &inputs );
	void back_prop( const vector<double> &expected_outputs );
	void get_output( vector<double> &outputs );
	double get_recent_average_error();
	double get_eta();
	void set_eta( double eta );
	double get_alpha();
	void set_alpha( double alpha );
	void reset();

	void print_weights( string filename );

private:
	vector<Node> _input_layer;
	vector<Layer> _layers;
	double _eta;
	double _alpha;

	double _error;
	double _recent_average_error;
	double _recent_average_smoothing_factor;
};
