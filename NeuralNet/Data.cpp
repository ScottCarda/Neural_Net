#include "Data.h"

Data::Data(const string TrainTestDataFileName, const int NumYearsBurnedAcerage,
	const int NumMonthsPDSI, const int EndMonthCurrYear, vector<int> &FireSeverityCutoffs )
{

	ifstream _traintestfile;
	_traintestfile.open(TrainTestDataFileName.c_str());
	if (_traintestfile.is_open())
	{
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
			if (!line.empty())
				year.push_back(stod(line));
			if (!year.empty())
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
		if ((NumMonthsPDSI / 12) > NumYearsBurnedAcerage)
			startyear = NumMonthsPDSI / 12;
		else
			startyear = NumYearsBurnedAcerage;

		for (int i = startyear; i < years.size(); i++)
		{
			YearData set;
			set.year = years[i][0];
			set.actualburnedacres = years[i][1];

			set.class_outputs.push_back( 0 );
			set.class_outputs.push_back( 0 );
			set.class_outputs.push_back( 0 );

			if ( set.actualburnedacres < FireSeverityCutoffs[0] )
				set.class_outputs[0] = 1;
			else if ( set.actualburnedacres < FireSeverityCutoffs[1] )
				set.class_outputs[1] = 1;
			else
				set.class_outputs[1] = 1;

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

	//exit program if file can't open
	else
	{
		cout << "Unable to open given Training and Testing Data file. Exiting." << endl;
		exit(EXIT_FAILURE);
	}

}

Data::~Data()
{

}

vector<YearData> Data::GetAllData()
{
	vector<YearData> _returndata = vector<YearData>(_data);
	return _returndata;
}
