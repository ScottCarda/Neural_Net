/*
                    ***** Data.cpp *****

This file contains the definitions for the Data class's member functions. See
the Data.h file for a description of the class.

CSC547 Artificial Intelligence - Spring 2016

Author: Hannah Aker, Scott Carda, Cassidy Vollmer
*/

/******************************************************************************/
/*                                Include Files                               */
/******************************************************************************/

#include "Data.h"

/******************************************************************************/
/*                    Data Class Member Functions                       */
/******************************************************************************/

// Constructor for Data class
Data::Data(const string TrainTestDataFileName, const int NumYearsBurnedAcerage,
	const int NumMonthsPDSI, const int EndMonthCurrYear, const vector<int> &FireSeverityCutoffs)
{
	//open data file for reading
	ifstream _traintestfile;
	_traintestfile.open(TrainTestDataFileName.c_str());
	//check that it's open
	if (_traintestfile.is_open())
	{
		//string for reading in a line from the file
		string line;
		//Get header lines, don't bother parsing
		getline(_traintestfile, line);
		getline(_traintestfile, line);

		//year is one line of data from the file (year, burned acerage, and 12 months of PDSI data)
		//years contains all the data in the file, in the order that it's presented in the file
		vector<vector<double>> years;
		vector<double> year;

		//read to the end of the file
		while (!_traintestfile.eof())
		{
			//clear the temp vector
			year.clear();
			//read 13 parameters, the 14th is a special case, because it ends with whitespace
			for ( int j = 0; j < 13; j++)
			{
				//get a string until a comma is encountered
				getline(_traintestfile, line, ',');
				//if got a line, add it to the vector for that year
				if (!line.empty())
					year.push_back(stod(line));
			}
			//get the last parameter on the line
			getline(_traintestfile, line);
			//add if exists
			if (!line.empty())
				year.push_back(stod(line));
			//add the year to the data if exists
			if (!year.empty())
				years.push_back(year);
		}

		//close the file
		_traintestfile.close();

		//variables to track for normalizing data
		double burnedacresmax = 0, burnedacresmin = 0;
		double pdsimax = 0, pdsimin = 0;

		//loop through all years
		for ( unsigned int i = 0; i < years.size(); i++)
		{
			//if encountering a new min or new max, update variable
			if (years[i][1] > burnedacresmax)
				burnedacresmax = years[i][1];
			if (years[i][1] < burnedacresmin)
				burnedacresmin = years[i][1];
			//loop through PDSIs, if encountering new min or max, update variable
			for ( int j = 2; j < 14; j++)
			{
				if (years[i][j] > pdsimax)
					pdsimax = years[i][j];
				if (years[i][j] < pdsimin)
					pdsimin = years[i][j];
			}
		}

		//determine the first year with a full set of the data specified in the paramaters file
		int startyear;
		//if there are more years of PDSI data than years of burned acerage
		if ((NumMonthsPDSI / 12) > NumYearsBurnedAcerage)
			//start in where the PDSI data has enough prior data
			startyear = NumMonthsPDSI / 12;
		else
			//else start where the burned acerage has enough prior data
			startyear = NumYearsBurnedAcerage;

		//loop through the years, and create sets of data,
		//including the year as an indentifier, the expected output and a vector of inputs
		for ( unsigned int i = startyear; i < years.size(); i++)
		{
			//set the year and actual burned acres
			YearData set;
			set.year = years[i][0];
			set.actualburnedacres = years[i][1];

			//determine which output class the actual burned acerage falls in
			set.class_outputs.push_back(0);
			set.class_outputs.push_back(0);
			set.class_outputs.push_back(0);

			//below first cutoff
			if (set.actualburnedacres < FireSeverityCutoffs[0])
				set.class_outputs[0] = 1;
			//above first cutoff, below second cutoff
			else if (set.actualburnedacres < FireSeverityCutoffs[1])
				set.class_outputs[1] = 1;
			//above second cutoff
			else
				set.class_outputs[2] = 1;

			//push back normalized previous years' burned acerage
			//j corresponds to which previous year to go to, 
			//first add the year immediately prior, then the next year back, etc.
			for ( int j = 0; j < NumYearsBurnedAcerage; j++)
				set.inputs.push_back((years[i - (j + 1)][1] - burnedacresmin) / (burnedacresmax - burnedacresmin));

			//fun variables because we're stepping back through elements (zero based) 2-13 of each year
			//until we reach the correct number of months of PDSI data
			//k tracks which month of the year we're in 
			//(starting with the  last month of data, as specified in parameters)
			int k = EndMonthCurrYear + 2;
			//l tracks which year we are in (starting in the current year)
			int l = i;
			//j tracks how many months we've grabbed
			for ( int j = 0; j < NumMonthsPDSI; j++, k--)
			{
				//push back a month of data
				set.inputs.push_back((years[l][k] - pdsimin) / (pdsimax - pdsimin));
				//if we've hit the beginning of the year (index 2), go to the end of the previous year
				if (k <= 2)
				{
					//december of previous year (index 13)
					k = 13;
					//step back a year
					l = l - 1;
				}
			}

			//add the parsed, normalized set of data to the class data
			_data.push_back(set);
		}
	}

	//exit program if file can't open
	else
	{
		cout << "Unable to open given Training and Testing Data file. Exiting." << endl;
		exit(EXIT_FAILURE);
	}

}

//Destructor
Data::~Data() {}

//Getter function
vector<YearData> Data::GetAllData()
{
	//return a copy of the data
	vector<YearData> _returndata = vector<YearData>(_data);
	return _returndata;
}
