/*
                    ***** ANNtrain.cpp *****

This file is the entry point for the ANNtrain program. It contains the
definition for the main function as well as the training function, which
carries out the algorithm for performing training on a neural net.

Training is done by performing feeding input into the net that has known, or
expected, output. The net will process the input and generate an output. This
output is then compared to the expected output, and, using this comparison,
the net performs a process known as back propagation. During back propagation
the net will correct, or train, the weight values on its weighted connections
between neurons. The process of feeding input into the net followed by back
propagation is carried out once on each training data record in a random order,
constituting one training epoch. The number of training epochs performed by
this program is found in the parameter file taken as a command-line argument.
Lastly the program will print out the neural net's weight values to a weight
file specified by the parameter file. It a weight file is found before training
is performed, the neural net will use those weights instead of randomly
generated ones, which will allow the net to pick up where it left off.

CSC547 Artificial Intelligence - Spring 2016

Author: Hannah Aker, Scott Carda, Cassidy Vollmer
*/

/******************************************************************************/
/*                                Include Files                               */
/******************************************************************************/

#include "Data.h"
#include "Net.h"
#include "Parameters.h"

#include <iomanip>

/******************************************************************************/
/*                             Function Prototypes                            */
/******************************************************************************/

// Trains a neural net with the training set provided. The weights for the
// trained net will be stored in the file specified by the Parameter object.
void training( Parameters &params, vector<YearData> &trainingSet );

/******************************************************************************/
/*                               Main Function                                */
/******************************************************************************/

// The entry point of the program.
int main( int argc, char* argv[] )
{
	// Ensure proper usage
	if ( argc != 2 )
	{
		cout << "Usage: CrossValidate <parameterfile>\n"
			<< "\tparameterfile: The filename for the parameter file."
			<< endl;
		return -1;
	}

	// Parse the parameter file
	Parameters params = Parameters( argv[1] );

	// Get and preprocess the data
	Data data = Data( params.GetTrainTestFileName(), params.GetNumYearsBurnedAcreage(),
		params.GetNumMonthsPDSI(), params.GetEndMonthCurrYear(), params.GetFireSeverityCutoffs() );

	// Print out file information
	cout << "Parameter file: " << params.GetParamFileName() << endl;
	cout << "Reading data from file: " << params.GetTrainTestFileName() << endl << endl;

	// Perform training on the data
	vector<YearData> data_vec = data.GetAllData();
	training( params, data_vec );

	return 0;
}

/******************************************************************************/
/*                             Training Function                              */
/******************************************************************************/

// Trains a neural net with the training set provided. The weights for the
// trained net will be stored in the file specified by the Parameter object.
void training( Parameters &params, vector<YearData> &trainingSet )
{
	string weights_filename = params.GetWeightsFileName();

	// Create a neural net
	Net ann( params.GetNodesPerLayer(), params.GetEta(), params.GetAlpha() );

	// Set the weights according to the weights file
	ann.read_in_weights( weights_filename, params.GetNodesPerLayer() );

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
		for ( unsigned int j = 0; j < trainingSet.size(); j++ )
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

		// Print out the average RMS error for every ten epochs
		if ( i % 10 == 0 )
		{
			cout << "Epoch" << setw( 7 ) << i << ": RMS error = "
				<< ann.get_avg_error() << endl;
			ann.reset_avg_error();
		}
	}

	// Save net's weights to the weight file
	ann.print_weights( weights_filename );

	return;
}