/*
 * DatabaseHelper.h
 *
 *  Created on: Sep 21, 2013
 *      Author: tonyyou
 */

#ifndef DATABASEHELPER_H_
#define DATABASEHELPER_H_
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <exception.h>
#include <resultset.h>
#include <statement.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
using namespace std;
using namespace boost;
class DatabaseHelper {
    sql::Driver *mDriver;
    sql::Connection *mCon;
    sql::Statement *mStmt;
    sql::ResultSet *mRes;
public:
	DatabaseHelper();
	void insertToDb(string data,string stockid,string brokerid,float bprice,unsigned int bvol,float sprice,unsigned int svol);
	void createTableByDate(std::string data);
	void createJumpingTableByDate(std::string data);
	int getRowSize(std::string data);
	void insertIndexTable(std::string date,std::string stockid,unsigned int startindex,unsigned int length);
	virtual ~DatabaseHelper();
};

#endif /* DATABASEHELPER_H_ */
