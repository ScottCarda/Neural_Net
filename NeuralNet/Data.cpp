#include "Data.h"

Data::Data(const string TrainTestDataFileName, const int NumYearsBurnedAcerage,
	const int NumMonthsPDSI, const int EndMonthCurrYear)
{
	
	ifstream _traintestfile;
	_traintestfile.open(TrainTestDataFileName.c_str());

	string line;
	//Get header lines, don't bother parsing
	getline(_traintestfile, line);
	getline(_traintestfile, line);

	vector<vector<double>> years;
	vector<double> year;

	while (!_traintestfile.eof())
	{
		year.clear();
		for (int j = 0; j < 13; j++)
		{

			getline(_traintestfile, line, ',');
			if (!line.empty())
			year.push_back(stod(line));

		}

		getline(_traintestfile, line);
		if( !line.empty())
		year.push_back(stod(line));
		if(!year.empty())
		years.push_back(year);

	}

	_traintestfile.close();

	double burnedacresmax = 0, burnedacresmin = 0;
	double pdsimax = 0, pdsimin = 0;
	for (int i = 0; i < years.size(); i++)
	{
		if (years[i][1] > burnedacresmax)
			burnedacresmax = years[i][1];
		if (years[i][1] < burnedacresmin)
			burnedacresmin = years[i][1];
			for (int j = 2; j < 14; j++)
			{
				if (years[i][j] > pdsimax)
					pdsimax = years[i][j];
				if (years[i][j] < pdsimin)
					pdsimin = years[i][j];
			}
	}

	int startyear;
	if (NumMonthsPDSI / 12 > NumYearsBurnedAcerage)
		startyear = NumMonthsPDSI / 12;
	else
		startyear = NumYearsBurnedAcerage;

	for (int i = startyear ; i < years.size(); i++)
	{
		TrainingTestingSet set;
		set.year = years[i][0];
		set.actualburnedacres = years[i][1];
		for (int j = 0; j < NumYearsBurnedAcerage; j++)
			set.inputs.push_back((years[i - (j + 1)][1] - burnedacresmin) / (burnedacresmax - burnedacresmin));
		int k = EndMonthCurrYear + 2;
		int l = i;
		for (int j = 0; j < NumMonthsPDSI; j++, k--)
		{
			set.inputs.push_back((years[l][k] - pdsimin) / (pdsimax - pdsimin));
				if (k <= 2)
				{
					k = 13;
					l = l - 1;
				}
		}
_data.push_back(set);
	}

	
}

Data::~Data()
{
	
}

vector<struct TrainingTestingSet> Data::GetAllData()
{
	return _data;
}
void Data::ShuffleData()
{
	random_shuffle(_data.begin(), _data.end());
}