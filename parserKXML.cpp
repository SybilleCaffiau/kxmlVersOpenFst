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

//retourne si la tache est iterative
bool isIteration(string num_tache, XMLElement* Noeud, string fichier_XML){
	bool iter=false;
	XMLDocument doc;
	char* f = (char*)fichier_XML.c_str();
	XMLError err =doc.LoadFile(f);
	
	if(err==0){

		//XMLElement* racine = doc.RootElement();
		//XMLElement* noeud_suivant= racine->FirstChildElement("task");
		//XMLElement* noeud_suivant1;
		string num_noeud = Noeud->FirstChildElement("task-numero")->GetText();
		
		
		//cout <<"les taches "<<num_tache<<" et "<<num_noeud<<" sont etudiées"<<endl;
		
		
		if(num_noeud==num_tache){
			
			//cout <<"la tache "<< num_tache<<" est trouvée" << endl;
			const char* value_iter = Noeud->FirstChildElement("task-iteration")->GetText();
			//strcmp retourne 0 si value_iter et [1] sont egaux
			if(strcmp(value_iter,"[1]")==0){
				iter=false;
			}
			else{
				iter=true;
			}
			//cout << "la tache "<<num_tache<<" est iterative ? :"<< iter<<endl;
			return iter;		
		}
		else{
	
			XMLElement* Fils1=Noeud->FirstChildElement("task");
			XMLElement* Frere;
			//cout << "on ne sait pas on passe à une autre "<<endl;
			bool b=false;
	
			while(Fils1){
				Frere=Fils1->NextSiblingElement("task");
				b=isIteration(num_tache, Fils1, fichier_XML);
				if(b){
					return b;
				}
				else{
					Fils1=Frere;
					return(isIteration(num_tache, Fils1, fichier_XML));
				}
			}
			//return(isIteration(num_tache, Fils1, fichier_XML));
		}
			//noeud_suivant1=racine->NextChildElement("task");
			//num_noeud = noeud_suivant1->FirstChildElement("task-numero")->GetText();
			//cout <<"la tache "<< num_noeud<<" est trouvée" << endl;
			//noeud_suivant=noeud_suivant1;
		//}
	}
	
	else{
		printf( "problème chargement\n");
		return false;
	}
 

	//return iter;
}


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

