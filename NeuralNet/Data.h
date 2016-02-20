/*
***** Data.h *****

Contains the definitions for the Data class. The Data class will read in all the
testing and training data when an instance of the Data class is constructed. If the
data file can't be opened, the program will inform the user and exit.

CSC547 Artificial Intelligence - Spring 2016

Author: Hannah Aker, Scott Carda, Cassidy Vollmer
*/

/******************************************************************************/
/*                                Include Files                               */
/******************************************************************************/
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm> 
#include <iostream>

using namespace std;

/******************************************************************************/
/*                            YearData Definition                             */
/******************************************************************************/
struct YearData
{
	int year;
	int actualburnedacres;
	vector<double> class_outputs; //corresponding to mild, moderate, severe fire danger
	vector<double> inputs;
};

/******************************************************************************/
/*                               Data Definition                              */
/******************************************************************************/
class Data
{
public:
	//constructor/destructor
	Data(const string TrainTestDataFileName, const int NumYearsBurnedAcerage,
		const int NumMonthsPDSI, const int EndMonthCurrYear, const vector<int> &FireSeverityCutoffs);
	~Data();

	//data getter
	vector<YearData> GetAllData();

private:
	//storage of all potential data sets
	vector<YearData> _data;
};