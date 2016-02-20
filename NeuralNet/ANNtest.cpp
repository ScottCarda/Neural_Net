#include <ctime>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include "Data.h"
#include "Net.h"
#include "Parameters.h"

using namespace std;

void testing( Parameters &params, vector<YearData> &testSet );

int main( int argc, char* argv[] )
{
	if ( argc != 2 )
	{
		cout << "Usage: ANNtest <parameterfile>\n"
			<< "\tparameterfile: The filename for the parameter file."
			<< endl;
		return -1;
	}

	Parameters params = Parameters( argv[1] );
	Data data = Data( params.GetTrainTestFileName(), params.GetNumYearsBurnedAcreage(),
		params.GetNumMonthsPDSI(), params.GetEndMonthCurrYear(), params.GetFireSeverityCutoffs() );

	cout << "Parameter file: " << params.GetParamFileName() << endl;
	cout << "Reading data from file: " << params.GetTrainTestFileName() << endl << endl;

	vector<YearData> data_vec = data.GetAllData();
	testing( params, data_vec );

	return 0;
}

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
		cout << i << ", ";

		// if the actual fire severity was mid
		if ( testSet[i].actualburnedacres > params.GetFireSeverityCutoffs().at( 0 ) &&
			testSet[i].actualburnedacres < params.GetFireSeverityCutoffs().at( 1 ) )
		{
			// output the mid
			cout << "010, ";
			mid = true;
		}
		// if the actual fire severity was high
		else if ( testSet[i].actualburnedacres > params.GetFireSeverityCutoffs().at( 1 ) )
		{
			// output the high
			cout << "001, ";
			high = true;
		}
		// if the acutal fire severity was low
		else
		{
			// output the low
			cout << "100, ";
			low = true;
		}

		// if low fire severity is predicted
		if ( outputsFromNet.at( 0 ) > outputsFromNet.at( 1 ) &&
			outputsFromNet.at( 0 ) > outputsFromNet.at( 2 ) )
		{
			// output low
			cout << "100";

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
			cout << "010";

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
			cout << "001";

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