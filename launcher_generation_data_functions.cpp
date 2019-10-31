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

/*
Faire une fonction qui prend duree1, duree2, nb apparition evenement, evenement pour générer la string chaos
Remplacer trace par traces ...
*/

/*Générer un nombre aléatoire entre a et b, a<b */
int rand_a_b(int a, int b){
	if (a==b) return a; //pas de division par 0
	else return rand()%(b-a) +a;
}

//**********************************************

/* Ajouter un  élément dans une trace particulière */
void add_element_one_trace(vector<vector<string>> &traces, string element, int numero_trace) {
	traces[numero_trace].push_back(element);
} 

//**********************************************

/* Ajouter le même élément dans toutes les traces */
void add_element_all_traces(vector<vector<string>> &traces, string element) {
	for (int i = 0; i < traces.size(); i++) {
		traces[i].push_back(element);
	}
} 

//**********************************************

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

//***********************************************

/*Afficher les traces */
void save_traces(vector<vector<string>> &traces, ofstream& file) {
	for (int i = 0; i < traces.size(); i++) {
		for (int j = 0; j < traces[i].size(); j++) {
			file << traces[i][j] ;
		}
		file << endl; 
	}
}

//**********************************************

/* Shuffle traces */
void shuffle_traces(vector<vector<string>> & traces) {
	random_device rd;
	mt19937 g(rd());
	shuffle(traces.begin(), traces.end(), g);
}

//**********************************************

void shuffle_vector(vector<string> & v) {
	random_device rd;
	mt19937 g(rd());
	shuffle(v.begin(), v.end(), g);
}

//**********************************************

void shuffle_vector_int(vector<int> & v) {
	random_device rd;
	mt19937 g(rd());
	shuffle(v.begin(), v.end(), g);
}

//**********************************************

void findAllOccurances(vector<int> & vec, string description_site, char delimiter)
{
	// Get the first occurrence
	int pos = description_site.find(delimiter);
 
	// Repeat till end is reached
	while( pos != string::npos)
	{
		// Add position to the vector
		vec.push_back(pos);
 
		// Get the next occurrence from the current position
		pos =description_site.find(delimiter, pos + delimiter);
	}
}

//***********************************************

int compte_somme_nb_apparition_evenement(string description, int nb_trace, int longueur_trace, vector<char> delimiters) {

		int somme_nb_apparition_evenement = 0;
		string first_part_description, last_part_description;

		if (delimiters.size() == 2) {
			vector<int> nb_apparition_evenement;
			int delimiter1, delimiter2;

			delimiter1 = description.find(delimiters[0]);

			if (delimiter1 != string::npos) { //plusieurs événements possibles.
				last_part_description = description;

				while (delimiter1 != string::npos) {

					first_part_description = last_part_description.substr(0, delimiter1);
					last_part_description = last_part_description.substr(delimiter1+1);

					delimiter2 = first_part_description.find(delimiters[1]);

					if (delimiter2 != string::npos) {
						nb_apparition_evenement.push_back(stoi(first_part_description.substr(delimiter2+1)));
					} else {
						nb_apparition_evenement.push_back(1);
					}

					delimiter1 = last_part_description.find(delimiters[0]);

				}

				delimiter2 = last_part_description.find(delimiters[1]);

				if (delimiter2 != string::npos) {
					nb_apparition_evenement.push_back(stoi(last_part_description.substr(delimiter2+1)));
				} else {
					nb_apparition_evenement.push_back(1);
				}
		
			} else { //un seul évenement possible

				delimiter2 = description.find(delimiters[1]);

				if (delimiter2 != string::npos) {
					nb_apparition_evenement.push_back(stoi(description.substr(delimiter2+1)));
				} else {
					nb_apparition_evenement = {1};
				}

			}

			//Vérif
			for (auto & i : nb_apparition_evenement) {
				somme_nb_apparition_evenement += i;
			}

		} else if (delimiters.size() == 1) {

			int delimiter;

			delimiter = description.find(delimiters[0]);

			if (delimiter == string::npos ) {
				somme_nb_apparition_evenement = 1;
			} else {

				somme_nb_apparition_evenement = 1;
				while (delimiter != string::npos) {
					somme_nb_apparition_evenement +=  1;
					delimiter = description.find(delimiters[0],delimiter+1);
				}
			}

		} else {
			cout << "pas géré" << endl;
		}		

		return somme_nb_apparition_evenement;

}

