#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <tinyxml2.h>
//#include <tinystr.h>
//#include <fst/fstlib.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "kxmlVersOpenFstConfig.h"

//using namespace fst;
using namespace std;
using namespace tinyxml2;

void affiche(XMLElement* Noeud, string fichierTaches){
	
	string nom_racine = Noeud->Name();
	ofstream LesTaches(fichierTaches,std::ofstream::out | std::ofstream::app);
	//printf( "Nom du premier element: %s\n", nom_racine);
	//char* eltRech = "task";
	if(nom_racine=="task"){
			//printf( "J'ai une tâche\n");
			//le nom
			//const XMLAttribute* nom_tache;
			XMLElement* nom= Noeud->FirstChildElement("task-name");
			const char* value_nom = nom->GetText();
			//nom_tache=Noeud->FindAttribute( );
			//Noeud->FirstChildElement("task-name");
			
    		//const char* valeur_nom_tache = nom_tache->GetText();
			//printf( "Tache : %s\n", value_nom);
			//le numéro
			
			XMLElement* numero= Noeud->FirstChildElement("task-numero");
			const char* value_numero = numero->GetText();
			//printf( "%s %s\n", value_nom, value_numero);
			string value_numero_string=value_numero;
			string value_nom_string=value_nom;
			LesTaches << value_numero_string <<" " << value_nom_string << endl;
		}
	else{
			//printf( "C'est un element de type %s\n", nom_racine);
		//printf( "Dans else \n");
	}
	LesTaches.close();
	
}


void affiche_recursif(XMLElement* Noeud, string fichier){
	
	affiche(Noeud, fichier);
	
	XMLElement* Fils1=Noeud->FirstChildElement("task");
	XMLElement* Frere;
	
	while(Fils1){
		Frere=Fils1->NextSiblingElement("task");
		affiche_recursif(Fils1, fichier);
		Fils1=Frere;
	}
	
	
}


void create_SymbolTable(string FichierTaches, string FichierSymbole){
	
	string ligne;
	
	//le fichier syms
	ofstream LesSymboles(FichierSymbole);
	
	
	ifstream LesTaches(FichierTaches);
	getline(LesTaches, ligne);
	stringstream buffer;
	buffer << ligne;
		
	if(LesTaches){
		string mot;
		char* nom;

		int i =2;
		//printf( "eps 0 \n");
		LesSymboles << "eps 0" << endl;
		LesSymboles << "R 1" << endl;

		while(!LesTaches.eof()){
			//cout << "Je suis dans la boucle" <<endl;
			buffer >> mot;
			nom = (char*)mot.c_str();
			//printf( "%s %i\n", nom, i);
			LesSymboles << mot <<" "<<i<< endl;
			i++;
			//cout << "Je change de ligne" <<endl;
			//cout << "Voici l'ancienne : %s" << ligne <<endl;
			getline(LesTaches, ligne);
			//cout << "Voici la nouvelle : %s" << ligne <<endl;
			buffer.str("");
			buffer << ligne;
			//cout << "Le buffer : %s" << buffer <<endl;

			
		}
	}
	else{
		printf( "ERREUR: Impossible d'ouvrir le fichier \n");
	}
	
	LesSymboles.close();

	
}

string intToString(int i){
	 std::ostringstream oss;
     oss << i;
     return oss.str();
}

//fonction qui genere un automate (fixhier txt) correspondant à la production de Tache (par son identifiant numérique) par ses sous-tâches sequentielles
void create_Automate_TacheSequentielle(XMLElement* Noeud, string FichierTaches){
	XMLElement* nom= Noeud->FirstChildElement("task-name");
	XMLElement* num= Noeud->FirstChildElement("task-numero");
	const char* nom_value = nom->GetText();
	const char* num_value = num->GetText();
	string num_mere=num_value;
	int num_fraterie=1;//une tache a au moins 2 fils
	string num_fils;
	string dernier_fils;
	string ligne;
	ifstream LesTaches(FichierTaches);
	getline(LesTaches, ligne);
	stringstream buffer;
	buffer << ligne;
	int state=0;
	
	//creation du fichier qui va contenir l'automate
	string Tache=num_value;
	string Nom_Fichier="Out/SeqA_"+Tache+".txt";
	ofstream Automate(Nom_Fichier);
	
	//trouver tous les enfants (à partir du deuxieme
	if((num_mere=="Root")||(num_mere=="Racine")){
		dernier_fils=intToString(num_fraterie);
		num_fraterie++;
		num_fils=intToString(num_fraterie);
	}
	else{
		dernier_fils=num_mere+"."+intToString(num_fraterie);
		num_fraterie++;
		num_fils=num_mere+"."+intToString(num_fraterie);
	}
	
	if(LesTaches){
		string mot;
		char* nom;


		while(!LesTaches.eof()){
			//cout << "Je suis dans la boucle" <<endl;
			buffer >> mot;
			nom = (char*)mot.c_str();
			printf( "%s\n", nom);
			
			//comparaison mot
			if(mot==num_fils){
				//on a un fils on peut ecrire l'aine
				cout <<"on a trouvé le fils :" <<num_fils << endl;
				Automate << state <<" "<< state +1<< " "<<dernier_fils<< " eps"<< endl;
				dernier_fils=num_fils;
				state ++;
				
				//pour le suivant
				num_fraterie++;
				if((num_mere=="Root")||(num_mere=="Racine")){
					num_fils=intToString(num_fraterie);
				}
				else{
					num_fils=num_mere+"."+intToString(num_fraterie);
				}

			}
			
			//cout << "Je change de ligne" <<endl;
			//cout << "Voici l'ancienne : %s" << ligne <<endl;
			getline(LesTaches, ligne);
			//cout << "Voici la nouvelle : %s" << ligne <<endl;
			buffer.str("");
			buffer << ligne;
			//cout << "Le buffer : %s" << buffer <<endl;

			
		}
		Automate << state << " 0 "<< dernier_fils<<" "<< num_mere << endl;
		Automate << "0 " <<endl;
		
	}
	else{
		printf( "ERREUR: Impossible d'ouvrir le fichier \n");
	}

	
	
	Automate.close();
	
}

