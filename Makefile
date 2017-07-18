all: svim

svim: Pass_hash.o functions.o
	g++ -Wall o/Pass_hash.o o/functions.o -o svim -lcrypto -lboost_program_options 
	
Pass_hash.o:
	g++ -Wall -c cpp/Pass_hash.cpp -o o/Pass_hash.o
	
functions.o:
	g++ -Wall -c cpp/functions.cpp -o o/functions.o
clean:
	rm -f o/*
	rm -f svim
