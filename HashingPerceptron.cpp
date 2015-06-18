/*
 * Perceptron.cpp
 *
 *  Created on: May 15, 2015
 *      Author: neo
 */

#include "HashingPerceptron.h"
#include <boost/foreach.hpp>
#include <ctime>
#include <time.h>

void HashingPerceptron::show_vec_str(vector<string> line) {
	for (unsigned int i=0; i<line.size(); i++) {
		cout << line[i] << ",";
	}
}

void HashingPerceptron::show_vec_int(vector<int> line) {
	for (unsigned int i=0; i<line.size(); i++) {
		cout << line[i] << "," ;
	}
}

void HashingPerceptron::show_vec_flt(vector<float> line) {
	for (unsigned int i=0; i<line.size(); i++) {
		cout << line[i] << "," ;
	}
}

HashingPerceptron::HashingPerceptron(char* path, int token_size, float learning_rate, unsigned int hash_size, float init) {

	ifstream myfile;
	myfile.open(path);

	Parser* parser = new Parser();

	//NO USO MAPPER
	//this->mapper = NULL;
	//El largo de la frase debe estar aca
	this->phrase_lenght = token_size;
	//Largo del hash
	this->hash_size = hash_size;
	clock_t inicio = clock();
	if(myfile.is_open())
	{
		printf("Comienzo a representar los reviews como vectores con hashing trick\n");

		string linea;
		while( getline(myfile,linea) )
		{
			vector<string> tokenized_columns = parser->tokenizeColumns(linea);
			string movie_id = tokenized_columns[0];
			//this->strToInt(tokenized_columns[1]);
			int sentiment = (tokenized_columns[1] == "0") ? 0:1;
			string review = tokenized_columns[2];

			vector<string> tokenized_words = parser->tokenizeReviews(review);
			//this->show_vec_str(tokenized_words);
			pair<int,vector<unsigned int>> par_aux;
			vector<unsigned int> vec_pos = this->getVector(tokenized_words);
			par_aux = make_pair(sentiment,vec_pos);
			this->test_vec.push_back(par_aux);
			//movie_id -> sentiment, [words]
			this->aux_map_2[movie_id] = par_aux;

		}
	}
	clock_t final = clock();
	double secs = double(final - inicio) / CLOCKS_PER_SEC;
	float min = floor(secs/60);
	float sec = secs - min*60;
	printf("Se tardo %0.0f:%.3f\n",min,sec);
	delete parser;
	this->learning_rate = learning_rate;
	this->threshold = 0.5;

	this->weights = vector<float>(this->hash_size,init);
	cout << "Los pesos se inicalizaron en " << this->weights[0] << endl;
	this->Initialize();
}

float HashingPerceptron::dot_product(vector<unsigned int> vec)
{
	float count = 0;
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		count += this->weights[vec[i]];
	}
	return count;
}

vector<unsigned int> HashingPerceptron::getVector(vector<string> tokenized_words)
{
	vector<unsigned int> vec_pos;
	vector<string> phrase_vec;
	//Primero debo generar las frases
	for(unsigned int i=0; i < tokenized_words.size(); i++)
	{
		string str = tokenized_words[i];
		phrase_vec.push_back(str);

		for (int j=1; j< this->phrase_lenght; j++)
		{
			if (i+j < tokenized_words.size())
			{
				if (tokenized_words[i+j] != "")
				{
					str.append(" ");
					str.append(tokenized_words[i+j]);
					phrase_vec.push_back(str);
				}
			}
		}
	}
	hash<string> hash_fn;
	for(unsigned int i=0; i < phrase_vec.size(); i++)
	{
		unsigned int pos = hash_fn(phrase_vec[i])%this->hash_size;
		vec_pos.push_back(pos);
	}
	return vec_pos;
}

void HashingPerceptron::Initialize()
{

	cout << "Comienzo a balancear los pesos del perceptron" << endl;
	printf("\nIteracion \t Errores \t Promedio \t Tiempo \n");
	int iter_count = 1;
	int rev_counter = 0;
	clock_t inicio = clock();
	int min_error = -1;
	int min_count = 0;
	vector<float> aux_weights;
	while (true)
	{
		int error_count = 0;
		rev_counter = 0;
		pair<int,vector<unsigned int>> elem;
		BOOST_FOREACH(elem,this->test_vec)
		{
			rev_counter++;
			float aux = dot_product(elem.second);
			bool result = aux > this->threshold;
			//int error = elem.second.first - result;
			int error = elem.first - result;
			if (error != 0)
			{
				int pos;
				error_count++;
				BOOST_FOREACH(pos, elem.second)
				{
					this->weights[pos] += learning_rate * error;
				}
			}
		}
		//Imprimo los resultados parciales del balanceo
		double final = clock();
		double secs = double(final - inicio) / CLOCKS_PER_SEC;
		float min = floor(secs/60);
		float sec = secs - min*60;
		printf("%d\t\t%d\t\t%0.5f\t\t%.0f:%.3f\n",iter_count,error_count, 1.0 - (double)error_count/rev_counter,min,sec);
		fflush(stdout);
		//Veo si el error es el minimo, guardo el hiperplano
		if (error_count <= min_error || min_error == -1)
		{
			if (error_count == min_error)
				min_count++;
			else
				min_count = 0;
			min_error = error_count;
			aux_weights = this->weights;
		}
		//Controlo el corte
		if (iter_count == ITERATION_LIMIT)
		{
			cout << "Llegado al limite permitido de iteraciones" << endl;
			break;
		}
		if (error_count == 0) break;
		if (min_count == 5)
		{
			cout << "Se supero el limite de iteraciones con error minimo" << endl;
			break;
		}
		iter_count++;
	}
	cout << "Termino de balancear el perceptron" << endl;
	//Guardo el hiperplano de minimo error
	cout << "El error minimo fue de " << min_error << endl;
	this->weights = aux_weights;
}

float HashingPerceptron::rate(vector<string> review)
{
	vector<unsigned int> aux_vec = this->getVector(review);
	float result = this->dot_product(aux_vec);
	return this->sigmoid(result);
}

HashingPerceptron::~HashingPerceptron() {
	//TODO debo borrar algo?
}

float HashingPerceptron::sigmoid(float x)
{
	return 1 / (1 + exp(-x*0.01));
}
