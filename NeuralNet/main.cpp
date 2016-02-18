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
	//testing( params, data.GetAllData() );
	//crossValidate(params, data.GetAllData());

	return 0;
}

void testing(Parameters &params, vector<YearData> &testSet)
{
	int numberCorrect = 0;
	bool low, mid, high;
	int i;
	double percentCorrect;
	Net ann(params.GetNodesPerLayer(), params.GetEta(), params.GetAlpha());
	vector<double> outputsFromNet;

	// read in weight file
	ann.read_in_weights(params.GetWeightsFileName(), params.GetNodesPerLayer());

	cout << "Sample, Actual, Predicted" << endl;

	for (i = 0; i < testSet.size(); i++)
	{
		low = false;
		mid = false;
		high = false;
		ann.feed_forward(testSet[i].inputs);
		ann.get_output(outputsFromNet);
		
		cout << i << ", ";

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

		if (testSet[i].actualburnedacres > params.GetFireSeverityCutoffs().at(0) && testSet[i].actualburnedacres < params.GetFireSeverityCutoffs().at(1))
		{
			cout << "010";
			if (mid) 
				numberCorrect++;
			else
				cout << ", *";
		}
		else if (testSet[i].actualburnedacres > params.GetFireSeverityCutoffs().at(1))
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

	percentCorrect = numberCorrect / (double)i;

	cout << "\nRMS error: fix this" << ann.get_avg_error() << endl;

	cout << "accuracy: " << fixed << setprecision(2) << percentCorrect << " %" << endl;
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
		{
			cout << "Epoch" << setw( 7 ) << i << ": RMS error = "
				<< ann.get_avg_error() << endl;
			ann._back_prop_count = 0;
			ann._avg_error;
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
	int i;
	double percentCorrect;
	vector<double> outputsFromNet;
	// Get number of training epochs
	int num_epochs = params.GetEpochs();

	// Get error threshold for stopping
	double error_thresh = params.GetErrorThresh();		
	
	cout << "Sample, Actual, Predicted" << endl;
	for ( int q = 0; q < cvSet.size(); q++)
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
			for (int j = 0; j < trainingSet.size(); j++)
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

percentCorrect = numberCorrect / (double)cvSet.size();
	cout << "accuracy: " << fixed << setprecision(2) << percentCorrect << " %" << endl;
	return;
}

