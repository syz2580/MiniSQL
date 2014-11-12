#ifndef __CATALOG_h__
#define __CATALOG_h__
#include "DS.h"

class CatalogManager{
private:
	vector<Table> tables;
	int tableNum;          //the number of tables in the database, should be equels to tables.size()
	vector<Index> indexes;                	//索引列表
	int indexNum;                   	//索引数目
private:
	void InitialTalbeCatalog();
	void InitialIndexCatalog();
	void StoreTalbeCatalog();
	void StoreIndexCatalog();
public:
	CatalogManager();
    ~CatalogManager();
	void createTable(Table& table);
	void createIndex(Index index);
	void dropTable(Table table);
	void dropIndex(Index index);
	void dropTable(string tablename);
	void dropIndex(string index_name);
	void update(Table& tableinfor);
	void update(Index& index);
	bool ExistTable(string tablename);
	bool ExistIndex(string tablename, int column);
	bool ExistIndex(string indexname);
	Table getTableInformation(string tablename);
	Index getIndexInformation(string tablename, int column);
	Index getIndexInformation(string indexName);
	void ShowCatalog();
	void ShowTableCatalog();//this method is for debug only
	void ShowIndexCatalog();//this method is for debug also
    int GetColumnNumber(Table& tableinfo,string columnname);
    int GetColumnAmount(Table& tableinfo);
};
#endif
