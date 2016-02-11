#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm> 

using namespace std;

struct TrainingTestingSet
{
	int year;
	int actualburnedacres;
	vector<double> class_outputs;
	vector<double> inputs;
};

class Data
{
public:
	Data( const string TrainTestDataFileName, const int NumYearsBurnedAcerage, 
		const int NumMonthsPDSI, const int EndMonthCurrYear, vector<int> &FireSeverityCutoffs );
	~Data();

	vector<struct TrainingTestingSet> GetAllData();
	void ShuffleData();

private:
	vector<struct TrainingTestingSet> _data;

	
};