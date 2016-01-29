#include <cmath>
#include <vector>
#include <fstream>

using namespace std;

class Node
{
public:
	Node() : output( 0 ) {};
	//Node( double val ) : output( val ) {};
	~Node() {};

	double output;
	vector<double> weight_corrections;
};

struct Connection
{
	double weight;
	double delta_weight;
	Node* node;
};

class Neuron : public Node
{
public:
	Neuron( double eta, double alpha ) :
		Node(),
		_bias( get_new_weight() ),
		_delta_bias( 0 ),
		_gradient( 0 ),
		_eta( eta ),
		_alpha( alpha )
	{};
	~Neuron() {};

	void add_connection( Node &node );
	void feed_forward();
	void back_prop();

	double get_eta();
	void set_eta( double eta );
	double get_alpha();
	void set_alpha( double alpha );

	void reset();
	void print_weights( ofstream &fout );

private:
	static double get_new_weight();
	static double transform( double val );
	static double transform_derivative( double val );
	void clac_gradient();
	void send_weight_corrections();
	void updateWeights();

	double _bias;
	double _delta_bias;
	vector<Connection> _input;

	double _gradient;
	double _eta;
	double _alpha;
};