//fonction qui genere un automate (fichier txt) correspondant à la production de Tache (par son identifiant numérique) par ses sous-tâches sequentielles
void create_Automate_TacheSequentielle(XMLElement* Noeud, string FichierTaches, string nom_fichierXML, XMLElement* racine){
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
	
	//trouver tous les enfants (à partir du deuxieme)
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
						//comparaison mot
			if(mot==num_fils){
				//on a un fils on peut ecrire l'aine
				//cout <<"on a trouvé le fils :" <<num_fils << endl;
				
				//cout<<"recherche de l'iteration pour : " <<dernier_fils<<endl ;
				//cout<<"on commence les recheche de : "<<racine->FirstChildElement("task-numero")->GetText()<<endl;
				bool iteration=isIteration(dernier_fils, Noeud, nom_fichierXML);
				//cout <<dernier_fils <<" iteratif seq ?:"<<iteration<<endl;
				
				if(!iteration){
					Automate << state <<" "<< state +1<< " "<<dernier_fils<< " eps"<< endl;
					state ++;
				}
				else{
					Automate << state <<" "<< state<< " "<<dernier_fils<< " eps"<< endl;
				}
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
			//printf( "%s\n", nom);
			
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
	XMLElement* racine;
	//cout << " dans recherche recursif pour la tache"<< nom_tache << endl;
	
	if(err==0){
		racine=doc.RootElement()->FirstChildElement("task");
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
				create_Automate_TacheSequentielle(Noeud, fichierNomTaches,fichierXML, racine);
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



//fonction qui genere un automate (fixhier txt) correspondant à la production de Racine (par son identifiant numérique) par ses sous-tâches sequentielles
void create_Automate_RacineSequentielle(XMLElement* Racine, string FichierTaches, string nom_fichierXML){
	//Attention ne fonctionne pas si la dernière tache est iterative
	//cout<<"dans create_Automate_RacineSequentielle"<<endl;

	XMLElement* nom= Racine->FirstChildElement("task-name");
	//XMLElement* num= Racine->FirstChildElement("task-numero");
	const char* nom_value = nom->GetText();
	const char* num_value = "Racine";
	
	//la racine est la mère
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
	string Nom_Fichier="Out/SeqA_Racine.txt";
	ofstream Automate(Nom_Fichier);
	

	dernier_fils=intToString(num_fraterie);
	num_fraterie++;
	num_fils=intToString(num_fraterie);
	
	if(LesTaches){
		string mot;
		char* nom;


		while(!LesTaches.eof()){
			//cout << "Je suis dans la boucle" <<endl;
			buffer >> mot;
			nom = (char*)mot.c_str();
			//printf( "%s\n", nom);
			
			
			//identifier si le fils est iteratif
			//bool iteration=isIteration(dernier_fils, Racine ,nom_fichierXML);
			//cout <<"iterative?:"<<iteration;
			
			//comparaison mot
			if(mot==num_fils){
				//on a un fils on peut ecrire l'aine
				//cout <<"on a trouvé le fils :" <<num_fils << endl;
				
				//il faut identifier si le fils est iteratif
				bool iteration=isIteration(dernier_fils, Racine, nom_fichierXML);
				//cout <<dernier_fils <<" iteratif ?:"<<iteration<<endl;
				if(!iteration){
					Automate << state <<" "<< state +1<< " "<<dernier_fils<< " eps"<< endl;
					state ++;
				}
				else{
					Automate << state <<" "<< state<< " "<<dernier_fils<< " eps"<< endl;
				}
				dernier_fils=num_fils;
				
				
				//pour le suivant
				num_fraterie++;
				num_fils=intToString(num_fraterie);

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

//à modifier
//fonction qui genere un automate (fixhier txt) correspondant à la production de Tache (par son identifiant numérique) par ses sous-tâches alternatives
void create_Automate_RacineAlternative(XMLElement* Noeud, string FichierTaches){
	XMLElement* nom= Noeud->FirstChildElement("task-name");
	//XMLElement* num= Racine->FirstChildElement("task-numero");
	const char* nom_value = nom->GetText();
	const char* num_value = "Racine";

	
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
	string Nom_Fichier="Out/AltA_Racine.txt";
	ofstream Automate(Nom_Fichier);
	

	dernier_fils=intToString(num_fraterie);
	num_fraterie++;
	num_fils=intToString(num_fraterie);
	
	if(LesTaches){
		string mot;
		char* nom;


		while(!LesTaches.eof()){
			//cout << "Je suis dans la boucle" <<endl;
			buffer >> mot;
			nom = (char*)mot.c_str();
			//printf( "%s\n", nom);
			
			//comparaison mot
			if(mot==num_fils){
				//on a un fils on peut ecrire l'aine
				//cout <<"on a trouvé le fils :" <<num_fils << endl;
				Automate << state <<" "<< state << " "<<dernier_fils<< " "<< num_mere<< endl;
				dernier_fils=num_fils;
				
				//pour le suivant
				num_fraterie++;

				num_fils=intToString(num_fraterie);

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

				//doc.close();
				
				//cas de la racine de l'arbre				
				Fils1=racine->FirstChildElement("task");
				ordonnancement= Fils1->FirstChildElement("task-decomposition");
				string ordo=ordonnancement->GetText();
				
				if(ordo=="ALT"){
					create_Automate_RacineAlternative(Fils1, fichierNomTaches);
				}
				if (ordo=="SEQ"){
					create_Automate_RacineSequentielle(Fils1, fichierNomTaches, fichierMdT);
				}
				if(ordo=="PAR"){//attention on choisit dans cette version de traiter le parallèlisme comme du alt
					create_Automate_RacineAlternative(Fils1, fichierNomTaches);
				}
				else{//reste le cas des parallèles et sans ordre
				}
				
				
				
				recherche_recursif(Fils1, fichierMdT, tache, fichierNomTaches);

				
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

