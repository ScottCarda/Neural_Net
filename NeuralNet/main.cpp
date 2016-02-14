#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Data.h"
#include "Net.h"
#include "Parameters.h"
using namespace std;

const double ETA = 0.3;
const double ALPHA = 0.5;

//void show_vector_vals(ofstream &fout, string label, vector<double> &v);
void training( Parameters &params, vector<YearData> &trainingSet );

int main( int argc, char* argv[] )
{
	if ( argc != 2 )
	{
		cout << "Incorrect Usage" << endl;
		return -1;
	}

	Parameters params = Parameters( argv[1] );
	Data data = Data( params.GetTrainTestFileName(), params.GetNumYearsBurnedAcreage(),
		params.GetNumMonthsPDSI(), params.GetEndMonthCurrYear(), params.GetFireSeverityCutoffs() );

	cout << "Parameter file: " << params.GetParamFileName() << endl;
	cout << "reading data from file: " << params.GetTrainTestFileName() << endl;

	training( params, data.GetAllData() );

	return 0;
}

//int not_not_main()
//{
//	
//	TrainingData training_data( "xor.txt" );
//
//	vector<unsigned> topology;
//	training_data.get_topology( topology );
//	Net myNet( topology, ETA, ALPHA );
//
//	vector<double> input_vals, expected_vals, output_vals;
//	int training_pass = 0;
//
//	ofstream fout;
//	fout.open( "out.txt", ofstream::out );
//	if ( !fout )
//		return -1;
//
//	while ( !training_data.is_eof() )
//	{
//		training_pass++;
//		fout << endl << "Pass: " << training_pass << endl;
//
//		if ( training_data.get_next_inputs( input_vals ) != topology[0] )
//			break;
//		show_vector_vals( fout, "Inputs:", input_vals );
//		myNet.feed_forward( input_vals );
//		myNet.get_output( output_vals );
//		show_vector_vals( fout, "Outputs:", output_vals );
//		training_data.get_expected_outputs( expected_vals );
//		show_vector_vals( fout, "Targets:", expected_vals );
//		myNet.back_prop( expected_vals );
//
//		fout << "Net recent average error: "
//			<< myNet.get_recent_average_error() << endl;
//	}
//	fout << endl << "Done" << endl;
//
//	// Writes weights to file
//	myNet.print_weights( "my_weights.wts" );
//
//	fout.close();
//
//	return 0;
//}

//void show_vector_vals(ofstream &fout, string label, vector<double> &v)
//{
//	fout << label << " ";
//	for (unsigned i = 0; i < v.size(); i++)
//		fout << v[i] << " ";
//	fout << endl;
//	return;
//}

void not_main()
{
	bool x, y, z;
	fstream myFile;

	myFile.open("xor.txt", fstream::out);
	myFile << "topology: 2 4 1" << endl;

	srand(time(NULL));

	for (unsigned i = 0; i < 2000; i++)
	{
		x = rand() % 2;
		y = rand() % 2;
		z = !x != !y;

		myFile << "in: " << double(x) << " " << double(y) << endl;
		myFile << "out: " << double(z) << endl;
	}

	myFile.close();

	return;
}

// Trains a neural net with the training set provided. The weights for the
// trained net will be stored in the file specified by the Parameter object.
void training( Parameters &params, vector<YearData> &trainingSet )
{
	// Create a neural net
	Net ann( params.GetNodesPerLayer(), params.GetEta(), params.GetAlpha() );
	
	// Get number of training epochs
	int num_epochs = params.GetEpochs();

	// Get error threshold for stopping
	double error_thresh = params.GetErrorThresh();

	// Only present floats to the thousandths place
	cout << setprecision( 3 );

	// Epoch loop
	for ( int i = 0; i < num_epochs; i++ )
	{
		// Shuffle order of records
		random_shuffle( trainingSet.begin(), trainingSet.end() );

		// Perform feed forward and back prop once for each record
		for ( int j = 0; j < trainingSet.size(); j++ )
		{
			ann.feed_forward( trainingSet[j].inputs );
			ann.back_prop( trainingSet[j].class_outputs );
		}

		// Stop if error threshold is reached
		if ( ann.get_avg_error() < error_thresh )
		{
			cout << "Epoch" << setw( 7 ) << i << ": RMS error = "
				 << ann.get_avg_error() << endl;
			cout << "Error Threshold Met" << endl;
			break;
		}

		// Print out every ten epochs
		if ( i % 10 == 0 )
			cout << "Epoch" << setw( 7 ) << i << ": RMS error = "
				 << ann.get_avg_error() << endl;
	}

	// Save net's weights to the weight file
	ann.print_weights( params.GetWeightsFileName() );

	return;
}