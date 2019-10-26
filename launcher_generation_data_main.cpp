#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>

using namespace std;

	/* COMMAND LINK EXAMPLE : ./launcher_generation_data_main . test 4 generation_data.par patterns.txt

	Command line arguments example : 
	https://www.geeksforgeeks.org/find-largest-among-three-different-positive-numbers-using-command-line-argument/
	C plus rapide que C++ ... Désire t elle qu'on fasse du C ?
	*/

int rand_a_b(int a, int b){
/*Générer un nombre aléatoire entre a et b, a<b */
	srand(time(NULL));
    return rand()%(b-a) +a;
}

int main(int argc, char** argv) {

	/*
	Séparer les déclarations de variables de leur affectation ?
	try except en C++ ?
	*/

	//checking
	if (argc != 6) {
		cerr << "Bad number of arguments. Check readme.md" << endl;
		exit(1);
	}

	//get parameters
	string output_directory=argv[1]; //dossier de sortie
	cout << "Output directory : " << output_directory << endl;
	string prefix_generation_data = argv[2]; //nom du préfixe fixe des jeux de données
	cout << "Prefix for generated datasets : " << prefix_generation_data << endl;
	int number_generated_data = atoi(argv[3]); //nombre de jeu de données désiré
	cout << "Number of generated datasets : " << number_generated_data << endl;
	string parameter_file=argv[4]; //nom du fichier de paramètres
	cout << "Name of the parameter file : " << parameter_file << endl;

	//lire le fichier de paramètres

	/* obligé de les déclarer en amont du while sinon n'est pas gardé en mémoire */
	int number_seq_max; //contient le nombre de séquences maximum par jeu de données
	int length_seq_max; //contient la longueur maximum des séquences
	string expression; //contient l'expression de génération des pattern de séquences
	int delimiter;
	string name_parameter;

	ifstream fin(parameter_file.c_str()); //on ouvre le fichier des paramètres
	if (fin.is_open()) {
		string line;
		int nb_parameter_given = 0; //pour vérifier si tous les paramètres sont bien fournis par l'utilisateur
		while (getline(fin, line)){
			if (line.find("//") == 0) continue; //ignorer les lignes commençant par #
			line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end()); //éliminer tous les espaces indésirables

			if(line[0] == '#' || line.empty()) //ne pas lire les commentaires et les lignes vides
                continue;
            delimiter = line.find(":"); //délimiteur entre le nom du paramètre et le paramètre lui-même
            name_parameter = line.substr(0, delimiter); 
             if (name_parameter == "number_seq_max" ) {
             	/* check if int */
            	number_seq_max = stoi(line.substr(delimiter + 1));
            } else if (name_parameter == "length_seq_max" ) {
            	/* check if int */
				length_seq_max = stoi(line.substr(delimiter + 1));
            } else if (name_parameter == "expression" ) {
            	/* check if known expression with a global regex */
				expression = line.substr(delimiter + 1);
			} else {
				cerr << "Unknown parameter. Please refer to readme.md" << endl;
				exit(2);
			}
			nb_parameter_given+=1;
		}

		//Not all parameters were given
		if (nb_parameter_given < 3) {
			cerr << "Not enough parameters given. Please refer to readme.md." << endl;
			exit(3);
		}

		fin.close();

	} else {
		cerr << "Couldn't open config file for reading" << endl;
		exit(4);
	}

	//Affichage des paramètres
	cout << "Number of seqs by dataset : " << number_seq_max << endl;
	cout << "Length max for a seq : " << length_seq_max << endl;
	cout << "Expression : " << expression << endl;
	cout << "_____" << endl;

	/* ZONE EN TRAVAUX */

	//Premier parcours : vérification si la somme des apparitions d'évenements est cohérente avec le nombre de séquence maximale

	string tmp;
	string first_part_expression;
	string last_part_expression;
	string exp;
	string jalon; //jalon de gauche
	int d1, d2; //durées 1 et 2
	int somme_d2 = 0; //vérifier si la somme des d2 est < longueur maximale d'une séquence
	string description_site; //partie variable
	int delimiter1, delimiter2, delimiter3, delimiter4, delimiter5, delimiter6; // <, >, |, (, ), ...

	last_part_expression = expression;
	delimiter1 = last_part_expression.find('<');

	while (delimiter1 != string::npos) {

		//Découpage progressif de l'expression
		delimiter2 = last_part_expression.find('>');
		first_part_expression	= last_part_expression.substr(0, delimiter2);
		last_part_expression = last_part_expression.substr(delimiter2+1);
		cout << "expression restante : " << last_part_expression << endl;
		
		//Jalon
		jalon = first_part_expression.substr(0, delimiter1);

		//Durée 1
		delimiter1 = first_part_expression.find('(');
		exp = first_part_expression.substr(delimiter1+1);
		delimiter2 = exp.find('-');
		d1 = stoi(exp.substr(0,delimiter2));

		//Durée 2
		delimiter1 = first_part_expression.find('-');
		exp = first_part_expression.substr(delimiter1+1);
		delimiter2 = exp.find(')');
		d2 = stoi(exp.substr(0,delimiter2));

		//Description du site
		delimiter1 = first_part_expression.find(')');
		exp = first_part_expression.substr(delimiter1+1);
		delimiter2 = exp.find('>');
		description_site = exp.substr(0,delimiter2);

		//Vérifs
		cout << "jalon de gauche : " << jalon << endl;
		cout << "d1 : " << d1 << endl;
		cout << "d2 : " << d2 << endl;
		cout << "description du site : " << description_site << endl;

		//Vérification si cohérence avec longueur max des séquences
		somme_d2  += d2;
		if (somme_d2 > length_seq_max) {
			cout << "somme des d2 : " << somme_d2 << endl;
			cerr << "too long !!!!!!" << endl;
		exit (5);
		}

		delimiter1 = last_part_expression.find('<');

	}

	jalon = last_part_expression;
	cout << "dernier jalon : " << last_part_expression << endl;


	return 0;

}

