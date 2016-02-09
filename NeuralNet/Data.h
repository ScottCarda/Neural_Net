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
	vector<double> inputs;
};

class Data
{
public:
	Data( const string TrainTestDataFileName, const int NumYearsBurnedAcerage, 
		const int NumMonthsPDSI, const int EndMonthCurrYear );
	~Data();

	vector<struct TrainingTestingSet> GetAllData();
	void ShuffleData();

private:
	vector<struct TrainingTestingSet> _data;

	
};