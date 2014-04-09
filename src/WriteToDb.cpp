//============================================================================
// Name        : WriteToDb.cpp
// Author      : TonyYou
// Version     :
// Copyright   : Tony Own
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <boost/algorithm/string.hpp>
#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_DEPRECATED
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/progress.hpp>
#include "CsvFileReader.h"
#include "DatabaseHelper.h"

using namespace std;
using namespace boost;
namespace fs = boost::filesystem;

void RecusiveListFiles(fs::path& fpath) {

	DatabaseHelper helper;
	string filepath = "/home/tonyyou/workspace/WriteToDb/data/20130527/OTC/2221.csv";
	string csvfile = "2221.csv";
	string category = "OTC";
	string date = "20130527";

	fs::recursive_directory_iterator beg_iter(fpath);
	fs::recursive_directory_iterator end_iter;
    for (; beg_iter != end_iter; ++beg_iter) {
        if (fs::is_directory(*beg_iter)){
            continue;
        } else {
        	cout<<beg_iter->path().file_string()<<endl;//path

        	filepath = beg_iter->path().file_string();
        	csvfile = beg_iter->path().filename().file_string();
        	category = beg_iter->path().parent_path().filename().file_string();
        	date = beg_iter->path().parent_path().parent_path().filename().file_string();

        	CsvFileReader reader(filepath,csvfile,category,date);
        	helper.createTableByDate(date);
        	helper.createJumpingTableByDate(date);

        	int startIndex = helper.getRowSize(date)+1;

        	vector<Summery> list = reader.getSummeryList();

        	string mcsvfile = csvfile;
            string::size_type foundpos = mcsvfile.find(".csv");

            if ( foundpos != string::npos )
            	mcsvfile.erase(mcsvfile.begin() + foundpos, mcsvfile.begin() + foundpos + string(".csv").length());

            if(list.size()>0)
        	helper.insertIndexTable(date,mcsvfile,startIndex,list.size());

        	for(size_t index=0;index<list.size();index++){
        		Summery summery=list.at(index);
        		helper.insertToDb(
        				date,
        				summery.stockid,
        				summery.brokerid,
        				summery.averagebuyprice,
        				summery.totalbuyvol,
        				summery.averagesellprice,
        				summery.totalsellvol);
        	}
        }
    }
}

int main() {
	// for test
/*	string filepath = "/home/tonyyou/workspace/WriteToDb/data/20130527/OTC/2221.csv";
	string csvfile = "2221.csv";
	string category = "OTC";
	string date = "20130527";*/

    boost::progress_timer t( std::clog );
    fs::path full_path( fs::initial_path<fs::path>() );

    full_path = fs::system_complete( fs::path( "data/" ) );
    RecusiveListFiles(full_path);

/*
 *
	CsvFileReader reader(filepath,
			csvfile,
			category,
			date);

	vector<Summery> list = reader.getSummeryList();

	DatabaseHelper helper;
	helper.createTableByDate(date);

	for(size_t index=0;index<list.size();index++){
		Summery summery=list.at(index);
		helper.insertToDb(
				date,
				summery.stockid,
				summery.brokerid,
				summery.averagebuyprice,
				summery.totalbuyvol,
				summery.averagesellprice,
				summery.totalsellvol);
	}
	cout<< "Finished!" <<endl;*/
	return 0;
}
