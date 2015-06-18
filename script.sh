#!/bin/bash
signature() {
printf "#%.0s" {1..50}
echo
cat << "EOF"
 _____                  ___      _            
/__   \_ __            /   \__ _| |_ ___  ___ 
  / /\/ '_ \ _____    / /\ / _` | __/ _ \/ __|
 / /  | |_) |_____|  / /_// (_| | || (_) \__ \
 \/   | .__/        /___,' \__,_|\__\___/|___/
      |_|                                     

    Colautti, Pernin, Piccone © 1991-2001
EOF
printf "#%.0s" {1..50}
echo
}

banner()
{
	echo
	printf "#%.0s" {1..50}
	echo
}

si_o_no()
{
	printf "\n$1 (s/n)\n"
	read -n 1 -r
	printf "\n\n"
	if [[ $REPLY =~ ^[sS]$ ]]
	then
		return 0
	else
		return 1
	fi
}
normal()
{
	banner
	printf "\t Copiando Files/labeledTrainData.tsv a aux.tsv"
	cp Files/labeledTrainData.tsv Files/aux.tsv
}

extra()
{
	banner
	printf "\t Copiando Files/labeledTrainData.tsv a aux.tsv"
	cp Files/labeledTrainData.tsv Files/aux.tsv
	if si_o_no "Desea utilizar set de datos extra? (recomendado: SI)"
	then
		printf "Ingrese cantidad deseada a utilizar 1 a 300.000 (recomendado 300000)\n"
		read -r
		head -n $REPLY Files/amazon.tsv >> Files/aux.tsv
		cant=$(wc -l Files/aux.tsv | cut -f 1 -d ' ')
		cant=$(($cant-1))
		printf "El set de aprendizaje a utilizar posee %d elementos\n\n" "$cant"
	fi
}

extraAuto()
{
	banner
	printf "\t Copiando Files/labeledTrainData.tsv a aux.tsv\n"
	cp Files/labeledTrainData.tsv Files/aux.tsv
	cat Files/amazon.tsv >> Files/aux.tsv
	cant=$(wc -l Files/aux.tsv | cut -f 1 -d ' ')
	cant=$(($cant-1))
	printf "\nEl set de aprendizaje a utilizar posee %d elementos\n\n" "$cant"
}

compilar()
{
	banner
	if si_o_no "Desea compilar el proyecto?";
	then
		printf "\tCompilando el proyecto\n\n"
		make
	fi
}

preParsear()
{
	banner
	printf "\tPreparseando los archivos de entrada \n\n"
	#python preParser.py Files/labeledTrainData.tsv
	python preParser.py Files/aux.tsv
	python preParser.py Files/testData.tsv
}

ejecutarNormal()
{
	banner
	printf "\t Se correra el programa principal con el set de datos original \n\n"
	printf "\t Inicializando pesos en -2 \n\n"
	./Datos complete_learning.tsv complete_test.tsv -2
	printf "Copio el resultado en tmp \n\n"
	cp popcorn.csv tmp/-2.csv

	banner
	printf "\t Inicializando pesos en -1 \n\n"
	./Datos complete_learning.tsv complete_test.tsv -1
	printf "\n Copio el resultado en tmp \n\n"
	cp popcorn.csv tmp/-1.csv

	banner
	printf "\t Inicializando pesos en 0 \n\n"
	./Datos complete_learning.tsv complete_test.tsv 0
	printf "\n Copio el resultado en tmp \n\n"
	cp popcorn.csv tmp/0.csv

	banner
	printf "\t Inicializando pesos en 1 \n\n"
	./Datos complete_learning.tsv complete_test.tsv 1
	printf "\n Copio el resultado en tmp \n\n"
	cp popcorn.csv tmp/1.csv

	banner
	printf "\t Inicializando pesos en 2 \n\n"
	./Datos complete_learning.tsv complete_test.tsv 2
	printf "\n Copio el resultado en tmp \n\n"
	cp popcorn.csv tmp/2.csv

	banner
	printf "\n\t Combino los resultados \n\n"
	python avg.py 'tmp/*'

	printf "\n Elimino los resultados temporales \n\n"
	rm tmp/*

	printf "\n Copio la combinacion en tmp\n\n"
	cp avg.csv tmp/avg.csv
}

ejecutarExtra()
{
	banner
	printf "\t Se correra el programa principal con el set de datos extra \n\n"
	printf "\t Inicializando pesos en 0 \n\n"
	./Datos complete_learning.tsv complete_test.tsv 0
	printf "Copio el resultado en tmp \n\n"
	cp popcorn.csv tmp/extra.csv

	banner
	printf "\n\t Combino los resultados \n\n"
	python avg.py 'tmp/*'

	printf "\n Elimino los resultados temporales \n\n"
	rm tmp/*

	printf "\n Nombro el resultado final.csv \n\n"
	mv avg.csv final.csv

	printf "\n Genero el grupo8_probs.csv \n\n"
	tail -n 25000 final.csv | cut -d ',' -f 2 > grupo8_probs.csv

	printf "\n Elimino archivos extras \n\n"
	rm popcorn.csv
}

testear()
{
	banner
	if si_o_no "Desea obtener la metrica offline (requiere python-sklearn)";
	then
		python test_results.py final.csv
	fi
}

main()
{
	mkdir 'tmp'
	rmp tmp/*
	clear
	signature
	compilar
	normal
	preParsear
	ejecutarNormal
	#El set extra
	extraAuto
	preParsear
	ejecutarExtra
	testear
	exit 0
}

main

echo "Desea obtener la metrica offline (s/n)? (requiere python-sklearn)"
read -n 1 -r
echo #
if [[ $REPLY =~ ^[sS]$ ]]
then
	echo "Dijiste que si"
else
	echo "Dijiste que no"
fi