//fonction qui genere un automate (fixhier txt) correspondant à la production de Tache (par son identifiant numérique) par ses sous-tâches alternatives
void create_Automate_TacheAlternative(XMLElement* Noeud, string FichierTaches){
	XMLElement* nom= Noeud->FirstChildElement("task-name");
	XMLElement* num= Noeud->FirstChildElement("task-numero");
	const char* nom_value = nom->GetText();
	const char* num_value = num->GetText();
	
	string num_mere=num_value;
	int num_fraterie=1;//une tache a au moins 2 fils
	string num_fils;
	string dernier_fils;
	string ligne;
	ifstream LesTaches(FichierTaches);
	getline(LesTaches, ligne);
	stringstream buffer;
	buffer << ligne;
	int state=0;
	
	//creation du fichier qui va contenir l'automate
	string Tache=num_value;
	string Nom_Fichier="Out/AltA_"+Tache+".txt";
	ofstream Automate(Nom_Fichier);
	

	//trouver tous les enfants (à partir du deuxieme
	if((num_mere=="Root")||(num_mere=="Racine")){
		dernier_fils=intToString(num_fraterie);
		num_fraterie++;
		num_fils=intToString(num_fraterie);
	}
	else{
		dernier_fils=num_mere+"."+intToString(num_fraterie);
		num_fraterie++;
		num_fils=num_mere+"."+intToString(num_fraterie);
	}
	
	if(LesTaches){
		string mot;
		char* nom;


		while(!LesTaches.eof()){
			//cout << "Je suis dans la boucle" <<endl;
			buffer >> mot;
			nom = (char*)mot.c_str();
			printf( "%s\n", nom);
			
			//comparaison mot
			if(mot==num_fils){
				//on a un fils on peut ecrire l'aine
				//cout <<"on a trouvé le fils :" <<num_fils << endl;
				Automate << state <<" "<< state << " "<<dernier_fils<< " "<< num_mere<< endl;
				dernier_fils=num_fils;
				
				//pour le suivant
				num_fraterie++;
				if((num_mere=="Root")||(num_mere=="Racine")){
					num_fils=intToString(num_fraterie);
				}
				else{
					num_fils=num_mere+"."+intToString(num_fraterie);
				}

			}
			
			//cout << "Je change de ligne" <<endl;
			//cout << "Voici l'ancienne : %s" << ligne <<endl;
			getline(LesTaches, ligne);
			//cout << "Voici la nouvelle : %s" << ligne <<endl;
			buffer.str("");
			buffer << ligne;
			//cout << "Le buffer : %s" << buffer <<endl;

			
		}
		Automate << state <<" "<< state << " "<<dernier_fils<< " "<< num_mere<< endl;
		Automate << "0 " <<endl;
		
	}
	else{
		printf( "ERREUR: Impossible d'ouvrir le fichier \n");
	}

	
	
	Automate.close();
	
	
	
}

//fonction qui genere un automate (fixhier txt) correspondant à la production de Tache (par son identifiant numérique) par ses sous-tâches parallèles
void create_Automate_TacheParallele(XMLElement* Noeud){
	XMLElement* nom= Noeud->FirstChildElement("task-name");
	const char* nom_value = nom->GetText();
	string Tache=nom_value;
	string Nom_Fichier="Out/ParA_"+Tache+".txt";
	ofstream Automate(Nom_Fichier);
}

//fonction qui genere un automate (fixhier txt) correspondant à la production de Tache (par son identifiant numérique) par ses sous-tâches sans ordre
void create_Automate_TacheSansOrdre(XMLElement* Noeud){
	XMLElement* nom= Noeud->FirstChildElement("task-name");
	const char* nom_value = nom->GetText();
	string Tache=nom_value;
	string Nom_Fichier="Out/SoA_"+Tache+".txt";
	ofstream Automate(Nom_Fichier);
}


