#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "TrainingData.h"
#include "Net.h"
#include "Parameters.h"
using namespace std;

const double ETA = 0.3;
const double ALPHA = 0.5;

void show_vector_vals( ofstream &fout, string label, vector<double> &v );

int main(int argc, char* argv[])
{
	if (argc == 2)
		Parameters params = Parameters(argv[1]);
	else
		cout << "You're doing it wrong!" << endl;

	return 0;

}

int not_not_main()
{
	
	TrainingData training_data( "xor.txt" );

	vector<unsigned> topology;
	training_data.get_topology( topology );
	Net myNet( topology, ETA, ALPHA );

	vector<double> input_vals, expected_vals, output_vals;
	int training_pass = 0;

	ofstream fout;
	fout.open( "out.txt", ofstream::out );
	if ( !fout )
		return -1;

	while ( !training_data.is_eof() )
	{
		training_pass++;
		fout << endl << "Pass: " << training_pass << endl;

		if ( training_data.get_next_inputs( input_vals ) != topology[0] )
			break;
		show_vector_vals( fout, "Inputs:", input_vals );
		myNet.feed_forward( input_vals );
		myNet.get_output( output_vals );
		show_vector_vals( fout, "Outputs:", output_vals );
		training_data.get_expected_outputs( expected_vals );
		show_vector_vals( fout, "Targets:", expected_vals );
		myNet.back_prop( expected_vals );

		fout << "Net recent average error: "
			<< myNet.get_recent_average_error() << endl;
	}
	fout << endl << "Done" << endl;

	// Writes weights to file
	myNet.print_weights( "my_weights.wts" );

	fout.close();

	return 0;
}

void show_vector_vals( ofstream &fout, string label, vector<double> &v )
{
	fout << label << " ";
	for ( unsigned i = 0; i < v.size(); i++ )
		fout << v[i] << " ";
	fout << endl;
	return;
}

void not_main()
{
	bool x, y, z;
	fstream myFile;

	myFile.open( "xor.txt", fstream::out );
	myFile << "topology: 2 4 1" << endl;

	srand( time( NULL ) );

	for ( unsigned i = 0; i < 2000; i++ )
	{
		x = rand() % 2;
		y = rand() % 2;
		z = !x != !y;

		myFile << "in: " << double( x ) << " " << double( y ) << endl;
		myFile << "out: " << double( z ) << endl;
	}

	myFile.close();

	return;
}