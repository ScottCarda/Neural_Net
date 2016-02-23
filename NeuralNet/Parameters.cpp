/*
                    ***** Parameters.cpp *****

This file contains the definitions for the Parameters class's member functions. See
the Parameters.h file for a description of the class.

CSC547 Artificial Intelligence - Spring 2016

Author: Hannah Aker, Scott Carda, Cassidy Vollmer
*/

/******************************************************************************/
/*                                Include Files                               */
/******************************************************************************/

#include "Parameters.h"

/******************************************************************************/
/*                    Parameters Class Member Functions                       */
/******************************************************************************/

// Constructor for Parameters class
Parameters::Parameters(const string ParamFileName)
{
	//save Parameter file name
	_paramfilename = ParamFileName;
	//create a stream for reading in file
	ifstream _paramfile;
	//open the file
	_paramfile.open(ParamFileName.c_str());
	//variable to temporarily store a parameter string
	string param;

	//check if _paramfile opened correctly
	if (_paramfile.is_open())
	{
		//get lines until the line doesn't start with a pound sign
		while (param.empty()) param = ParseLine(_paramfile);
		//save the weights file name
		_weightsfilename = param;
		//clear the param string for reading new params
		param.clear();

		//get Epochs
		while (param.empty()) param = ParseLine(_paramfile);
		_epochs = stoi(param);
		param.clear();

		//get Eta (learning rate)
		while (param.empty()) param = ParseLine(_paramfile);
		_eta = stod(param);
		param.clear();

		//get Alpha (momentum)
		while (param.empty()) param = ParseLine(_paramfile);
		_alpha = stod(param);
		param.clear();

		//get Error Threshold
		while (param.empty()) param = ParseLine(_paramfile);
		_errorthresh = stod(param);
		param.clear();

		//get Number of Layers
		while (param.empty()) param = ParseLine(_paramfile);
		_numlayers = stoi(param);
		param.clear();

		//get the Number of Nodes in each layer
		//special read because there are multiple parameters in the same line
		for ( int i = 0; i < (_numlayers + 1); i++)
		{
			_paramfile >> param;
			_nodesperlayer.push_back(stoi(param));
		}
		param.clear();

		//get the filename for the training and testing file 
		while (param.empty()) param = ParseLine(_paramfile);
		_traintestfilename = param;
		param.clear();

		//get the number of burned acreage to train with 
		while (param.empty()) param = ParseLine(_paramfile);
		_numyearsburnedacreage = stoi(param);
		param.clear();

		//get the number of months of PDSI data to train with 
		while (param.empty()) param = ParseLine(_paramfile);
		_nummonthspdsi = stoi(param);
		param.clear();

		//get the last month of data for the year
		while (param.empty()) param = ParseLine(_paramfile);
		_endmonthcurryear = stoi(param);
		param.clear();

		//get the number of output classes
		while (param.empty()) param = ParseLine(_paramfile);
		_numoutputclasses = stoi(param);
		param.clear();

		//get the class cutoffs for the burned acreage
		while (param.empty()) param = ParseLine(_paramfile);
		for ( int i = 0; i < (_numoutputclasses - 1); i++)
		{
			_fireseveritycutoffs.push_back(stoi(param));
			_paramfile >> param;
		}
		param.clear();
	}

	//exit program if file can't open
	else
	{
		cout << "Unable to open given Parameter file. Exiting." << endl;
		exit(EXIT_FAILURE);
	}
	//close the parameters file
	_paramfile.close();

}

//Helper function for parsing lines in parameter file
string Parameters::ParseLine(ifstream &File)
{
	string line;
	string parsed;
	//get a line
	getline(File, line);
	//make the line into just the part that doesn't start with a #, new line, tab, or return
	parsed = line.substr(0, line.find_first_of(" #\n\t\r"));
	//return the parsed line
	return parsed;

}

//Destructor
Parameters::~Parameters() {}

//functions to get class variables, read only- no setters
string Parameters::GetParamFileName() { return _paramfilename; }
string Parameters::GetWeightsFileName() { return _weightsfilename; }
int Parameters::GetEpochs() { return _epochs; }
double Parameters::GetEta() { return _eta; } //learning rate
double Parameters::GetAlpha() { return _alpha; } //momentum
double Parameters::GetErrorThresh() { return _errorthresh; }
int Parameters::GetNumLayers() { return _numlayers; }
vector<unsigned> Parameters::GetNodesPerLayer() { return _nodesperlayer; }
string Parameters::GetTrainTestFileName() { return _traintestfilename; }
int Parameters::GetNumYearsBurnedAcreage() { return _numyearsburnedacreage; }
int Parameters::GetNumMonthsPDSI() { return _nummonthspdsi; }
int Parameters::GetEndMonthCurrYear() { return _endmonthcurryear; }
int Parameters::GetNumOutputClasses() { return _numoutputclasses; }
vector<int> Parameters::GetFireSeverityCutoffs() { return _fireseveritycutoffs; }