/*Définir type expression de site*/
int which_type_of_site(string description, bool &vide_possible) {

	bool is_matching;

	is_matching = regex_match(description, regex("\\w+\\%\\d+(\\|\\w+\\%\\d+)*"));
	if (is_matching) {

		// vector<int> pos_delimiter = {};
		// vector<string> subdescriptions = {};
		// int start_pos = 0;

		// findAllOccurances(pos_delimiter, description, '|');

		// if (pos_delimiter != string::npos) { //Plusieurs évenements possibles
		// 	for (auto & pos : pos_delimiter) {
		// 		subdescriptions.push_back(start_pos, pos);
		// 	}

		// 	for (auto & subdescription : subdescriptions) {
		// 		delimiter 
		// 	}

	

		return 1;
	
	} else {

		is_matching = regex_match(description, regex("\\w+(\\|\\w+)*"));
		if (is_matching) return 2;

		else {

			is_matching = regex_match(description, regex("\\w+(\\:\\d+)*(\\;\\w+(\\:\\d+)*)*"));
			if (is_matching) return 3;

			else {

				return 0;

			}
		}
	}
}


//**********************************************

string generate_chaos(int d1, int d2, vector<string> v_evenements, vector<int> v) {

	vector<string> chaos = {};
	int al_number; //nombre de points
	int al_position; //position évenement
	string evenement;
	int nb_apparition_evenement;

	al_number = rand_a_b(d1, d2);

	for (int i = 0; i < al_number; i++) {
		chaos.push_back(". ");
	}

	if (v_evenements.size() == 1) { //1 seul évenement dans le même site

		evenement = v_evenements[0];
		nb_apparition_evenement = v[0];

	    if (nb_apparition_evenement == 1) {

		al_position = rand_a_b(0, al_number-1);
		chaos[al_position] = evenement + " "; //On place l'évenement

		} else { //L'évenement apparait plusieurs fois

			vector<int> positions_possibles = {};

			//Vecteur des positions possibles
			for (int i = 0; i < chaos.size(); i++) {
				positions_possibles.push_back(i);
			}

			while (nb_apparition_evenement > 0) {
				shuffle_vector_int(positions_possibles);
				al_position = positions_possibles[0];

				chaos[al_position] = evenement + " "; //On place l'évenement

				nb_apparition_evenement-=1;
				positions_possibles.erase(positions_possibles.begin() + 0); //On supprime la position utilisée : un événement ne doit pas être placé >= deux fois au même endroit.
			}
		}

	} else { //Plusieurs évenements dans même site

			vector<int> positions_possibles = {};

			//Vecteur des positions possibles
			for (int i = 0; i < chaos.size(); i++) {
				positions_possibles.push_back(i);
			}

			for (int i = 0; i < v_evenements.size(); i++ ) {

				evenement = v_evenements[i];
				nb_apparition_evenement = rand_a_b(1,v[i]);

				while (nb_apparition_evenement > 0) {
					shuffle_vector_int(positions_possibles);
					al_position = positions_possibles[0];

					chaos[al_position] = evenement + " "; //On place l'évenement

					nb_apparition_evenement-=1;
					positions_possibles.erase(positions_possibles.begin() + 0); //On supprime la position utilisée : un événement ne doit pas être placé >= deux fois au même endroit.
				}
			}
	}

	string s;
	for (const auto &piece : chaos) s += piece;
	return s;
}


