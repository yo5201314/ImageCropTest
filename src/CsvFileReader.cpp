/*
 * CsvFileReader.cpp
 *
 *  Created on: Sep 20, 2013
 *      Author: tonyyou
 */

#include "CsvFileReader.h"

#include <csv_parser/csv_parser.hpp>
#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_DEPRECATED
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/progress.hpp>
#include <boost/lexical_cast.hpp>
using namespace std;
using namespace boost;

CsvFileReader::CsvFileReader(string filepath,string csvfile, string category, string date) {
	mFilepath = filepath;
	mCsvfile = csvfile;
	mCategory = category;
	mDate = date;

	counter = 1;
	mRawdataList.clear();
	mSummeryList.clear();

	ReadFileIn();
	SummeryRawdata();
}

CsvFileReader::~CsvFileReader() {

}
vector<Rawdata> CsvFileReader::getRawdataList(){
	return mRawdataList;
}
vector<Summery> CsvFileReader::getSummeryList(){
	return mSummeryList;
}
void CsvFileReader::ReadFileIn(){

    if(mCsvfile.empty() ||  mCsvfile.find(".csv")==std::string::npos)
        return;

    cout<< mFilepath << endl;

    boost::algorithm::to_lower(mCategory);

    mDate.insert(4,1,'-');
    mDate.insert(7,1,'-');
//    cout<< mDate << endl;

    string::size_type foundpos = mCsvfile.find(".csv");

    if ( foundpos != string::npos )
        mCsvfile.erase(mCsvfile.begin() + foundpos, mCsvfile.begin() + foundpos + string(".csv").length());

//    cout<< mCsvfile << endl;

    enum cate { TSE=1,OTC,ETF};
    int categoryid = 0;
    if(mCategory.compare("otc")==0)
        categoryid = OTC;
    else if(mCategory.compare("tse")==0)
        categoryid = TSE;
    else if(mCategory.compare("etf")==0){
        categoryid = ETF;
    }

    const char * filename = mFilepath.c_str();//"/home/tonyyou/database/data/20130520/ETF/0052.csv";
    const char field_terminator = ',';
    const char line_terminator  = '\n';
    const char enclosure_char   = '"';
    csv_parser file_parser;

    /* Define how many records we're gonna skip. This could be used to skip the column definitions. */
    file_parser.set_skip_lines(3);

    /* Specify the file to parse */
    file_parser.init(filename);

    /* Here we tell the parser how to parse the file */
    if(categoryid==OTC)
        file_parser.set_enclosed_char(enclosure_char, ENCLOSURE_REQUIRED);

    file_parser.set_field_term_char(field_terminator);
    file_parser.set_line_term_char(line_terminator);
    unsigned int row_count = 1U;
        /* Check to see if there are more records, then grab each row one at a time */
        //case  0  empty
        //case  1  one data
        //case  2  two data
        string temp = "";
        string brokerId1 = "";
        string brokerId2 = "";
        string stockId = "";
        string price1 = "";
        string price2 = "";
        string buyVol1 = "";
        string buyVol2 = "";
        string sellVol1 = "";
        string sellVol2 = "";
        while(file_parser.has_more_rows())
        {
                unsigned int i = 0;
                unsigned int whichcase = 0;
                unsigned int rowsize = 0;
                int offset = 0;
                csv_row row = file_parser.get_row();
                rowsize = row.size();
                if(categoryid==OTC){
                    if( rowsize < 5 )
                        whichcase = 0;
                    else if( rowsize >=5 && rowsize < 10 ){
                        whichcase = 1;
                    }else if( rowsize >= 10 ){
                        whichcase = 2;
                    }
                }else{
                    if(rowsize < 6)
                        whichcase = 0;
                    else if(rowsize >=6 && rowsize < 11){
                        whichcase = 1;
                    }else if(!row[7].empty()){
                        whichcase = 2;
                    }else{
                        whichcase = 1;
                    }
                }

//                cout<< "case:" << whichcase <<"rowsize:" << rowsize << endl;

                for (i = 0; i < row.size(); i++)
                {
                        if(categoryid!=OTC && i >5)
                            offset=1;
                        else
                            offset=0;

                        switch((int)i+1-offset){
                        case 2 :
                            brokerId1 = row[i].substr(0,4);
//                            cout << row[i] << endl;
                            break;
                        case 3 :
                            price1 = row[i];
                            break;
                        case 4 :
                            temp = row[i];
                            temp.erase (remove(temp.begin(), temp.end(), ','), temp.end());
                            temp.erase (remove(temp.begin(), temp.end(), ' '), temp.end());
                            buyVol1 = temp;
                            break;
                        case 5 :
                            temp = row[i];
                            temp.erase (remove(temp.begin(), temp.end(), ','), temp.end());
                            temp.erase (remove(temp.begin(), temp.end(), ' '), temp.end());
                            sellVol1 = temp;
                            break;
                        case 7 :
                            brokerId2 = row[i].substr(0,4);
//                            cout << row[i] << endl;
                            break;
                        case 8 :
                            price2 = row[i];
                            break;
                        case 9 :
                            temp = row[i];
                            temp.erase (remove(temp.begin(), temp.end(), ','), temp.end());
                            temp.erase (remove(temp.begin(), temp.end(), ' '), temp.end());
                            buyVol2 = temp;
                            break;
                        case 10 :
                            temp = row[i];
                            temp.erase (remove(temp.begin(), temp.end(), ','), temp.end());
                            temp.erase (remove(temp.begin(), temp.end(), ' '), temp.end());
                            sellVol2 = temp;

                            break;
                         }
                }
                //if(whichcase ==0) cout<< "case 0: no print" << endl;
                if(whichcase ==1){/*cout<< "case 1:" << endl;*/
                Rawdata data = covert2RawData(counter,mCsvfile,brokerId1,price1,buyVol1,sellVol1);
                mRawdataList.push_back(data);
  //                  cout<< "brokerid:"<< brokerId1 << " price:" << price1 << " buy:" << buyVol1 << " sell:" << sellVol1 << endl;
//////                    insertDatabase(date,csvfile, category , brokerId1 , price1, buyVol1, sellVol1);
                }
                if(whichcase ==2) {/*cout << "case 2:" << endl;*/

                Rawdata data1 = covert2RawData(counter,mCsvfile,brokerId1,price1,buyVol1,sellVol1);
                Rawdata data2 = covert2RawData(counter+1,mCsvfile,brokerId2,price2,buyVol2,sellVol2);

                mRawdataList.push_back(data1);
                mRawdataList.push_back(data2);

  //              cout<< "brokerid:"<< brokerId1 << " price:" << price1 << " buy:" << buyVol1 << " sell:" << sellVol1 << endl;
  //              cout<< "brokerid:"<< brokerId2 << " price:" << price2 << " buy:" << buyVol2 << " sell:" << sellVol2 << endl;
/////                    insertDatabase(date,csvfile, category , brokerId1 , price1, buyVol1, sellVol1);
/////                    insertDatabase(date,csvfile, category , brokerId2 , price2, buyVol2, sellVol2);
                }
                if(whichcase==2)
                    counter+=2;
                else if(whichcase==1)
                    counter++;
                row_count++;
        }

/*
     for(size_t index=0;index<mRawdataList.size();index++){
    	 cout<< mRawdataList.at(index).serialno
    		 << ":"
    		 << mRawdataList.at(index).brokeid
    		 << " "
    		 << mRawdataList.at(index).price
    		 << " "
    		 << mRawdataList.at(index).buyvol
    		 << " "
    		 << mRawdataList.at(index).sellvol
    		 <<endl;
     }

     cout<<"total:" << counter << endl;
*/
}

