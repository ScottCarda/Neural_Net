/*
                    ***** ANNtrain.cpp *****

This file is the entry point for the ANNtest program. It contains the
definition for the main function as well as the testing function, which
carries out the algorithm for performing testing on a neural net.

This file starts in main. It checks to make sure there is a correct number
of command line arguments, and outputs and error is it doesn't. The main
function then retrieves the parameter file information and the test data.
The main then calls the testing function. The testing function starts by
creating the nueral net and reading in the weights. Next the function 
goes through each test set and performs the feed forward and gets the output.
The file then outputs the run number, the actual output and then the predicted
output. If the actual and the predicted don't match then the testing function
outputs a * to inform the user of the difference. While going through each
test set the testing function calculates the RMS error and keeps track of
the number of correct predictions. After the testing function finishes all 
the test sets it outputs the average RMS error and the accuracy of this test
run.

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

// Tests a neural net with the testing set provided.
void testing( Parameters &params, vector<YearData> &testSet );

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

	// Perform testing on the data
	vector<YearData> data_vec = data.GetAllData();
	testing( params, data_vec );

	return 0;
}

/******************************************************************************/
/*                              Testing Function                              */
/******************************************************************************/

// Tests a neural net with the testing set provided.
void testing( Parameters &params, vector<YearData> &testSet )
{
	int numberCorrect = 0;		// count of number of correct predictions
	bool low, mid, high;		// bools to determine what the actual was
	unsigned int i;		// count of total number of test sets
	double percentCorrect;		// the percent of correct predictions
	vector<double> outputsFromNet;		// outputs from the net
	vector<double> expected_outputs;		// expected outputs
	double error = 0.0;		// current error of the output node
	double avg_error = 0.0;		// average error of the net

	// create the net
	Net ann( params.GetNodesPerLayer(), params.GetEta(), params.GetAlpha() );

	// read in weight file
	ann.read_in_weights( params.GetWeightsFileName(), params.GetNodesPerLayer() );

	// output the type of format
	cout << "Sample, Actual, Predicted" << endl;

	// loop through each test set
	for ( i = 0; i < testSet.size(); i++ )
	{
		// set the low, mid, and high bools to false
		low = false;
		mid = false;
		high = false;

		// perform the feed forward on the current test set
		ann.feed_forward( testSet[i].inputs );

		// get the output of the net for the current test set
		ann.get_output( outputsFromNet );

		// output the sample
		cout << setw(3) << (i+1) << ", ";

		// if the actual fire severity was mid
		if ( testSet[i].actualburnedacres > params.GetFireSeverityCutoffs().at( 0 ) &&
			testSet[i].actualburnedacres < params.GetFireSeverityCutoffs().at( 1 ) )
		{
			// output the mid
			cout << setw(10) <<"010, ";
			mid = true;
		}
		// if the actual fire severity was high
		else if ( testSet[i].actualburnedacres > params.GetFireSeverityCutoffs().at( 1 ) )
		{
			// output the high
			cout << setw(10) << "001, ";
			high = true;
		}
		// if the acutal fire severity was low
		else
		{
			// output the low
			cout << setw(10) << "100, ";
			low = true;
		}

		// if low fire severity is predicted
		if ( outputsFromNet.at( 0 ) > outputsFromNet.at( 1 ) &&
			outputsFromNet.at( 0 ) > outputsFromNet.at( 2 ) )
		{
			// output low
			cout << setw(7) << "100";

			// if it predicted correctly increment the count
			if ( low )
				numberCorrect++;
			// if its wrong star that line
			else
				cout << ", *";
		}
		// if mid fire severity is predicted
		else if ( outputsFromNet.at( 1 ) > outputsFromNet.at( 0 ) &&
			outputsFromNet.at( 1 ) > outputsFromNet.at( 2 ) )
		{
			// output mid
			cout << setw(7) << "010";

			// if it predicted correctly increment the count
			if ( mid )
				numberCorrect++;
			// if its wrong star that line
			else
				cout << ", *";
		}
		// if high fire severity is predcted
		else
		{
			// output high
			cout << setw(7) << "001";

			// if it predicted correctly increment the count
			if ( high )
				numberCorrect++;
			// if its wrong star that line
			else
				cout << ", *";
		}

		cout << endl;

		// put the actual into a vector
		if ( mid )
			expected_outputs.push_back( 0.0 );
		else
		{
			expected_outputs.push_back( 0.0 );
			expected_outputs.push_back( 0.0 );
		}
		expected_outputs.push_back( 1.0 );
		expected_outputs.push_back( 0.0 );
		expected_outputs.push_back( 0.0 );

		error = 0.0;
		// Loop through output_layer ( not bias ), setting the errors for each
		for ( unsigned j = 0; j < outputsFromNet.size() - 1; j++ )
		{
			double delta = expected_outputs[j] - outputsFromNet[j];
			// Sum the squared errors
			error += delta * delta;
		}

		// Calculate RMSE
		error /= outputsFromNet.size();
		error = sqrt( error );

		// Updates average error with the new value
		avg_error += ( error - avg_error ) / ( ( i + 1 )*outputsFromNet.size() );
		expected_outputs.clear();
	}

	// calculate the percentage of correct predictions
	percentCorrect = ( numberCorrect / (double) i ) * 100;

	// output the RMS error
	cout << "\nRMS error: " << avg_error << endl;

	// output the percentage of correct predictions
	cout << "accuracy: " << fixed << setprecision( 2 ) << percentCorrect;
	cout << " %" << endl;

	return;
}