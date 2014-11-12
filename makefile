objects = API.o Interpreter.o BufferManager.o \
	RecordManager.o IndexManager.o CatalogManager.o
sql : $(objects)
	g++ -o sql $(objects)
API.o : API.cpp Interpreter.h
	g++ -c API.cpp
Interpreter.o : Interpreter.cpp Interpreter.h
	g++ -c Interpreter.cpp
BufferManager.o : BufferManager.cpp BufferManager.h
	g++ -c BufferManager.cpp
RecordManager.o : RecordManager.cpp RecordManager.h
	g++ -c RecordManager.cpp
IndexManager.o : IndexManager.cpp IndexManager.h
	g++ -c IndexManager.cpp
CatalogManager.o : CatalogManager.cpp CatalogManager.h
	g++ -c CatalogManager.cpp
clean :
	rm sql $(objects)