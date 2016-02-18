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

void training( Parameters &params, vector<YearData> &trainingSet );
void testing(Parameters &params, vector<YearData> &testSet);
void crossValidate(Parameters &params, vector<YearData> &cvSet);

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
	cout << "Reading data from file: " << params.GetTrainTestFileName() << endl << endl;

	training( params, data.GetAllData() );
	testing( params, data.GetAllData() );
	crossValidate(params, data.GetAllData());

	return 0;
}

// Tests a neural net with the testing set provided.
void testing(Parameters &params, vector<YearData> &testSet)
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
	Net ann(params.GetNodesPerLayer(), params.GetEta(), params.GetAlpha());

	// read in weight file
	ann.read_in_weights(params.GetWeightsFileName(), params.GetNodesPerLayer());

	// output the type of format
	cout << "Sample, Actual, Predicted" << endl;

	// loop through each test set
	for (i = 0; i < testSet.size(); i++)
	{
		// set the low, mid, and high bools to false
		low = false;
		mid = false;
		high = false;

		// perform the feed forward on the current test set
		ann.feed_forward(testSet[i].inputs);

		// get the output of the net for the current test set
		ann.get_output(outputsFromNet);

		// output the sample
		cout << i << ", ";

		// if the actual fire severity was mid
		if (testSet[i].actualburnedacres > params.GetFireSeverityCutoffs().at(0) &&
			testSet[i].actualburnedacres < params.GetFireSeverityCutoffs().at(1))
		{
			// output the mid
			cout << "010, ";
			mid = true;
		}
		// if the actual fire severity was high
		else if (testSet[i].actualburnedacres > params.GetFireSeverityCutoffs().at(1))
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
		if (outputsFromNet.at(0) > outputsFromNet.at(1) &&
			outputsFromNet.at(0) > outputsFromNet.at(2))
		{
			// output low
			cout << "100";

			// if it predicted correctly increment the count
			if (low)
				numberCorrect++;
			// if its wrong star that line
			else
				cout << ", *";
		}
		// if mid fire severity is predicted
		else if (outputsFromNet.at(1) > outputsFromNet.at(0) &&
			outputsFromNet.at(1) > outputsFromNet.at(2))
		{
			// output mid
			cout << "010";

			// if it predicted correctly increment the count
			if (mid)
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
			if (high)
				numberCorrect++;
			// if its wrong star that line
			else
				cout << ", *";
		}

		cout << endl;

		// put the actual into a vector
		if (mid)
			expected_outputs.push_back(0.0);
		else
		{
			expected_outputs.push_back(0.0);
			expected_outputs.push_back(0.0);
		}
		expected_outputs.push_back(1.0);
		expected_outputs.push_back(0.0);
		expected_outputs.push_back(0.0);
		/*
		if (low)
		{
			double delta = expected_outputs[0] - outputsFromNet[0];
			// Sum the squared errors
			error += delta * delta;
		}
		else if (mid)
		{
			double delta = expected_outputs[1] - outputsFromNet[1];
			// Sum the squared errors
			error += delta * delta;
		}
		else
		{
			double delta = expected_outputs[2] - outputsFromNet[2];
			// Sum the squared errors
			error += delta * delta;
		}*/

		error = 0.0;
		// Loop through output_layer ( not bias ), setting the errors for each
		for (unsigned j = 0; j < outputsFromNet.size() - 1; j++)
		{
			double delta = expected_outputs[j] - outputsFromNet[j];
			// Sum the squared errors
			error += delta * delta;
		}

		// Calculate RMSE
		error /= outputsFromNet.size();
		error = sqrt(error);

		// Updates average error with the new value
		avg_error += (error - avg_error) / ((i + 1)*outputsFromNet.size());
		expected_outputs.clear();
	}

	// calculate the percentage of correct predictions
	percentCorrect = (numberCorrect / (double)i) * 100;

	// output the RMS error
	cout << "\nRMS error: " << avg_error << endl;

	// output the percentage of correct predictions
	cout << "accuracy: " << fixed << setprecision(2) << percentCorrect;
	cout << " %" << endl;

	return;
}

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

void crossValidate(Parameters &params, vector<YearData> &cvSet)
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
	
	cout << "Sample, Actual, Predicted" << endl;
	for ( unsigned int q = 0; q < cvSet.size(); q++)
	{
		trainingSet = vector<YearData>(cvSet);
		trainingSet.erase(trainingSet.begin()+q);
		testSet = YearData(cvSet.at(q));
		// Create a neural net
		Net ann(params.GetNodesPerLayer(), params.GetEta(), params.GetAlpha());

		// Only present floats to the thousandths place
		cout << setprecision(3);

		// Epoch loop
		for (int i = 0; i < num_epochs; i++)
		{
			// Shuffle order of records
			random_shuffle(trainingSet.begin(), trainingSet.end());

			// Perform feed forward and back prop once for each record
			for (unsigned int j = 0; j < trainingSet.size(); j++)
			{
				ann.feed_forward(trainingSet[j].inputs);
				ann.back_prop(trainingSet[j].class_outputs);
			}

			// Stop if error threshold is reached
			if (ann.get_avg_error() < error_thresh)
				break;

		}

		//testing
		low = false;
		mid = false;
		high = false;
		//feed the data through the net
		ann.feed_forward(testSet.inputs);
		ann.get_output(outputsFromNet);

		cout << testSet.year << ", ";

		if (outputsFromNet.at(0) > outputsFromNet.at(1) && outputsFromNet.at(0) > outputsFromNet.at(2))
		{
			cout << "100, ";
			low = true;
		}
		else if (outputsFromNet.at(1) > outputsFromNet.at(0) && outputsFromNet.at(1) > outputsFromNet.at(2))
		{
			cout << "010, ";
			mid = true;
		}
		else
		{
			cout << "001, ";
			high = true;
		}

		if (testSet.actualburnedacres > params.GetFireSeverityCutoffs().at(0) && testSet.actualburnedacres < params.GetFireSeverityCutoffs().at(1))
		{
			cout << "010";
			if (mid)
				numberCorrect++;
			else
				cout << ", *";
		}
		else if (testSet.actualburnedacres > params.GetFireSeverityCutoffs().at(1))
		{
			cout << "001";
			if (high)
				numberCorrect++;
			else
				cout << ", *";
		}
		else
		{
			cout << "100";
			if (low)
				numberCorrect++;
			else
				cout << ", *";
		}

		cout << endl;
	}

percentCorrect = (numberCorrect / (double)cvSet.size()) *100;
	cout << "accuracy: " << fixed << setprecision(2) << percentCorrect << " %" << endl;
	return;
}
