all:
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -Wno-write-strings -o Parser.o Parser.cpp  
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -Wno-write-strings -o HashingPerceptron.o HashingPerceptron.cpp
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -Wno-write-strings -o main.o main.cpp  
	g++ -o Datos main.o Parser.o HashingPerceptron.o 