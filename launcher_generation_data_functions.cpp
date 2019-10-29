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

void shuffle_vector(vector<string> & v) {
	random_device rd;
	mt19937 g(rd());
	shuffle(v.begin(), v.end(), g);
}

/*Définir type expression de site*/
int which_type_of_site(string description, bool &vide_possible) {

	bool is_matching;

	is_matching = regex_match(description, regex("\\w+\\%\\d+(\\|\\w+\\%\\d+)*"));
	if (is_matching) return 1;

	else {

		is_matching = regex_match(description, regex("\\w+(\\|\\w+)*"));
		if (is_matching) return 2;

		else {

			is_matching = regex_match(description, regex("\\w+(\\:\\d+)*(\\;\\w+(X\\d+)*)*"));
			if (is_matching) return 3;

			else {

				return 0;

			}
		}
	}
}

void verification_expression(string expression, int nb_seq, int longueur_seq_max, bool &continuer,  string& premier_jalon, string& dernier_jalon, int& d1, int &d2, string& description_site) {

	string first_part_expression;
	string last_part_expression;
	string exp;
	int somme_d2 = 0; //vérifier si la somme des d2 est < longueur maximale d'une séquence
	int delimiter1, delimiter2; // <, >, |, (, ), ...

	last_part_expression = expression;
	delimiter1 = last_part_expression.find('<');
	
	//Jalon
	premier_jalon = last_part_expression.substr(0, delimiter1);
		

	while (delimiter1 != string::npos) {

		//Découpage progressif de l'expression
		delimiter2 = last_part_expression.find('>');
		first_part_expression	= last_part_expression.substr(0, delimiter2);
		last_part_expression = last_part_expression.substr(delimiter2+1);

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
		// cout << "premier jalon : " << premier_jalon << endl;
		// cout << "d1 : " << d1 << endl;
		// cout << "d2 : " << d2 << endl;
		// cout << "description du site : " << description_site << endl;

		//Vérification si cohérence avec longueur max des séquences
		somme_d2  += d2;
		if (somme_d2 > longueur_seq_max) {
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

	dernier_jalon = last_part_expression;
	continuer = 1; //vérif ok
}

/* Génération des sites à % */
void type_site_1_generation(vector<vector<string>> &traces, const string expression, int nb_seq, int duree1, int duree2) {
	
	int seq = 0;
	int delimiter1 = expression.find('|');
	string evenement;
	int nb_apparition_evenement;
	vector<string> chaos = {};
	int al_number; //nombre de points
	int al_position; //position évenement


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


/* Génération des sites à % */
void type_site_2_generation(vector<vector<string>> &traces, string description_site, int nb_seq, int duree1, int duree2) {
	
	int seq = 0;
	int delimiter = description_site.find('|');
	string evenement;
	int nb_apparition_evenement;
	vector<string> chaos = {};
	int al_number; //nombre de points
	int al_position; //position évenement

	
	if (delimiter == string::npos ) { //1 seul évenement possible

		while (seq != nb_seq) {

			evenement = description_site;

			al_number = rand_a_b(duree1, duree2);

			for (int i = 0; i < al_number; i++) {
				chaos.push_back(". ");
			}

			al_position = rand_a_b(0, al_number-1);
			chaos[al_position] = evenement + " "; //on place l'évenement

			string s;
			for (const auto &piece : chaos) s += piece;
			add_element_one_trace(traces, s, seq);

			seq+=1;

		}

	} else { //Plusieurs évenements possibles

		vector<string> list_evenements = {};

		while (delimiter != string::npos) {
				evenement = description_site.substr(0, delimiter);
				list_evenements.push_back(evenement);
				description_site = description_site.substr(delimiter+1);
				delimiter = description_site.find('|');
		}

		//dernier evenement possible
		evenement = description_site;
		list_evenements.push_back(evenement); 

		while (seq != nb_seq) {

			shuffle_vector(list_evenements); //permutation
			evenement = list_evenements[0]; //evenement du premier indice

			al_number = rand_a_b(duree1, duree2);

			for (int i = 0; i < al_number; i++) {
				chaos.push_back(". ");
			}

			al_position = rand_a_b(0, al_number-1);
			chaos[al_position] = evenement + " "; //on place l'évenement

			string s;
			for (const auto &piece : chaos) s += piece;
			add_element_one_trace(traces, s, seq);

			seq+=1;
			chaos = {};
		}
	}
	shuffle_traces(traces);
}

void handle_sites(vector<vector<string>> &traces, string premier_jalon, string dernier_jalon, int d1, int d2, string expression, int nb_seq, vector<int>& types_of_sites) {

		string first_part_expression, last_part_expression;
		string exp;
		string description_site;
		int delimiter1, delimiter2;

		last_part_expression = expression;
		delimiter1 = last_part_expression.find('<');

		int type_site;
		int cpt_site = 0;

		// cout << "Types des sites : " << endl;
		// for (auto i = 0; i < types_of_sites.size(); i++) {
		// 	cout << types_of_sites[i] << " " ;
		// }
		// cout << endl;

		while (delimiter1 != string::npos) {

			//Découpage progressif de l'expression
			delimiter2 = last_part_expression.find('>');
			first_part_expression	= last_part_expression.substr(0, delimiter2);
			last_part_expression = last_part_expression.substr(delimiter2+1);

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

			//Nouveau jalon
			add_element_all_traces(traces, premier_jalon); 

			type_site = types_of_sites[cpt_site];

			switch(type_site) {
				case 1:
					type_site_1_generation(traces, description_site, nb_seq, d1, d2);
					break;
				case 2:
					type_site_2_generation(traces, description_site, nb_seq, d1, d2);
					break;
			}
			
			delimiter1 = last_part_expression.find('<');

			//Jalon
			premier_jalon = last_part_expression.substr(0, delimiter1)+" ";

			cpt_site+=1;

		}

		add_element_all_traces(traces, dernier_jalon); //ajout dernier jalon
}