//**********************************************

/* Génération des sites à % */
void type_site_1_generation(vector<vector<string>> &traces, const string expression, int nb_trace, int duree1, int duree2) {
	
	int trace = 0;
	int delimiter1 = expression.find('|');
	string evenement = "";
	int nb_apparition_evenement;
	string chaos = "";
	vector<string> v_evenements = {};
	vector<int> v;


	if (delimiter1 == string::npos ) { //1 seul %
			int delimiter2 = expression.find('%');
			evenement = expression.substr(0, delimiter2);
			nb_apparition_evenement = stoi(expression.substr(delimiter2+1));

			while (nb_apparition_evenement != 0) {

				v_evenements = {evenement};
				chaos = generate_chaos(duree1, duree2, v_evenements,  v = {1});

				add_element_one_trace(traces, chaos, trace);

				trace += 1;
				nb_apparition_evenement -= 1;
				chaos = "";
			}

			//Plus d'évenement à placer 
			while (trace != nb_trace) { //il faut continuer à générer les points pour les séquences restantes
				

				chaos = generate_chaos(duree1, duree2, v_evenements = {""}, v = {0});

				add_element_one_trace(traces, chaos, trace);

				trace +=1;
				chaos = "";
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
			
				v_evenements = {evenement};
				chaos = generate_chaos(duree1, duree2, v_evenements, v = {1});

				add_element_one_trace(traces, chaos, trace);

				trace += 1;
				nb_apparition_evenement -= 1;
				chaos = "";
			}

			delimiter1 = last_part_expression.find('|');
		}

		//Pour le dernier %
		int delimiter2 = last_part_expression.find('%');
		evenement = last_part_expression.substr(0, delimiter2);
		nb_apparition_evenement = stoi(last_part_expression.substr(delimiter2+1));

		while (nb_apparition_evenement != 0) {

				v_evenements = {evenement};
				chaos = generate_chaos(duree1, duree2, v_evenements, v = {1});

				add_element_one_trace(traces, chaos, trace);

				trace += 1;
				nb_apparition_evenement -= 1;
				chaos = "";
		}

		//Plus d'évenement à placer 
		while (trace != nb_trace) { //il faut continuer à générer les points pour les séquences restantes
	
			chaos = generate_chaos(duree1, duree2,  v_evenements = {""}, v = {0});
			
			add_element_one_trace(traces, chaos, trace);

			trace +=1;
			chaos = "";
		}

		shuffle_traces(traces);

	}
}

//**********************************************

