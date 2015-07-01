"""
Uso: python test_results.py <resultados.csv>

Instalacion skelarn > apt-get install python-sklearn 
"""
import sys
import numpy as np
from sklearn.metrics import roc_auc_score

TRUE_PATH = "./Files/test.info"

SCORE_PATH = sys.argv[1]

d = {}

for e, line in enumerate(open(SCORE_PATH,"rb")):
	if (e > 0):
		data = line.strip().split(',')
		id = data[0]
		score = float(data[1])
		d[id] = [0,score]

for line in (open(TRUE_PATH,"rb")):
	data = line.strip().split(',')
	id = data[0]
	score = int(data[1])
	d[id][0] = score

y_true = []
y_score = []

for true, score in d.values():
	y_true.append(true)
	y_score.append(score)

y_true = np.array(y_true)
y_score = np.array(y_score)

print "Score: %0.5f" % roc_auc_score(y_true,y_score)