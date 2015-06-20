/*
 * Perceptron.h
 *
 *  Created on: May 15, 2015
 *      Author: neo
 */

#ifndef HASHINGPERCEPTRON_H_
#define HASHINGPERCEPTRON_H_

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <math.h>
#include <cmath>
#include "Parser.h"
using namespace std;
#define ITERATION_LIMIT 150

class HashingPerceptron{
public:
	HashingPerceptron(char* path, int token_size, float learning_rate, unsigned int hash_size, float init);
	~HashingPerceptron();
	vector<pair<string,float>> rate(char* path);
	map<string,pair<int,vector<string>>> aux_map;

private:
	int phrase_lenght;
	unsigned int hash_size;
	void Initialize();
	vector<float> weights;
	float learning_rate;
	float threshold;
	map<string,pair<int,vector<unsigned int>>> aux_map_2;
	float dot_product(vector<unsigned int> v);

	vector<pair<int,vector<unsigned int>>> test_vec;

	vector<unsigned int> getVector(vector<string> tokenized_words);

	void show_vec_str(vector<string> line);
	void show_vec_int(vector<int> line);
	void show_vec_flt(vector<float> line);

	float sigmoid(float x);

};

#endif /* HASHINGPERCEPTRON_H_ */
