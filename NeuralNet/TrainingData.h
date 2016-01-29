#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

class TrainingData
{
public:
	TrainingData( const string filename );
	~TrainingData();
	bool is_eof();

	unsigned get_topology( vector<unsigned> &topology );
	unsigned get_next_inputs( vector<double> &input_vals );
	unsigned get_expected_outputs( vector<double> &expected_outputs );

private:
	ifstream _data_file;
};