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
//#include "Mapper.h"
//#include "AnalysisMethod.h"
//#include "SimpleCounter.h"
//#include "MultipleCounter.h"
//#include "LastLineCounter.h"
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
	//cout << "" << " | "<<l2[i]<< "\n";
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

	float init = atoi(args[3]);

	// Método M1 - Palabras individuales + puntaje + ocurrencia
	//SimpleCounter* counter = new SimpleCounter(learningPath);

    // Método M1.1 - Palabras en grupo (de 1, 2, o 3) + puntaje + ocurrencia
	//MultipleCounter* m_counter = new MultipleCounter(learningPath);

	// Metodo M2 - Palabras simples de la ultima oracion
	//LastLineCounter* ll_counter = new LastLineCounter(learningPath);

	// Metodo Perceptron
	unsigned int hash_size = pow(2,25);
	float learning_rate = 0.1 * log(2);
	HashingPerceptron* perceptron = new HashingPerceptron(learningPath,3, learning_rate ,hash_size,init);

	//Levanto y parseo el archivo a analizar
	Parser* parser = new Parser();

	ifstream testfile;
	testfile.open(testPath);

	vector<string> id_reviews;
	vector<float> scoresM1;
	vector<float> scoresM1_1;
	vector<float> scoresM2;
	vector<float> scoresP;
	//Utilizo para normalizar los resultados del perceptron
	float maxScore = 0;
	float minScore = 0;
	bool firstRun = true;
	printf("Comienzo a evaluar el set de prueba\n");
	if(testfile.is_open())
	{
		string linea;
		while( getline(testfile,linea) )
		{
			vector<string> tokenized_columns = parser->tokenizeColumns(linea);
			vector<string> tokenized_words = parser->tokenizeReviews(tokenized_columns[1]); //TODO cero porque solo son strings

			id_reviews.push_back(tokenized_columns[0]);

			// genero los resultados por cada método
			//float reviewScoreM1 = counter->rateReview(tokenized_words);
			//scoresM1.push_back(reviewScoreM1);

			//float reviewScoreM1_1 = m_counter->rateReview(tokenized_words);
			//scoresM1_1.push_back(reviewScoreM1_1);

			//float reviewScoreM2 = ll_counter->rateReview(tokenized_words);
			//scoresM2.push_back(reviewScoreM2);

			float score = perceptron->rate(tokenized_words);
			if (score < minScore || firstRun)
			{
				minScore = score;
			}
			if (score > maxScore || firstRun )
			{
				maxScore = score;
			}
			scoresP.push_back(score);
			firstRun = false;
		}
	}
	delete parser;
	//Normalizo los resultados
	/*
	for (unsigned int i=0; i < scoresP.size(); i++)
	{
			scoresP[i] = (scoresP[i]-minScore)/(maxScore - minScore);
	}
	*/
	//show(id_reviews, scoresM1, scoresM1_1, scoresM2);

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
