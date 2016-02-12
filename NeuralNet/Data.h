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
/*                            Training Definition                           */
/******************************************************************************/
struct YearData
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

	vector<YearData> GetAllData();

private:
	vector<YearData> _data;
};