void recherche_recursif(XMLElement* Noeud, string fichierXML, string nom_tache, string fichierNomTaches){
	
	XMLDocument doc;
	XMLElement* ordonnancement;
	char* nom = (char*)fichierXML.c_str();
	XMLError err =doc.LoadFile(nom);
	//cout << " dans recherche recursif pour la tache"<< nom_tache << endl;
	
	if(err==0){
		XMLElement* num= Noeud->FirstChildElement("task-numero");
		const char* value_num = num->GetText();
		//printf( "dans if avec value_nom %s\n", value_num);
		if(value_num==nom_tache){
			//cout << "la tache est trouve" << endl;
			//doc.close();
			
			ordonnancement= Noeud->FirstChildElement("task-decomposition");
			string ordo=ordonnancement->GetText();
			//printf( "le noeud est  %s\n", ordonnancement->GetText());
			if(ordo =="ALT"){
				create_Automate_TacheAlternative(Noeud, fichierNomTaches);
			}
			if (ordo=="SEQ"){
				create_Automate_TacheSequentielle(Noeud, fichierNomTaches);
			}
			if(ordo =="PAR"){//attention on choisit dans cette version de traiter le parallèlisme comme du alt
				create_Automate_TacheAlternative(Noeud, fichierNomTaches);
			}
			else{//reste le cas des parallèles et sans ordre
			}
		

		}
		else{
	
			XMLElement* Fils1=Noeud->FirstChildElement("task");
			XMLElement* Frere;
	
			while(Fils1){
				Frere=Fils1->NextSiblingElement("task");
				recherche_recursif(Fils1, fichierXML, nom_tache, fichierNomTaches);
				Fils1=Frere;
			}
		}
	}
	else{
		printf( "problème chargement\n");
	}
	//printf( "dans if avec value_nom %s\n", trouve->GetText());
}

void create_Automates(string fichierNomTaches, string fichierMdT){
	
	//ouverture et lecture du fichier avec le nom des taches
	ifstream LesTaches(fichierNomTaches);
	string ligne;
	string tache;
	getline(LesTaches, ligne);
	stringstream buffer;
	buffer << ligne;
	
	
	XMLElement* racine;
	XMLElement* Fils1;
	XMLElement* tacheXML ;
	XMLElement* ordonnancement;

	
	if(LesTaches){

		
		//ouverture du fichier kxml
		XMLDocument doc;
		char* nom = (char*)fichierMdT.c_str();
		XMLError err =doc.LoadFile(nom);

		if(err==0){
			//cout << " dans if de create Automate"<< endl;

			
			while(!LesTaches.eof()){
	
				//pour une tache
				buffer >> tache;
	
	
				//recherche de la description xml de la tache
				racine = doc.RootElement();
				cout << " racine trouve"<< endl;
				//doc.close();
				
				//cas de la racine de l'arbre				
				Fils1=racine->FirstChildElement("task");
				ordonnancement= Fils1->FirstChildElement("task-decomposition");
				printf( "le noeud est  %s\n", ordonnancement->GetText());
				
				recherche_recursif(Fils1, fichierMdT, tache, fichierNomTaches);
				//cout << " tache trouvee"<< endl;
				//aiguillage vers la bonne fonction de traduction en automate
				//ordonnancement= tacheXML->FirstChildElement("task-decomposition");
				//printf( "le noeud est  %s\n", ordonnancement->GetText());
				
				//on passe à la tache suivante
				getline(LesTaches, ligne);
				buffer.str("");
				buffer << ligne;
	
			}			
		}
		else{
			printf( "problème chargement du fichier XML\n");
		}
	}
	
	else{
		printf( "problème chargement du fichier txt\n");
	}
	
}



//fichier en entree = fichier kxml
int main(int argc, char *argv[]){	

	//string nomFichierKXML= argv[1];
	string nomFichierSymbol= "Out/label.syms";//pour le moment imposé
	string nomFichierNomTaches= "Out/nomTaches.txt";//pour le moment imposé
	//string nomFichierMdT= "templateSortieSki.kxml";
	//char* nom;
	//nom = (char*)nomFichierMdT.c_str();
	XMLDocument doc;
	
	
	XMLError err =doc.LoadFile(argv[1]);
	//ofstream TableSymbols(nomFichierSymbol);
	
	
	if(err==0){
		
		//Navigation dans les noeuds
		//printf( "recherche du premier element\n");
	
	
		XMLElement* racine = doc.RootElement();
		//printf( "racine trouvée\n");
		//
		//
		
		
		
		
		//
		//
		//traitement de la racine (pour creer son automate)
		
		//
		//
		
		
		
		affiche_recursif(racine, nomFichierNomTaches);
		
		
	}
	
	else{
		printf( "problème chargement\n");
	}

	
	create_SymbolTable(nomFichierNomTaches, nomFichierSymbol);
	//doc.SaveFile( "test.xml" );
	
	create_Automates(nomFichierNomTaches, argv[1]);
	
	
	//pour le fichier syms on peut garder les .
	
	return 0;
}

