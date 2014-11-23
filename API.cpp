#include "Interpreter.h"

//enum Operation{CREATABLE, INSERT, SELECT, DELECT, DROPINDEX, CRETEINDEX, DROPTABLE, ERROR};
//Interpreter interprt; 
RecordManager record;
IndexManager indexm;
CatalogManager catalog;
Interpreter parsetree;
BufferManager buf;

void welcome();
void ShowResult(Data data, Table tableinfor, vector<Attribute> column);
void Execute();
void AddSeperator(char *command);
short int IsComEnd(char *input);



int main()
{
	welcome();
	vector<Condition> conditions;
	Table tableinfor;
	Index indexinfor;
	Row insertValue;
	Data datas;
	char command[COMLEN] = "";
	char input[INPUTLEN] = "";
	char word[WORDLEN] = "";
	short int ComEnd = 0;
	FILE *stream;
	stream=freopen("D:\\test.txt","r",stdin);
	/*FILE *fileout;
	fileout=freopen("result.txt","w",stdout);*/

	//int count = 0;
	while(1)
	{
		strcpy(command, "");//command����
		ComEnd = 0;
		cout<<" Adward's Database >>";
 		while(!ComEnd)
		{
	/*		count ++;
			if(count % 100 == 0)
				cout << count/100 << "%" << endl;*/
			gets(input);
			if(IsComEnd(input))
				ComEnd = 1;
				strcat(command, input);
			AddSeperator(command);
		}
		parsetree.Parse(command);
		Execute();
	}
	getchar();
	return 0;
}

void welcome(){
	cout << "----------Welcome to MiniSQL----------" << endl;
}

