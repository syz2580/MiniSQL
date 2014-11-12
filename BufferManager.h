#ifndef __BUFFER_H__
#define __BUFFER_H__
#include "DS.h"

class BufferManager{
public:
	friend class RecordManager;
	friend class IndexManager;
	friend class Leaf;
	friend class Branch;
	friend class BPlusTree;
	BufferManager();
	~BufferManager();
private:
public:
	buffer bufferBlock[MAXBLOCKNUMBER];
	void flashBack(int bufferNum);
	int getbufferNum(string filename, int blockOffset);
	void readBlock(string filename, int blockOffset, int bufferNum);
	void writeBlock(int bufferNum);
	void useBlock(int bufferNum);//this LRU algorithm is quite expensive
    int getEmptyBuffer();
	int getEmptyBufferExcept(string filename);
	insertPos getInsertPosition(Table& tableinfor);
	int addBlockInFile(Table& tableinfor);//add one more block in file for the table
	int addBlockInFile(Index& indexinfor);//add one more block in file for the index
	int getIfIsInBuffer(string filename, int blockOffset);
	void scanIn(Table tableinfo);//this is dangerous because
	void setInvalid(string filename);//when a file is deleted, a table or an index, all the value in buffer should be set invalid
public:
	/*This function can show the values in the buffer, which is for debug only
	Take care when using. If the size of buffer is very big, your computer may 
	crash down*/
	void ShowBuffer(int from, int to);
	void ShowBuffer(int bufferNum);
};

#endif
