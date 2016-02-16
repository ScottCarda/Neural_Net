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

void training( Parameters &params, vector<YearData> &trainingSet );

int main( int argc, char* argv[] )
{
	if ( argc != 2 )
	{
		cout << "Usage: ANNtrain <parameterfile>\n"
			 << "\tparameterfile: The filename for the parameter file."
			 << endl;
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