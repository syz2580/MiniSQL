#include "BufferManager.h"

BufferManager::BufferManager(){
    for (int i = 0; i< MAXBLOCKNUMBER; i++) bufferBlock[i].initialize();
}
BufferManager::~BufferManager(){
    for (int i = 0; i< MAXBLOCKNUMBER; i++)
    flashBack(i);
}
void BufferManager::flashBack(int bufferNum){
    if (!bufferBlock[bufferNum].isWritten) return;
    string filename = bufferBlock[bufferNum].filename;
    fstream fout(filename.c_str(), ios::in|ios::out);
    fout.seekp(BLOCKSIZE*bufferBlock[bufferNum].blockOffset, fout.beg);
    fout.write(bufferBlock[bufferNum].values, BLOCKSIZE);
    bufferBlock[bufferNum].initialize();
    fout.close();
}
int BufferManager::getbufferNum(string filename, int blockOffset){
    int bufferNum = getIfIsInBuffer(filename, blockOffset);
    if(bufferNum == -1){
        bufferNum = getEmptyBufferExcept(filename);
        readBlock(filename, blockOffset, bufferNum);
    }
    return bufferNum;
}

void BufferManager::readBlock(string filename, int blockOffset, int bufferNum){
    bufferBlock[bufferNum].isValid = 1;
    bufferBlock[bufferNum].isWritten = 0;
    bufferBlock[bufferNum].filename = filename;
    bufferBlock[bufferNum].blockOffset = blockOffset;
    fstream  fin(filename.c_str(), ios::in);
    fin.seekp(BLOCKSIZE*blockOffset, fin.beg);
    fin.read(bufferBlock[bufferNum].values, BLOCKSIZE);
    fin.close();
}
	
void BufferManager::writeBlock(int bufferNum){
    bufferBlock[bufferNum].isWritten = 1;
    useBlock(bufferNum);
}
	
void BufferManager::useBlock(int bufferNum){//this LRU algorithm is quite expensive
    for(int i = 1; i < MAXBLOCKNUMBER; i++)
    {
        if(i == bufferNum ){
            bufferBlock[bufferNum].LRUvalue = 0;
            bufferBlock[i].isValid = 1;
        }
        else bufferBlock[bufferNum].LRUvalue++;	//The greater is LRUvalue, the less useful the block is
    }
}
	
int BufferManager::getEmptyBuffer(){
    int bufferNum = 0;
    int highestLRUvalue = bufferBlock[0].LRUvalue;
    for(int i = 0; i < MAXBLOCKNUMBER; i++)
    {
        if(!bufferBlock[i].isValid){
            bufferBlock[i].initialize();
            bufferBlock[i].isValid = 1;
            return i;
        }
        else if(highestLRUvalue < bufferBlock[i].LRUvalue)
        {
            highestLRUvalue = bufferBlock[i].LRUvalue;
            bufferNum = i;
        }
    }
    flashBack(bufferNum);
    bufferBlock[bufferNum].isValid = 1;
    return bufferNum;
}
	
int BufferManager::getEmptyBufferExcept(string filename){	//buffer with the destine filename is not suppose to be flashback
    int bufferNum = -1;
    int highestLRUvalue = bufferBlock[0].LRUvalue;
    for(int i = 0; i < MAXBLOCKNUMBER; i++)
    {
        if(!bufferBlock[i].isValid){
            bufferBlock[i].isValid = 1;
            return i;
        }
        else if(highestLRUvalue < bufferBlock[i].LRUvalue && bufferBlock[i].filename != filename)
        {
            highestLRUvalue = bufferBlock[i].LRUvalue;
            bufferNum = i;
        }
    }
    if(bufferNum == -1){
        cout << "All the buffers in the database system are used up. Sorry about that!" << endl;
        exit(0);
    }
    flashBack(bufferNum);
    bufferBlock[bufferNum].isValid = 1;
    return bufferNum;
}