/* Génération des sites à | */
void type_site_2_generation(vector<vector<string>> &traces, string description_site, int nb_trace, int duree1, int duree2) {
	
	int trace = 0;
	int delimiter = description_site.find('|');
	string evenement;
	string chaos;
	vector <int> v;
	vector <string> v_evenements;
	
	if (delimiter == string::npos ) { //1 seul évenement possible

		while (trace != nb_trace) {

			evenement = description_site;

			v_evenements = {evenement};
			chaos = generate_chaos(duree1, duree2, v_evenements, v = {1});
			add_element_one_trace(traces, chaos, trace);

			trace+=1;

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

		while (trace != nb_trace) {

			shuffle_vector(list_evenements); //permutation
			evenement = list_evenements[0]; //evenement du premier indice

			v_evenements = {evenement};
			chaos = generate_chaos(duree1, duree2, v_evenements, v = {1});

			add_element_one_trace(traces, chaos, trace);

			trace+=1;
			chaos = "";
		}
	}
	shuffle_traces(traces);
}

//**********************************************

/* Génération des sites à : */
void type_site_3_generation(vector<vector<string>> &traces, string description_site, int nb_trace, int duree1, int duree2) {
	
	int trace = 0;
	string evenement;
	int nb_apparition_evenement;
	string chaos;
	vector<int> positions_possibles = {};
	int i; 
	vector <int> v;
	vector <string> v_evenements;

	int delimiter2;

	int delimiter1 = description_site.find(';');

	if (delimiter1 == string::npos ) { //1 seul évenement possible

		delimiter2 = description_site.find(':');

		if (delimiter2 == string::npos) { // L'évenement apparaît une seule fois, dans toutes les traces

			while (trace != nb_trace) {

				evenement=description_site;

				v_evenements = {evenement};
				chaos = generate_chaos(duree1, duree2, v_evenements, v = {1});

				add_element_one_trace(traces, chaos, trace);

				trace+=1;

			}

		} else { //L'évenement peut apparaître plusieurs fois dans toutes les traces. 

			evenement = description_site.substr(0,delimiter2);

			while (trace != nb_trace) {

				v = {rand_a_b(1,stoi(description_site.substr(delimiter2+1))+1)}; //On tire au sort le nombre d'apparition de l'évenement entre 1 et le nombre d'apparitions maximal

				// cout << "nombre apparition de l'évenement " << evenement << " : " << nb_apparition_evenement << endl;

				// cout << "max apparition possible : " << stoi(description_site.substr(delimiter2+1)) << endl;

				
				v_evenements = {evenement};
				chaos = generate_chaos(duree1, duree2, v_evenements, v );

				// cout << "chaos généré : " << chaos << endl;

				add_element_one_trace(traces, chaos, trace);

				trace+=1;

			}

		}

	} else { //Plusieurs évenements possibles dans un même site

		string first_part_expression, last_part_expression;
		last_part_expression = description_site;

			while (delimiter1 != string::npos) {

				first_part_expression = last_part_expression.substr(0, delimiter1);
				last_part_expression = last_part_expression.substr(delimiter1+1);

				delimiter2 = first_part_expression.find(':');

				if (delimiter2 != string::npos) { //l'évenement peut app pls fois

					evenement = first_part_expression.substr(0,delimiter2);
					nb_apparition_evenement = stoi(first_part_expression.substr(delimiter2+1));

					delimiter1 = last_part_expression.find(';');

					v_evenements.push_back(evenement);
					v.push_back(nb_apparition_evenement);

				} else { //l'évenement n'apparaît qu'une fois

					evenement = first_part_expression;
					nb_apparition_evenement = 1;
					v_evenements.push_back(evenement);
					v.push_back(1);

				}

			}

			delimiter2 = last_part_expression.find(':');

			//Dernière partie
			if (delimiter2 != string::npos) {

				evenement = last_part_expression.substr(0,delimiter2);
				nb_apparition_evenement = stoi(last_part_expression.substr(delimiter2+1));
				// cout << evenement << " : " << nb_apparition_evenement << endl;
				v_evenements.push_back(evenement);
				v.push_back(nb_apparition_evenement);

			} else {

				evenement = last_part_expression;
				nb_apparition_evenement = 1;
				v_evenements.push_back(evenement);
				v.push_back(1);

			}


			while (trace != nb_trace) {
				chaos = generate_chaos(duree1, duree2, v_evenements , v );
				add_element_one_trace(traces, chaos, trace);
				trace += 1;
			}
			


	}
}


//**********************************************

void handle_sites(vector<vector<string>> &traces, string premier_jalon, string dernier_jalon, int d1, int d2, string expression, int nb_trace, vector<int>& types_of_sites) {

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
					type_site_1_generation(traces, description_site, nb_trace, d1, d2);
					break;
				case 2:
					type_site_2_generation(traces, description_site, nb_trace, d1, d2);
					break;
				case 3:
					type_site_3_generation(traces, description_site, nb_trace, d1, d2);
					break;
			}
			
			delimiter1 = last_part_expression.find('<');

			//Jalon
			premier_jalon = last_part_expression.substr(0, delimiter1)+" ";

			cpt_site+=1;

		}

		add_element_all_traces(traces, dernier_jalon); //ajout dernier jalon
}
