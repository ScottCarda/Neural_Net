#include "Parameters.h"

Parameters::Parameters( const string paramfile)
{
	_paramfilename = paramfile;
	ifstream _data_file;
	_data_file.open(paramfile.c_str());



	_data_file.close();


}

Parameters::~Parameters()
{
}

string Parameters::GetParamFileName() { return _paramfilename; }
string Parameters::GetWeightsFileName(){ return _weightsfilename;}
int Parameters::GetEpochs(){ return _epochs;}
double Parameters::GetEta(){ return _eta;} //learning rate
double Parameters::GetAlpha(){ return _alpha;} //momentum
double Parameters::GetErrorThresh(){ return _errorthresh;}
int Parameters::GetNumLayers(){ return _numlayers;}
vector<int> Parameters::GetNodesPerLayer(){ return _nodesperlayer;}
string Parameters::GetTrainTestFileName(){ return _traintestfilename;}
int Parameters::GetNumYearsBurnedAcreage(){ return _numyearsburnedacreage;}
int Parameters::GetNumMonthsPDSI(){ return _nummonthspdsi;}
int Parameters::GetEndMonthCurrYear(){ return _endmonthcurryear;}
int Parameters::GetNumOutputClasses(){ return _numoutputclasses;}
vector<int> Parameters::GetFireSeverityCutoffs(){ return _fireseveritycutoffs;}
