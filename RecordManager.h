#ifndef __RECORD_H__
#define __RECORD_H__
#include "BufferManager.h"

extern BufferManager buf;
class RecordManager{
    Row splitRow(Table tableinfor, string row);
	string connectRow(Table tableinfor, Row splitedRow);
public:
	bool Comparator(Table tableinfor, Row row, vector<Condition> conditions);
	Data select(Table& tableinfor);
	Data select(Table tableinfor, vector<Condition> conditions);
	void insertValue(Table& tableinfor, Row& splitedRow);
	int deleteValue(Table tableinfor);
	int deleteValue(Table tableinfor, vector<Condition> conditions);
	void dropTable(Table tableinfo);
	void createTable(Table tableinfo);
    void showDatas(const Data& datas) const;
};
#endif






