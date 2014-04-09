/*
 * DatabaseHelper.cpp
 *
 *  Created on: Sep 21, 2013
 *      Author: tonyyou
 */

#include "DatabaseHelper.h"

DatabaseHelper::DatabaseHelper() {
    mCon = NULL;
    mStmt = NULL;
    mRes = NULL;

    mDriver = get_driver_instance();
    mCon = mDriver->connect("tcp://localhost:3306","stockwriter","stockwriter123");
    mStmt = mCon->createStatement();
}

void DatabaseHelper::insertToDb(string data,string stockid,string brokerid,float bprice,unsigned int bvol,float sprice,unsigned int svol){
	string sqlcommand = "";
	string strbprice = boost::lexical_cast<string>(bprice);
	string strbvol = boost::lexical_cast<string>(bvol);
	string strsprice = boost::lexical_cast<string>(sprice);
	string strsvol = boost::lexical_cast<string>(svol);
	sqlcommand = "INSERT INTO `stock`.`stk_"+data+"` (`TradeId`, `StockId`, `BrokerId`, `BPrice`, `BVolume`, `SPrice`, `SVolume`) VALUES (NULL, '"+
			stockid+"', '"+brokerid+"', '"+strbprice+"', '"+strbvol+"', '"+strsprice+"', '"+strsvol+"');";

//	cout << sqlcommand << endl;
	try{
		mStmt->execute("USE stock");
		mStmt->execute(sqlcommand);
	}catch(sql::SQLException e){
		cout << "SQL error. Error message: " << e.what() << endl;
	}
}

int DatabaseHelper::getRowSize(std::string date){
	int count = 0;
	mStmt->execute("USE stock");
	mRes = mStmt->executeQuery("SELECT COUNT(*) FROM `stock`.`stk_"+date+";");
	if(mRes->next()){
		count = mRes->getInt(1);
		cout<< count <<endl;
		return count;
	}
	return 0;
}

void DatabaseHelper::insertIndexTable(std::string date,std::string stockid,unsigned int startindex,unsigned int length){
	string sqlcommand = "";
	string strstartindex = boost::lexical_cast<string>(startindex);
	string strlength = boost::lexical_cast<string>(length);
	sqlcommand = "INSERT INTO `stock`.`stk_"+date+"_idx` (`Id`, `StockId`, `StartIndex`, `Length`) VALUES (NULL, '"+
			stockid+"', '"+strstartindex+"', '"+strlength+"');";
	cout << stockid<<" " << startindex <<" "<< length<<endl;
	try{
		mStmt->execute("USE stock");
		mStmt->execute(sqlcommand);
	}catch(sql::SQLException e){
		cout << "SQL error. Error message: " << e.what() << endl;
	}

}

void DatabaseHelper::createTableByDate(std::string data){
	string sqlcommand = "CREATE TABLE IF NOT EXISTS `stk_" + data +"` ("
			+ "`TradeId` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY, "
			+ "`StockId` CHAR(10), "
			+ "`BrokerId` CHAR(4), "
			+ "`BPrice` DECIMAL(5,2), "
			+ "`BVolume` INT UNSIGNED, "
			+ "`SPrice` DECIMAL(5,2), "
			+ "`SVolume` INT UNSIGNED"
			+ " ) ENGINE = MYISAM CHARACTER SET utf8 COLLATE utf8_unicode_ci;";

	try{
		mStmt->execute("USE stock");
		mStmt->execute(sqlcommand);
	}catch(sql::SQLException e){
		cout << "SQL error. Error message: " << e.what() << endl;
	}
}
void DatabaseHelper::createJumpingTableByDate(std::string data){
	string sqlcommand = "CREATE TABLE IF NOT EXISTS `stk_" + data +"_idx" +"` ("
			+ "`Id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY, "
			+ "`StockId` CHAR(10), "
			+ "`StartIndex` INT UNSIGNED, "
			+ "`Length` INT UNSIGNED"
			+ " ) ENGINE = MYISAM CHARACTER SET utf8 COLLATE utf8_unicode_ci;";

	try{
		mStmt->execute("USE stock");
		mStmt->execute(sqlcommand);
	}catch(sql::SQLException e){
		cout << "SQL error. Error message: " << e.what() << endl;
	}
}

DatabaseHelper::~DatabaseHelper() {
    if(mRes!=NULL)
    delete mRes;
    if(mStmt!=NULL)
    delete mStmt;
    if(mCon!=NULL)
    delete mCon;
}

