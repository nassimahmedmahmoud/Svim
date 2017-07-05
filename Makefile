svim: Pass_hash.o 
	g++ -o svim Pass_hash.o
Pass_hash.o: Pass_hash.cpp
	g++ -Wall -g -c Pass_hash.cpp -o Pass_hash.o

