/*
 * Parser.cpp
 *
 *  Created on: May 8, 2015
 *      Author: ana
 */

#include "Parser.h"

void Parser::loadSwearingChars(char* filename) {
	std::fstream f(filename);
	if(f.is_open()) {
		char c;
		while (!f.eof()) { // &&
			f.get(c);
			if (c != '\n')
				this->swearingChars.push_back(c);
		}
		f.close();
	}
}

void Parser::loadColumnDelimiters() {
	this->columnDelimeter = "\t";
}

void Parser::loadWordDelimeters() {
	this->wordDelimeters = " .,!?-/";
}

Parser::Parser() {
	//this->loadBadSmileys("bad-smileys.txt");
	//this->loadGoodSmileys("good-smileys.txt");
	//this->loadSwearingChars("swearing-chars.txt");
	this->loadColumnDelimiters();
	this->loadWordDelimeters();

}

Parser::~Parser() {
	// TODO Auto-generated destructor stub
}


string Parser::noFrontSpaces(string line) {
	string aux = line;
	while ((line[0] == '\n') || (line[0] == ' ')){
		aux.erase(0,1);
		line = aux;
	}
	return aux;
}

vector<string> Parser::tokenizeColumns(string line){
	vector<string> parts;
	boost::split(parts, line, boost::is_any_of(this->columnDelimeter));

	for (unsigned int i=0; i< parts.size(); i++){
		if (parts[i].size()==0)
			parts.erase(parts.begin()+i);
		parts[i] = noFrontSpaces(parts[i]);
	}

	return parts;
}

vector<string> Parser::tokenizeReviews(string line){
	vector<string> parts;
	vector<string> aux;
	boost::split(parts, line, boost::is_any_of(this->wordDelimeters));

	for (unsigned int i=0; i< parts.size(); i++){
		if (parts[i].size()==0)
			parts.erase(parts.begin()+i);
		parts[i] = noFrontSpaces(parts[i]);
	}
	string word;
	BOOST_FOREACH(word,parts)
	{
		if (word != "") aux.push_back(word);
	}
	return aux;
}

vector<string> Parser::tokenize(string line, string newDelimiters){
	vector<string> parts;
	boost::split(parts, line, boost::is_any_of(newDelimiters));

	for (unsigned int i=0; i< parts.size(); i++){
		if (parts[i].size()==0)
			parts.erase(parts.begin()+i);
		parts[i] = noFrontSpaces(parts[i]);
	}
	return parts;
}

string Parser::lastLine(string line) {
	vector<string> lines = this->tokenize(line, "\n.");
	while (lines[lines.size()-1].size() == 0) {
		lines.pop_back();
	}

	string l = lines[lines.size()-1];
	return noFrontSpaces(l);

}

bool Parser::isSwearing(string word, int minChars) {
	bool s = false;
	int counter = 0;

	for (unsigned int i=0; i< word.size(); i++) {
		for (unsigned int j=0; j< this->swearingChars.size(); j++) {
			if (word[i] == this->swearingChars[j])
				counter ++;
		}
	}
	if (counter >= minChars)
		s = true;
	return s;
}

vector<string> Parser::noSwearing(vector<string> tokens) {

	for (unsigned int i=0; i< tokens.size(); i++){
		if (this->isSwearing(tokens[i], 3)) // cantidad de chars extraños para considerarlo un insulto
			tokens.erase(tokens.begin()+i);
		//tokens[i] = noFrontSpaces(parts[i]);
	}

	return tokens;
}

//TODO fix this

bool Parser::isFractionScore(string s) {
	int counter  = 0;
	for (unsigned int i=0; i< s.size(); i++) {
		if (s[i]=='/')
			counter ++;
	}

	return (counter==1);
}

float Parser::fractionScore(string s) {
	float f = 0.5;
	if (isFractionScore(s)){
		vector<string> fs = tokenize(s, "/");
		if (fs.size()==2) {
			std::string::size_type sz;
			float f_aux = 0.0;//std::stof(fs[0], &sz)/std::stof(fs[1], &sz);
			if ((f_aux >= 0)&&(f_aux <= 1)) {
				f = f_aux;
			}
		}
	}

	return f;
}
