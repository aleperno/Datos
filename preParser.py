#!/usr/bin/python
from __future__ import division
import sys
import re

"""
python preParser.py <archivo>
"""
BAD_SMILEYS_PATH = "bad-smileys.txt"
GOOD_SMILEYS_PATH = "good-smileys.txt"

LEARNING_NAME = "complete_learning.tsv"
TEST_NAME = "complete_test.tsv"

class Parser:

	def __init__(self):
		self.badSmileys = self.loadBadSmileys()
		self.goodSmileys = self.loadGoodSmileys()

	def loadBadSmileys(self):
		d = {}
		smileysFile = open(BAD_SMILEYS_PATH)
		for smiley in smileysFile:
			smiley = smiley.replace('\n','')
			d[smiley] = None
		return d

	def loadGoodSmileys(self):
		d = {}
		smileysFile = open(GOOD_SMILEYS_PATH)
		for smiley in smileysFile:
			smiley = smiley.replace('\n','')
			d[smiley] = None
		return d

	def parseLearningLine(self,line):
		(movie_id, sentiment, review) = line.split('\t')
		#movie_id = movie_id[1:][:-1]
		review = review[1:][:-1]
		return (movie_id, sentiment, review)

	def parseTestLine(self,line):
		(movie_id, review) = line.split('\t')
		#movie_id = movie_id[1:][:-1]
		review = review[1:][:-1]
		return (movie_id, review)


	def parseReview(self,review):
		review = review.replace('<br />','')
		review = review.replace('"','')
		review = review.replace('\\','')
		return review
	
	def clean(self,s):
		"""
			Returns a cleaned, lowercased string
		"""
		return " ".join(re.findall(r'\w+', s,flags = re.UNICODE | re.LOCALE)).lower()

	def parse(self,mode,line):
		if mode:
			return self.clean(line)
		else:
			return self.parseReview(line)


def main():
	file = open(sys.argv[1])
	
	try:
		mode = int(sys.argv[2]) != 0
	except:
		mode = False

	parser = Parser()
	#Verifico que tipo de archivo se trata
	line = file.readline()
	if (len(line.split()) is 3):
		#LearningData
		is_learning = True
		output_name = LEARNING_NAME
	else:
		is_learning = False
		output_name = TEST_NAME

	#Creo el archivo de salida
	output = open(output_name,"wb")

	print ("Archivo salida: %s, con metodo alternativo: %s") % (output_name,str(mode))
	if is_learning:
		for line in file:
			line = line.replace('\n','')
			(movie_id, sentiment, review) = parser.parseLearningLine(line)
			review = parser.parse(mode,review).lower()
			output.write(movie_id+"\t"+sentiment+"\t"+review+"\n")
	else:
		for line in file:
			line = line.replace('\n','')
			(movie_id, review) = parser.parseTestLine(line)
			review = parser.parse(mode,review).lower()
			output.write(movie_id+"\t"+review+"\n")

if __name__ == "__main__" :
	main()