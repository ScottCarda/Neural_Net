/*
***** Parameters.h *****

Contains the definitions for the Parameters class. The Parameters class contains
all the information included in the Parameter.prm file. The variables in this class
are read only, and can only be accessed via Get methods. 

CSC547 Artificial Intelligence - Spring 2016

Author: Hannah Aker, Scott Carda, Cassidy Vollmer
*/

/******************************************************************************/
/*                                Include Files                               */
/******************************************************************************/
#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

/******************************************************************************/
/*                          Parameters Class Definition                       */
/******************************************************************************/
class Parameters
{
public:
	Parameters(const string ParamFileName);
	string ParseLine(ifstream &File);
	~Parameters();

	string GetParamFileName();
	string GetWeightsFileName();
	int GetEpochs();
	double GetEta(); //learning rate
	double GetAlpha(); //momentum
	double GetErrorThresh();
	int GetNumLayers();
	vector<int> GetNodesPerLayer();
	string GetTrainTestFileName();
	int GetNumYearsBurnedAcreage();
	int GetNumMonthsPDSI();
	int GetEndMonthCurrYear();
	int GetNumOutputClasses();
	vector<int> GetFireSeverityCutoffs();

private:
	string _paramfilename;
	string _weightsfilename;
	int _epochs;
	double _eta; //learning rate
	double _alpha; //momentum
	double _errorthresh;
	int _numlayers;
	vector<int> _nodesperlayer;
	string _traintestfilename;
	int _numyearsburnedacreage;
	int _nummonthspdsi;
	int _endmonthcurryear;
	int _numoutputclasses;
	vector<int> _fireseveritycutoffs;

};

