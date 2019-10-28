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
#include <chrono>
using namespace std;
using namespace std::chrono; 


/*Générer un nombre aléatoire entre a et b, a<b */
int rand_a_b(int a, int b){
	if (a==b) return a; //pas de division par 0
	else return rand()%(b-a) +a;
}

/* Ajouter un  élément dans une trace particulière */
void add_element_one_trace(vector<vector<string>> &traces, string element, int numero_trace) {
	traces[numero_trace].push_back(element);
} 


/* Ajouter le même élément dans toutes les traces */
void add_element_all_traces(vector<vector<string>> &traces, string element) {
	for (int i = 0; i < traces.size(); i++) {
		traces[i].push_back(element);
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
void site_percentage_generation(vector<vector<string>> &traces, const string expression, int nb_seq, int duree1, int duree2) {
	
	int seq = 0;
	int delimiter1 = expression.find('|');
	string evenement;
	int nb_apparition_evenement;
	vector<string> chaos = {};
	int al_number; //nombre de points
	int al_position; //position évenement

	srand(time(NULL));  //set seed to generate random number

	if (delimiter1 == string::npos ) { //1 seul %
			int delimiter2 = expression.find('%');
			evenement = expression.substr(0, delimiter2);
			nb_apparition_evenement = stoi(expression.substr(delimiter2+1));

			while (nb_apparition_evenement != 0) {

				al_number = rand_a_b(duree1, duree2);

				for (int i = 0; i < al_number; i++) {
					chaos.push_back(". ");
				}

				al_position = rand_a_b(0, al_number-1);
				chaos[al_position] = evenement + " "; //On place l'évenement

				string s;
				for (const auto &piece : chaos) s += piece;
				add_element_one_trace(traces, s, seq);

				seq += 1;
				nb_apparition_evenement -= 1;
				chaos = {};
			}

			//Plus d'évenement à placer 
			while (seq != nb_seq) { //il faut continuer à générer les points pour les séquences restantes
				al_number = rand_a_b(duree1, duree2);

				for (int i = 0; i < al_number; i++) {
					chaos.push_back(". ");
				}

				string s;
				for (const auto &piece : chaos) s += piece;
				add_element_one_trace(traces, s, seq);

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
				al_number = rand_a_b(duree1, duree2);

				for (int i = 0; i < al_number; i++) {
					chaos.push_back(". ");
				}

				al_position = rand_a_b(0, al_number-1);
				chaos[al_position] = evenement  + " "; //On place l'évenement

				string s;
				for (const auto &piece : chaos) s += piece; //conversion du vecteur<string> en simple string
				add_element_one_trace(traces, s, seq);

				seq += 1;
				nb_apparition_evenement -= 1;
				chaos = {};
			}

			delimiter1 = last_part_expression.find('|');
		}

		//Pour le dernier %
		int delimiter2 = last_part_expression.find('%');
		evenement = last_part_expression.substr(0, delimiter2);
		nb_apparition_evenement = stoi(last_part_expression.substr(delimiter2+1));

		while (nb_apparition_evenement != 0) {
				al_number = rand_a_b(duree1, duree2);

				for (int i = 0; i < al_number; i++) {
					chaos.push_back(". ");
				}

				al_position = rand_a_b(0, al_number);
				chaos[al_position] = evenement  + " "; //On place l'évenement

				string s;
				for (const auto &piece : chaos) s += piece;
				add_element_one_trace(traces, s, seq);

				seq += 1;
				nb_apparition_evenement -= 1;
				chaos = {};
		}


		//Plus d'évenement à placer 
		while (seq != nb_seq) { //il faut continuer à générer les points pour les séquences restantes
			al_number = rand_a_b(duree1, duree2);


			for (int i = 0; i < al_number; i++) {
				chaos.push_back(". ");
			}

			string s;
			for (const auto &piece : chaos) s += piece;
			add_element_one_trace(traces, s, seq);

			seq +=1;
			chaos = {};
		}

		shuffle_traces(traces);

	}
}