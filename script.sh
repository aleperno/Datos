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

    Colautti, Pernin, Piccone
    University of Buenos Aires
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
	printf "\n$1 (y/n)\n"
	read -n 1 -r
	printf "\n\n"
	if [[ $REPLY =~ ^[yY]$ ]]
	then
		return 0
	else
		return 1
	fi
}
normal()
{
	banner
	printf "\t Copying Files/labeledTrainData.tsv to aux.tsv"
	cp Files/labeledTrainData.tsv Files/aux.tsv
}

extra()
{
	banner
	printf "\t Copying Files/labeledTrainData.tsv to aux.tsv"
	cp Files/labeledTrainData.tsv Files/aux.tsv
	if si_o_no "Would you like to use the extra data set? (advised: yes) (requires Files/Amazon.tsv file)"
	then
		printf "Input the ammount of reviews to use? 1 to 300,000 (advised 300000)\n"
		read -r
		head -n $REPLY Files/amazon.tsv >> Files/aux.tsv
		cant=$(wc -l Files/aux.tsv | cut -f 1 -d ' ')
		cant=$(($cant-1))
		printf "The data set to be used has %d elements\n\n" "$cant"
	fi
}

extraAuto()
{
	banner
	printf "\t Copying Files/labeledTrainData.tsv to aux.tsv\n"
	cp Files/labeledTrainData.tsv Files/aux.tsv
	cat Files/amazon.tsv >> Files/aux.tsv
	cant=$(wc -l Files/aux.tsv | cut -f 1 -d ' ')
	cant=$(($cant-1))
	printf "The data set to be used has %d elements\n\n" "$cant"
}

compilar()
{
	banner
	if si_o_no "Wish to compile the project?";
	then
		printf "\tCompiling\n\n"
		make
	fi
}

preParsear()
{
	banner
	printf "\tPreparsing input files \n\n"
	#python preParser.py Files/labeledTrainData.tsv
	python preParser.py Files/aux.tsv
	python preParser.py Files/testData.tsv
}

ejecutarNormal()
{
	banner
	printf "\t Running the main program with original data set \n\n"
	printf "\t Initializing weights with -2 \n\n"
	./Datos complete_learning.tsv complete_test.tsv -2
	printf "Copying the result to tmp \n\n"
	cp popcorn.csv tmp/-2.csv

	banner
	printf "\t Initializing weights with -1 \n\n"
	./Datos complete_learning.tsv complete_test.tsv -1
	printf "\n Copying the result to tmp \n\n"
	cp popcorn.csv tmp/-1.csv

	banner
	printf "\t Initializing weights with 0 \n\n"
	./Datos complete_learning.tsv complete_test.tsv 0
	printf "\n Copying the result to tmp \n\n"
	cp popcorn.csv tmp/0.csv

	banner
	printf "\t Initializing weights with 1 \n\n"
	./Datos complete_learning.tsv complete_test.tsv 1
	printf "\n Copying the result to tmp \n\n"
	cp popcorn.csv tmp/1.csv

	banner
	printf "\t Initializing weights with 2 \n\n"
	./Datos complete_learning.tsv complete_test.tsv 2
	printf "\n Copying the result to tmp \n\n"
	cp popcorn.csv tmp/2.csv

	banner
	printf "\n\t Combining results \n\n"
	python avg.py 'tmp/*'

	printf "\n Deleting temporary files \n\n"
	rm tmp/*

	printf "\n Copying the combination to tmp\n\n"
	cp avg.csv tmp/avg.csv
}

ejecutarExtra()
{
	banner
	printf "\t Running the main program with extra data set \n\n"
	printf "\t Initializing weights with 0 \n\n"
	./Datos complete_learning.tsv complete_test.tsv 0
	printf "Copying the result to tmp \n\n"
	cp popcorn.csv tmp/extra.csv

	banner
	printf "\n\t Combining results \n\n"
	python avg.py 'tmp/*'

	printf "\n Deleting temporary files \n\n"
	rm tmp/*
	rm tmp

	printf "\n Naming end result final.csv \n\n"
	mv avg.csv final.csv

	#printf "\n Genero el grupo8_probs.csv \n\n"
	#tail -n 25000 final.csv | cut -d ',' -f 2 > grupo8_probs.csv

	printf "\n Deleting temporary files \n\n"
	rm popcorn.csv
}

testear()
{
	banner
	if si_o_no "Wish to obtain the AUC score? (requires python-sklearn)";
	then
		score=$(python test_results.py final.csv)
		echo $score
		score=$(echo $score | cut -d ' ' -f 4)
		cp final.csv Submittions/$score.csv
	fi
}

main()
{
	mkdir 'tmp'
	rm tmp/*
	clear
	signature
	printf "\nNo error handling, make sure you have all the necessary files"
	compilar
	normal
	preParsear
	ejecutarNormal
	#El set extra
	extra
	preParsear
	ejecutarExtra
	testear
	exit 0
}

main