/*
 * main.cpp
 *
 *  Created on: May 7, 2015
 *      Author: ana
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <boost/foreach.hpp>
#include <map>
#include <math.h>
#include <cstdlib>

#include "Parser.h"
#include "HashingPerceptron.h"

using namespace std;


void show(vector<string> line) {
	for (unsigned int i=0; i<line.size(); i++) {
		cout << line[i] <<endl;
	}
}

void show(vector<char> line) {
	for (unsigned int i=0; i<line.size(); i++) {
		cout << line[i] <<endl;
	}
}

void show(vector<int> line) {
	for (unsigned int i=0; i<line.size(); i++) {
		cout << line[i] << ",";
	}
	cout << endl;
}

void show(vector<float> line) {
	for (unsigned int i=0; i<line.size(); i++) {
		cout << line[i] << "\n";
	}
	cout << endl;
}

void show(vector<string> l0, vector<float> l1, vector<float> l2, vector<float> l3) {
	if ((l1.size()!=l2.size()) && (l1.size()!=l3.size()))
		return;
	for (unsigned int i=0; i<l1.size(); i++) {
		cout << l0[i] << "\t| " << l1[i] << "\t| "<<l2[i]<< "\t| "<<l3[i]<< "\n";
	}
	cout << endl;
}


int main( int argc, char* args[] ) {

	//Se le pasará como argumento el archivo labeledTrainData.tsv (preparseado).
	char* learningPath = args[1];

	// path de análisis
	char* testPath = args[2];

	//El valor con el que se inicializara los pesos del perceptron
	float init = atoi(args[3]);

	// Metodo Perceptron
	unsigned int hash_size = pow(2,25);
	float learning_rate = 0.1 * log(2);
	HashingPerceptron* perceptron = new HashingPerceptron(learningPath,3, learning_rate ,hash_size,init);

	//Levanto y parseo el archivo a analizar
	Parser* parser = new Parser();

	ifstream testfile;
	testfile.open(testPath);

	vector<string> id_reviews;
	vector<float> scoresP;
	//Utilizo para normalizar los resultados del perceptron
	printf("Comienzo a evaluar el set de prueba\n");
	if(testfile.is_open())
	{
		string linea;
		while( getline(testfile,linea) )
		{
			vector<string> tokenized_columns = parser->tokenizeColumns(linea);
			vector<string> tokenized_words = parser->tokenizeReviews(tokenized_columns[1]); //TODO cero porque solo son strings

			id_reviews.push_back(tokenized_columns[0]);

			float score = perceptron->rate(tokenized_words);
			scoresP.push_back(score);
		}
	}
	delete parser;

	//Imprimo los resultados
	ofstream outfile("popcorn.csv");
	outfile << "\"id\",\"sentiment\"" << endl;

	for (unsigned int i=0; i < id_reviews.size(); i++)
	{
		outfile << id_reviews[i]+"," << scoresP[i]<<endl;
	}
	outfile.close();

	return 0;
}
