#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <fstream>
#include <conio.h>
#include <windows.h>

#define valeur_max 9999

#include "Graphe.h"

bool fin_tache()
{
    std::cout<< "\nAppuyez sur Enter pour revenir au menu" << std::endl;

     while(getch() != 13)
     {
         Sleep(100);
     }

     return true;
}

int menu() // Fonction d'affichage du menu des choix
{
    int choix = 0;

    std::cout << "     Borne d'information - les Arcs\n" << std::endl;
    std::cout<<"\n  1. Affichage du chargement fichier" << std::endl;
    std::cout<<"  2. Information sur un trajet " << std::endl;
    std::cout<<"  3. Information sur un point de la station " << std::endl;
    std::cout<<"  4. Tous les plus courts chemins a partir d'un point " << std::endl;
    std::cout<<"  5. Chemin le plus rapide entre deux points " << std::endl;
    std::cout<<"  6. Quitter " << std::endl;

    std::cout<<"\n     Votre choix : ";
    std::cin >> choix;

    return choix;
}

int main()
{
    //Initialisation variables
    int depart, arrivee, num, choix = 0;
    bool retour_menu = false;

    //Chargement de notre fichier texte
    Graphe g{"data_arcs.txt"};

    std::cout << "\n            Projet piscine" << std::endl;

    choix = menu();

    do{

    switch(choix)
    {
    case 1 :
        {
               ///Affichage du chargement du graphe.
               g.afficher_sommet();
               g.afficher_arc();
               retour_menu = fin_tache();
               if(retour_menu){system("cls"); choix = menu();}

        }

    case 2:
        {
               g.infoTrajet();
               retour_menu = fin_tache();
               if(retour_menu){system("cls"); choix = menu();}

        }

    case 3:
        {
               g.infoSommet();
               retour_menu = fin_tache();
               if(retour_menu){system("cls"); choix = menu();}
        }

    case 4:
        {
            //Choix du numéro du sommet initial
            std::cout<<std::endl<<"Votre point de la station : ";
            std::cin>>num;


            std::vector<int> arbre_BFS=g.BFS(num);
            //affichage des chemins obtenus
            std::cout<<std::endl<<std::endl<<"Parcours BFS a partir du sommet "<<num<<" :\n";
            g.afficherParcours(num,arbre_BFS);
            retour_menu = fin_tache();
            if(retour_menu){system("cls"); choix = menu();}
        }

    case 5:
        {
                std::cout<<"\n Sommet de depart : ";
                std::cin >> depart;
                std::cout<<"\n Sommet d'arrivee : ";
                std::cin >> arrivee;

                //Appel de l'algorithme de Dijkstra
                g.Dijkstra(depart,arrivee);
                retour_menu = fin_tache();
                if(retour_menu){system("cls"); choix = menu();}

        }

    case 6:
        {
            return 0; // QUITTER
        }


    }

    }while(choix != 6);

 return 0;

}