void ShowResult(Data data, Table tableinfor, vector<Attribute> column){
	if(column[0].name == "*"){
		cout << endl <<"+";
		for(int i = 0; i < tableinfor.attriNum; i++){
			for(int j = 0; j < tableinfor.attributes[i].length + 1; j++){
				cout << "-";
			}
			cout << "+";
		}
		cout << endl;
		cout << "| ";
		for(int i = 0; i < tableinfor.attriNum; i++){
			cout << tableinfor.attributes[i].name;
			int lengthLeft = tableinfor.attributes[i].length - tableinfor.attributes[i].name.length();
			for(int j = 0; j < lengthLeft; j++){
				cout << ' ';
			}
			cout << "| ";
		}
		cout << endl;
		cout << "+";
		for(int i = 0; i < tableinfor.attriNum; i++){
			for(int j = 0; j < tableinfor.attributes[i].length + 1; j++){
				cout << "-";
			}
			cout << "+";
		}
		cout << endl;

		//����
		for(int i = 0; i < data.rows.size(); i++){
			cout << "| ";
			for(int j = 0; j < tableinfor.attriNum; j++){
				int lengthLeft = tableinfor.attributes[j].length;
				for(int k =0; k < data.rows[i].columns[j].length(); k++){
					if(data.rows[i].columns[j].c_str()[k] == EMPTY) break;
					else{
						cout << data.rows[i].columns[j].c_str()[k];
						lengthLeft--;
					}
				}
				for(int k = 0; k < lengthLeft; k++) cout << " ";
				cout << "| ";
			}
			cout << endl;
		}

		cout << "+";
		for(int i = 0; i < tableinfor.attriNum; i++){
			for(int j = 0; j < tableinfor.attributes[i].length + 1; j++){
				cout << "-";
			}
			cout << "+";
		}
		cout << endl;
	}
	else{
		cout << endl <<"+";
		for(int i = 0; i < column.size(); i++){
			int col;
			for(col = 0; col < tableinfor.attributes.size(); col++){
				if(tableinfor.attributes[col].name == column[i].name) break;
			}
			for(int j = 0; j < tableinfor.attributes[col].length + 1; j++){
				cout << "-";
			}
			cout << "+";
		}
		cout << endl;
		cout << "| ";
		for(int i = 0; i < column.size(); i++){
			int col;
			for(col = 0; col < tableinfor.attributes.size(); col++){
				if(tableinfor.attributes[col].name == column[i].name) break;
			}
			cout << tableinfor.attributes[col].name;
			int lengthLeft = tableinfor.attributes[col].length - tableinfor.attributes[col].name.length();
			for(int j = 0; j < lengthLeft; j++){
				cout << ' ';
			}
			cout << "| ";
		}
		cout << endl;
		cout << "+";
		for(int i = 0; i < column.size(); i++){
			int col;
			for(col = 0; col < tableinfor.attributes.size(); col++){
				if(tableinfor.attributes[col].name == column[i].name) break;
			}
			for(int j = 0; j < tableinfor.attributes[col].length + 1; j++){
				cout << "-";
			}
			cout << "+";
		}
		cout << endl;

		//����
		for(int i = 0; i < data.rows.size(); i++){
			cout << "| ";
			for(int j = 0; j < column.size(); j++){
				int col;
				for(col = 0; col < tableinfor.attributes.size(); col++){
					if(tableinfor.attributes[col].name == column[j].name) break;
				}
				int lengthLeft = tableinfor.attributes[col].length;
				for(int k =0; k < data.rows[i].columns[col].length(); k++){
					if(data.rows[i].columns[col].c_str()[k] == EMPTY) break;
					else{
						cout << data.rows[i].columns[col].c_str()[k];
						lengthLeft--;
					}
				}
				for(int k = 0; k < lengthLeft; k++) cout << " ";
				cout << "| ";
			}
			cout << endl;
		}

		cout << "+";
		for(int i = 0; i < column.size(); i++){
			int col;
			for(col = 0; col < tableinfor.attributes.size(); col++){
				if(tableinfor.attributes[col].name == column[i].name) break;
			}
			for(int j = 0; j < tableinfor.attributes[col].length + 1; j++){
				cout << "-";
			}
			cout << "+";
		}
		cout << endl;
	}
	cout << data.rows.size() << " rows have been found."<< endl;
}
void Execute()
{	
	int i;
	int j;
	int k;
	Table tableinfor;
	Index indexinfor;
	string tempKeyValue;
	int tempPrimaryPosition=-1;
	int rowCount=0;
	Data data;
	switch(parsetree.m_operation)
	{
	case CRETAB:
		parsetree.getTableInfo.attriNum=parsetree.getTableInfo.attributes.size();
		catalog.createTable(parsetree.getTableInfo);
		record.createTable(parsetree.getTableInfo);
		cout<<"Table "<<parsetree.getTableInfo.name<<" is created successfully"<<endl;
		break;
	case TABLEEXISTED:
		cout<<"CREATE ERROR: Table existed"<<endl;
		break;
	case DRPTAB:
		record.dropTable(parsetree.getTableInfo);
		for(int i = 0; i < parsetree.getTableInfo.attriNum; i++){//���������е�index��ɾ��
			indexinfor = catalog.getIndexInformation(parsetree.getTableInfo.name, i);
			if(indexinfor.index_name != "")
				indexm.dropIndex(indexinfor);
		}
		catalog.dropTable(parsetree.getTableInfo);
		cout<<"Table "<<parsetree.getTableInfo.name<<" is dropped successfully"<<endl;
		break;
	case INSERT:
		tableinfor = parsetree.getTableInfo;
		if(parsetree.PrimaryKeyPosition==-1&&parsetree.UniquePostion==-1){
			record.insertValue(tableinfor, parsetree.row);
			catalog.update(tableinfor);
			cout<<"Insert successfully"<<endl;
			break;
		}
		if(parsetree.PrimaryKeyPosition!=-1)
		{
			data=record.select(tableinfor, parsetree.condition);
			if(data.rows.size()>0){
				cout<<"INSERT ERROR: Primary key redundancy"<<endl;
				break;
			}
		}
		if(parsetree.UniquePostion!=-1){
			
			data=record.select(tableinfor, parsetree.UniqueCondition);
			if(data.rows.size()>0){
				cout<<"INSERT ERROR: Unique value redundancy"<<endl;
				break;
			}
		}
		record.insertValue(tableinfor,parsetree.row);
		catalog.update(tableinfor);
		cout<<"Insert successfully"<<endl;
		break;
	case INSERTERR:
		cout << "Syntax ERROR: Incorrect usage of \"insert\"." << endl;
		break;
	case SELECT_NOWHERE_CAULSE:
		tableinfor = parsetree.getTableInfo;
		data=record.select(tableinfor);
		if(data.rows.size()!=0)
			ShowResult( data, tableinfor, parsetree.column);
		else{
			cout << "No data is found." << endl;
		}
		break;
	case SELECT_WHERE_CAULSE:
		tableinfor=parsetree.getTableInfo;
		if(parsetree.condition.size()==1){
			for(int i=0;i<parsetree.getTableInfo.attributes.size();i++){
		/*�޸�*/if((parsetree.getTableInfo.attributes[i].isPrimeryKey==true||parsetree.getTableInfo.attributes[i].isUnique==true)&&parsetree.m_colname==parsetree.getTableInfo.attributes[i].name){
					tempPrimaryPosition=i;
					indexinfor=catalog.getIndexInformation(tableinfor.name,i);
					break;
				}
			}
			if(tempPrimaryPosition==parsetree.condition[0].columnNum&&parsetree.condition[0].op==Eq&&indexinfor.table_name!=""){
				
				tempKeyValue=parsetree.condition[0].value;
				data= indexm.selectEqual(tableinfor,indexinfor,tempKeyValue);
			}
			else{

				data=record.select(tableinfor,parsetree.condition);
			}
		}
		else{
			data=record.select(tableinfor,parsetree.condition);
		}
		if(data.rows.size()!=0)
			ShowResult( data, tableinfor, parsetree.column);
		else{
			cout << "No data is found." << endl;
		}
		break;
	case DELETE:
		rowCount = record.deleteValue(parsetree.getTableInfo,parsetree.condition);
		cout<< rowCount <<"  tuples are deleted."<<endl;
		break;
	case CREIND:
		indexinfor = parsetree.getIndexInfo;
		tableinfor = parsetree.getTableInfo;
		if(!tableinfor.attributes[indexinfor.column].isPrimeryKey && !tableinfor.attributes[indexinfor.column].isUnique){//����primary key�������Խ�index
			cout << "Column " << tableinfor.attributes[indexinfor.column].name <<"  is not unique."<< endl;
			break;
		}
		catalog.createIndex(indexinfor);
		indexm.createIndex(tableinfor, indexinfor);
		catalog.update(indexinfor);
		cout<<"Index "<< indexinfor.index_name << "is created successfully."<<endl;
		break;
	case INDEXERROR:
		cout<<"ERROR: Index existed."<<endl;
		break;
	case DRPIND:
		indexinfor = catalog.getIndexInformation(parsetree.m_indname);
		if(indexinfor.index_name == ""){
			cout << "ERROR: Index" << parsetree.m_indname << "does not exist!" << endl;
		}
		indexm.dropIndex(indexinfor);
		catalog.dropIndex(parsetree.m_indname);
		cout<<"The index is dropped successfully"<<endl;
		break;
	case CREINDERR:
		cout << "Syntax ERROR: Incorrect usage of \"create index\" query." << endl;
		break;
	case QUIT:
		cout << "Bye Bye~" << endl;
		system("pause");
		exit(0);
		break;
	case EMPTY:
		cout << "Query Empty." << endl;
		break;
	case UNKNOW:
		cout << "Syntax ERROR: Please check your query." << endl;
		break;
	case SELERR:
		cout << "Syntax ERROR: Incorrect usage of \"select\" query." << endl;
		break;
	case CRETABERR:
		cout << "Syntax ERROR: Incorrect usage of \"create table\" query." << endl;
		break;
	case DELETEERR:
		cout << "Syntax ERROR: Incorrect usage of \"delete from\" query." << endl;
		break;
	case DRPTABERR:
		cout << "Syntax ERROR: Incorrect usage of \"drop table\" query." << endl;
		break;
	case DRPINDERR:
		cout << "Syntax ERROR: Incorrect usage of \"drop index\" query." << endl;
		break;
	case VOIDPRI:
		cout << "ERROR: Invalid primary key." << endl;
		break;
	case VOIDUNI:
		cout << "ERROR: Invalid unique key." << endl;
		break;
	case CHARBOUD:
		cout << "ERROR: Too long query. Only 1~255 charactors is allowed." << endl;
		break;
	case NOPRIKEY:
		cout << "ERROR: Please define a primary key." << endl;
		break;
	case TABLEERROR:
		cout << "ERROR: Table is not existed."<<endl;
		break;
	case INDEXEROR:
		cout << "ERROR: Index is not existed."<<endl;
		break;
	case COLUMNERROR:
		cout << "ERROR: Column is not existed"<<endl;
		break;
	case INSERTNUMBERERROR:
		cout << "ERROR: The amount of the columns is not matched."<<endl;
		break;
	}
	
}
void AddSeperator(char *command)
{
	unsigned len = strlen(command);
	command[len] = ' ';
	command[len + 1] = '\0';
}

short int IsComEnd(char *input)
{
	unsigned int next = strlen(input) - 1;
	char prev = ' ';
	while(prev == '\t' || prev ==' ')
	{
		prev = input[next];
		next --;
	}
	if(prev == ';')
	{
		input[next + 1] ='\0';
		return 1;
	}
	return 0;
}
