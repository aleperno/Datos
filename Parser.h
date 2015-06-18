/*
 * Parser.h
 *
 *  Created on: May 8, 2015
 *      Author: ana
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

using namespace std;

class Parser {
public:
	Parser();
	virtual ~Parser();

	std::string columnDelimeter;
	std::string wordDelimeters;
	vector<char> swearingChars;


	vector<string> tokenizeColumns(string line);
	vector<string> tokenizeReviews(string line);
	string lastLine(string line);
	string noSmileys(string line);
	vector<string> noSwearing(vector<string> tokens);

private:
	void loadSwearingChars(char* filename);
	void loadColumnDelimiters();
	void loadWordDelimeters();

	string noFrontSpaces(string line);

	vector<string> tokenize(string line, string newDelimiters);
	bool isSwearing(string word, int minChars);

//TODO fix this
	bool isFractionScore(string s);
	float fractionScore(string s);

};

#endif /* PARSER_H_ */