Rawdata CsvFileReader::covert2RawData(int serialno,string stockid,string brokerId,string price,string buyvol,string sellvol){
	Rawdata data;

	try{
		data.serialno = serialno;
		data.stockid = stockid;
		data.brokeid = brokerId;
		data.price = boost::lexical_cast<double>(price);
		data.buyvol = boost::lexical_cast<int>(buyvol);
		data.sellvol = boost::lexical_cast<int>(sellvol);
	}catch(boost::bad_lexical_cast const&){
		cout<< "Error: input string was not valid!!" <<endl;
	}

	return data;
}
void CsvFileReader::SummeryRawdata(){
	size_t size = mRawdataList.size();

	for(size_t index = 0;index < size ;/*index++*/){
		Rawdata data = mRawdataList.at(index);
		Summery summery;
		string brokerid = summery.brokerid = data.brokeid;
		summery.stockid = data.stockid;

		summery.totalbuyvol = data.buyvol;
		summery.totalbuyprice = data.price * data.buyvol;
		summery.averagebuyprice = 0;

		summery.totalsellvol = data.sellvol;
		summery.totalsellprice = data.price * data.sellvol;
		summery.averagesellprice = 0;

		size_t samebrokerindex;
		for(samebrokerindex = index
			;++samebrokerindex <size && brokerid.compare(mRawdataList.at(samebrokerindex).brokeid)==0
			;){

			Rawdata data2 = mRawdataList.at(samebrokerindex);
			summery.totalbuyvol += data2.buyvol;
			summery.totalbuyprice += data2.buyvol * data2.price;
			summery.totalsellvol += data2.sellvol;
			summery.totalsellprice += data2.sellvol * data2.price;

		}
		if(summery.totalbuyvol!=0){
			summery.averagebuyprice = (float)summery.totalbuyprice/summery.totalbuyvol;
		}else{
			summery.averagebuyprice = 0;
		}

		if(summery.totalsellvol!=0){
			summery.averagesellprice = (float)summery.totalsellprice/summery.totalsellvol;
		}else{
			summery.averagesellprice=0;
		}

		index = samebrokerindex;
		mSummeryList.push_back(summery);
	}

/*    for(size_t index=0;index<mSummeryList.size();index++){
   	 cout<<"index:"
   	     << index
   	     << "    brokerid:"
   	 	 << mSummeryList.at(index).brokerid

   		 << "    totalbuyvol:"
   		 << mSummeryList.at(index).totalbuyvol
   		 << "    totalbuyprice:"
   		 << mSummeryList.at(index).totalbuyprice
   		 << "    averagebuyprice:"
   		 << mSummeryList.at(index).averagebuyprice

   		 << "    totalsellvol:"
   		 << mSummeryList.at(index).totalsellvol
   		 << "    totalsellprice:"
   		 << mSummeryList.at(index).totalsellprice
   		 << "    totalsellprice:"
   		 << mSummeryList.at(index).averagesellprice
   		 <<endl;
    }*/
}