insertPos BufferManager::getInsertPosition(Table& tableinfor){//To increase efficiency, we always insert values from the back of the file
    insertPos ipos;
    if( tableinfor.blockNum == 0){//the *.table file is empty and the data is firstly inserted
        ipos.bufferNUM = addBlockInFile(tableinfor);
        ipos.position = 0;
        return ipos;
    }
    string filename = tableinfor.name + ".table";
    int length = tableinfor.totalLength + 1;
    int blockOffset = tableinfor.blockNum - 1;//get the block offset in file of the last block
    int bufferNum = getIfIsInBuffer(filename, blockOffset);
    if(bufferNum == -1){//indicate that the data is not read in buffer yet
        bufferNum = getEmptyBuffer();
        readBlock(filename, blockOffset, bufferNum);
    }
    const int recordNum = BLOCKSIZE / length;//¼Ó¶àÒ»Î»À´ÅÐ¶ÏÕâÌõ¼ÇÂ¼ÊÇ·ñ±»É¾³ýÁË
    for(int offset = 0; offset < recordNum; offset ++){
        int position  = offset * length;
        char isEmpty = bufferBlock[bufferNum].values[position];
        if(isEmpty == EMPTY){//find an empty space
            ipos.bufferNUM = bufferNum;
            ipos.position = position;
            return ipos;
        }
    }
    //if the program run till here, the last block is full, therefor one more block is added
    ipos.bufferNUM = addBlockInFile(tableinfor);
    ipos.position = 0;
    return ipos;
}

int BufferManager::addBlockInFile(Table& tableinfor){//add one more block in file for the table
    int bufferNum = getEmptyBuffer();	
    bufferBlock[bufferNum].initialize();
    bufferBlock[bufferNum].isValid = 1;
    bufferBlock[bufferNum].isWritten = 1;
    bufferBlock[bufferNum].filename = tableinfor.name + ".table";
    bufferBlock[bufferNum].blockOffset = tableinfor.blockNum++;
    return bufferNum;
}
	
int BufferManager::addBlockInFile(Index& indexinfor){//add one more block in file for the index
    string filename = indexinfor.index_name + ".index";
    int bufferNum = getEmptyBufferExcept(filename);
    bufferBlock[bufferNum].initialize();
    bufferBlock[bufferNum].isValid = 1;
    bufferBlock[bufferNum].isWritten = 1;
    bufferBlock[bufferNum].filename = filename;
    bufferBlock[bufferNum].blockOffset = indexinfor.blockNum++;
    return bufferNum;
}

int BufferManager::getIfIsInBuffer(string filename, int blockOffset){
    for(int bufferNum = 0; bufferNum < MAXBLOCKNUMBER; bufferNum++)
    if(bufferBlock[bufferNum].filename == filename && bufferBlock[bufferNum].blockOffset == blockOffset)	return bufferNum;
    return -1;	//indicate that the data is not read in buffer yet
}
	
void BufferManager::scanIn(Table tableinfo){//this is dangerous because
    string filename = tableinfo.name + ".table";
    fstream  fin(filename.c_str(), ios::in);
    for(int blockOffset = 0; blockOffset < tableinfo.blockNum; blockOffset++){
        if(getIfIsInBuffer(filename, blockOffset) == -1){	//indicate that the data is not read in buffer yet
            int bufferNum = getEmptyBufferExcept(filename);
            readBlock(filename, blockOffset, bufferNum);
        }
    }
    fin.close();
}
	
void BufferManager::setInvalid(string filename){//when a file is deleted, a table or an index, all the value in buffer should be set invalid
    for(int i = 0; i < MAXBLOCKNUMBER; i++)
    {
        if(bufferBlock[i].filename == filename){
            bufferBlock[i].isValid = 0;
            bufferBlock[i].isWritten = 0;
        }
    }
}
/*This function can show the values in the buffer, which is for debug only
  Take care when using. If the size of buffer is very big, your computer may 
  crash down*/
void BufferManager::ShowBuffer(int from, int to){
    const int max = 30;
    if(!(0 <= from && from <= to && to < MAXBLOCKNUMBER)){
        cout << "No such buffer!" <<endl;
        return;
    }
    if((to-from) > max){
        cout << max <<" buffers at most!" << endl;
        return; 
    }
    for(int i = from; i <= to; i++){
        ShowBuffer(i);
    }
}
void BufferManager::ShowBuffer(int bufferNum){
    cout << "BlockNum: " << bufferNum << endl;
    cout << "IsWritten: " << bufferBlock[bufferNum].isWritten << endl;
    cout << "IsValid: " << bufferBlock[bufferNum].isValid << endl;
    cout << "Filename: " << bufferBlock[bufferNum].filename << endl;
    cout << "blockOffset: " << bufferBlock[bufferNum].blockOffset << endl;
    cout << bufferBlock[bufferNum].values <<endl;
}