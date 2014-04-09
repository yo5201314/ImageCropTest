/*
 * CsvFileReader.h
 *
 *  Created on: Sep 20, 2013
 *      Author: tonyyou
 */
 
#ifndef CSVFILEREADER_H_
#define CSVFILEREADER_H_
#include <iostream>
#include <stdlib.h>
#include <boost/algorithm/string.hpp>
#include <vector>
using namespace boost;
using namespace std;

class Summery{
public:
	string brokerid;
	string stockid;

	unsigned int totalbuyvol;
	unsigned long totalbuyprice;
	float averagebuyprice;

	unsigned int totalsellvol;
	unsigned long totalsellprice;
	float averagesellprice;
};

class Rawdata{
public:
	int serialno;
	string brokeid;
	string stockid;
	double price;
	int  buyvol;
	int sellvol;
};

class CsvFileReader {
private:
	string mFilepath;
	string mCsvfile;
	string mCategory;
	string mDate;

	vector<Rawdata> mRawdataList;
	vector<Summery> mSummeryList;
    int counter;


public:
    vector<Rawdata> getRawdataList();
    vector<Summery> getSummeryList();
	CsvFileReader(string filepath,string csvfile, string category, string date);
	virtual ~CsvFileReader();
private:
	void SummeryRawdata();
	void ReadFileIn();
    Rawdata covert2RawData(int serialno,string stockid,string brokerId,string price,string buyvol,string sellvol);

};

#endif /* CSVFILEREADER_H_ */
