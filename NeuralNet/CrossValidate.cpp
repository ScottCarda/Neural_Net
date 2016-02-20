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

void crossValidate( Parameters &params, vector<YearData> &cvSet );

int main( int argc, char* argv[] )
{
	if ( argc != 2 )
	{
		cout << "Usage: CrossValidate <parameterfile>\n"
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
	crossValidate( params, data_vec );

	return 0;
}

void crossValidate( Parameters &params, vector<YearData> &cvSet )
{
	//split cv set into training set and testing set
	vector<YearData> trainingSet;
	YearData testSet;
	int numberCorrect = 0;
	bool low, mid, high;
	double percentCorrect;
	vector<double> outputsFromNet;
	// Get number of training epochs
	int num_epochs = params.GetEpochs();

	// Get error threshold for stopping
	double error_thresh = params.GetErrorThresh();

	cout << "Year, Burned, Actual, Predicted (training error)" << endl;

	for ( unsigned int q = 0; q < cvSet.size(); q++ )
	{
		trainingSet = vector<YearData>( cvSet );
		trainingSet.erase( trainingSet.begin() + q );
		testSet = YearData( cvSet.at( q ) );
		// Create a neural net
		Net ann( params.GetNodesPerLayer(), params.GetEta(), params.GetAlpha() );

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
				break;

		}

		// set the low, mid, and high bools to false
		low = false;
		mid = false;
		high = false;

		// perform the feed forward on the current test set
		ann.feed_forward( testSet.inputs );

		// get the output of the net for the current test set
		ann.get_output( outputsFromNet );

		// output the sample
		cout << testSet.year << ", " << setw( 6 ) << right << testSet.actualburnedacres << ", ";

		// if the actual fire severity was mid
		if ( testSet.actualburnedacres > params.GetFireSeverityCutoffs().at( 0 ) &&
			testSet.actualburnedacres < params.GetFireSeverityCutoffs().at( 1 ) )
		{
			// output the mid
			cout << setw( 8 ) << right << "010, ";
			mid = true;
		}
		// if the actual fire severity was high
		else if ( testSet.actualburnedacres > params.GetFireSeverityCutoffs().at( 1 ) )
		{
			// output the high
			cout << setw( 8 ) << right << "001, ";
			high = true;
		}
		// if the acutal fire severity was low
		else
		{
			// output the low
			cout << setw( 8 ) << right << "100, ";
			low = true;
		}

		// if low fire severity is predicted
		if ( outputsFromNet.at( 0 ) > outputsFromNet.at( 1 ) &&
			outputsFromNet.at( 0 ) > outputsFromNet.at( 2 ) )
		{


			// if it predicted correctly increment the count
			if ( low )
			{
				cout << setw( 9 ) << right << "100   ";
				numberCorrect++;

			}
			// if its wrong star that line
			else
				cout << setw( 9 ) << right << "100, *";
		}
		// if mid fire severity is predicted
		else if ( outputsFromNet.at( 1 ) > outputsFromNet.at( 0 ) &&
			outputsFromNet.at( 1 ) > outputsFromNet.at( 2 ) )
		{
			// if it predicted correctly increment the count
			if ( mid )
			{
				cout << setw( 9 ) << right << "010   ";
				numberCorrect++;
			}
			// if its wrong star that line
			else
				cout << setw( 9 ) << right << "010, *";
		}
		// if high fire severity is predcted
		else
		{
			// if it predicted correctly increment the count
			if ( high )
			{
				cout << setw( 9 ) << right << "001   ";
				numberCorrect++;

			}
			// if its wrong star that line
			else
				cout << setw( 9 ) << right << "001, *";
		}

		cout << "          (" << ann.get_avg_error() << ")" << endl;

	}


	percentCorrect = ( numberCorrect / (double) cvSet.size() ) * 100;
	cout << "Overall accuracy: " << fixed << setprecision( 2 ) << percentCorrect << " %" << endl;

	return;
}