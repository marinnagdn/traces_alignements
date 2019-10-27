#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <random>


using namespace std;

	/* COMMAND LINK EXAMPLE : ./launcher_generation_data_main . test 4 generation_data.par

	Command line arguments example : 
	https://www.geeksforgeeks.org/find-largest-among-three-different-positive-numbers-using-command-line-argument/
	C plus rapide que C++ ... Désire t elle qu'on fasse du C ?
	*/

/*Générer un nombre aléatoire entre a et b, a<b */
int rand_a_b(int a, int b){
	srand(time(NULL));
    return rand()%(b-a) +a;
}

/* Ajouter un  élément dans une trace particulière */
void add_element_one_trace(vector<vector<string>> &traces, vector<string> element, int numero_trace) {
	traces.push_back(element);
} 


/* Ajouter le même élément dans toutes les traces */
void add_element_all_traces(vector<vector<string>> &traces, vector<string> element) {
	cout << "taille du tableau : " << traces.size() << endl;
	for (int i = 0; i < traces.size(); i++) {
		traces.push_back(element);
	}
} 

/*Afficher les traces */
void afficher_traces(vector<vector<string>> &traces) {
	cout << "***" << endl;
	for (int i = 0; i < traces.size(); i++) {
		for (int j = 0; j < traces[i].size(); j++) {
			cout << traces[i][j] ;
		}
		cout << endl; 
	}
	cout << "***" << endl;
}

/* Shuffle traces */
void shuffle_traces(vector<vector<string>> & traces) {
	random_device rd;
	mt19937 g(rd());
	shuffle(traces.begin(), traces.end(), g);
}

/*Définir type expression de site*/
int which_type_of_site(string description, bool &vide_possible) {
	// int delimiter = description.find('|');
	// if (delimiter != string::npos) {
	// 	delimiter = description.find ('%');
	// }
	bool is_matching;

	// is_matching = regex_match(description, regex("(.+)(\\|+.+)*"));
	is_matching = regex_match(description, regex("(.+)(\\|+)"));

	cout << description << endl;
	cout << (is_matching ? "ça match" : "ça match pas") ;
	cout << endl;

	return 0;
}

/* Génération des sites à % */
void site_percentage_generation(const string expression, vector<vector<string>> &traces, int nb_seq, int duree1, int duree2) {
	int seq = 0;
	int delimiter1 = expression.find('|');
	string evenement;
	int nb_apparition_evenement;

	if (delimiter1 == string::npos ) { //1 seul %
			int delimiter2 = expression.find('%');
			evenement = expression.substr(0, delimiter2);
			nb_apparition_evenement = stoi(expression.substr(delimiter2+1));

				vector<string> chaos = {};
				int al_number; //nombre de points
				int al_position; //position évenement

			while (nb_apparition_evenement != 0) {

			
				al_number = rand_a_b(duree1, duree2);

				for (int i = 0; i < al_number; i++) {
					chaos.push_back(".") ;
				}

				al_position = rand_a_b(0, al_number-1);
				chaos[al_position] = evenement; //On place l'évenement

				add_element_one_trace(traces, chaos, seq);

				seq += 1;
				nb_apparition_evenement -= 1;
				chaos = {};
			}

			//Plus d'évenement à placer 
			while (seq != nb_seq) { //il faut continuer à générer les points pour les séquences restantes
				al_number = rand_a_b(duree1, duree2);

				for (int i = 0; i < al_number; i++) {
					chaos.push_back(".") ;
				}

				add_element_one_trace(traces, chaos, seq);

				seq +=1;
				chaos = {};
			}

			shuffle_traces(traces);

		} else { //Plusieurs %

		string first_part_expression, last_part_expression;
		last_part_expression = expression;

		while (delimiter1 != string::npos ) {
			first_part_expression = last_part_expression.substr(0, delimiter1);
			last_part_expression = last_part_expression.substr(delimiter1 + 1);
			int delimiter2 = first_part_expression.find('%');
			evenement = first_part_expression.substr(0, delimiter2);
			nb_apparition_evenement = stoi(first_part_expression.substr(delimiter2+1));

			while (nb_apparition_evenement != 0) {
					//generer les points selon chiffre piqué au hasard entre d1 et d2
					//generer position aléatoire au sein de ces points
					//placer l'évenement dedans
					seq += 1;
					nb_apparition_evenement -= 1;
				}

			delimiter1 = last_part_expression.find('|');
		}

		//Pour le dernier %
		int delimiter2 = last_part_expression.find('%');
		evenement = last_part_expression.substr(0, delimiter2);
		nb_apparition_evenement = stoi(last_part_expression.substr(delimiter2+1));

		while (nb_apparition_evenement != 0) {
					//generer les points selon chiffre piqué au hasard entre d1 et d2
					//generer position aléatoire au sein de ces points
					//placer l'évenement dedans
					seq += 1;
					nb_apparition_evenement -= 1;
		}


		//Plus d'évenement à placer 
		while (seq != nb_seq) { //il faut continuer à générer les points pour les séquences restantes
				//generer les points selon chiffre piqué au hasard entre d1 et d2
			seq +=1;
		}
			//Shuffle le tableau
	}
}


int main(int argc, char** argv) {

	/*
	Séparer les déclarations de variables de leur affectation ?
	try except en C++ ?
	*/

	//checking
	if (argc != 5) {
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

	string first_part_expression;
	string last_part_expression;
	string exp;
	string jalon; //jalon de gauche
	int d1, d2; //durées 1 et 2
	int somme_d2 = 0; //vérifier si la somme des d2 est < longueur maximale d'une séquence
	string description_site; //partie variable
	int delimiter1, delimiter2; // <, >, |, (, ), ...

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

		//Type description du site
		bool aucun_evenement_possible = 0 ;
		int type_site = 0;
		type_site = which_type_of_site(description_site, aucun_evenement_possible);

		delimiter1 = last_part_expression.find('<');

	}

	jalon = last_part_expression;
	cout << "dernier jalon : " << last_part_expression << endl;

	//GENERATION DU TABLEAU
	vector<vector<string>> Traces (number_seq_max);
	vector<string> ev = {"E1"};
	vector<string> ch = {".",".","."};

	add_element_all_traces(Traces, ev);
	afficher_traces(Traces);
	add_element_one_trace(Traces, ch , 2);
	afficher_traces(Traces);
	shuffle_traces(Traces);
	afficher_traces(Traces);

	return 0;
}

/*
Réussir à ajouter des vecteur de string dans des vecteurs de vecteurs de string
*/