#include "Parameters.h"

Parameters::Parameters( const string ParamFileName)
{
	_paramfilename = ParamFileName;
	ifstream _paramfile;
	_paramfile.open(ParamFileName.c_str());

	string param;
	
	while (param.empty()) param = ParseLine(_paramfile);
	_weightsfilename = param;
	param.clear();
	while (param.empty()) param = ParseLine(_paramfile);
	_epochs = stoi(param);
	param.clear();
	while (param.empty()) param = ParseLine(_paramfile);
	_eta = stod(param);
	param.clear();
	while (param.empty()) param = ParseLine(_paramfile);
	_alpha = stod(param);
	param.clear();
	while (param.empty()) param = ParseLine(_paramfile);
	_errorthresh = stod(param);
	param.clear();
	while (param.empty()) param = ParseLine(_paramfile);
	_numlayers = stoi(param);
	param.clear();
	for (int i = 0; i < (_numlayers+1); i++)
	{
		_paramfile >> param;
		_nodesperlayer.push_back(stoi(param));
	}
	param.clear();
	while (param.empty()) param = ParseLine(_paramfile);
	_traintestfilename = param;
	param.clear();
	while (param.empty()) param = ParseLine(_paramfile);
	_numyearsburnedacreage = stoi(param);
	param.clear();
	while (param.empty()) param = ParseLine(_paramfile);
	_nummonthspdsi = stoi(param);
	param.clear();
	while (param.empty()) param = ParseLine(_paramfile);
	_endmonthcurryear = stoi(param);
	param.clear();
	while (param.empty()) param = ParseLine(_paramfile);
	_numoutputclasses = stoi(param);
	param.clear();
	while (param.empty()) param = ParseLine(_paramfile);	
	for (int i = 0; i < (_numoutputclasses - 1); i++)
	{
		_fireseveritycutoffs.push_back(stoi(param));
		_paramfile >> param;
	}


	_paramfile.close();

}

string Parameters::ParseLine(ifstream &File)
{
	string line;
	string parsed;
	getline(File, line);
	parsed = line.substr(0, line.find_first_of("#\n\t\r"));
	return parsed;

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
vector<unsigned> Parameters::GetNodesPerLayer(){ return _nodesperlayer;}
string Parameters::GetTrainTestFileName(){ return _traintestfilename;}
int Parameters::GetNumYearsBurnedAcreage(){ return _numyearsburnedacreage;}
int Parameters::GetNumMonthsPDSI(){ return _nummonthspdsi;}
int Parameters::GetEndMonthCurrYear(){ return _endmonthcurryear;}
int Parameters::GetNumOutputClasses(){ return _numoutputclasses;}
vector<int> Parameters::GetFireSeverityCutoffs(){ return _fireseveritycutoffs;}
