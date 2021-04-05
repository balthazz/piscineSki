#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <fstream>

#define valeur_max 9999

#include "Graphe.h"


int main()
{
    int depart, arrivee, num;
    Graphe g{"data_arcs.txt"};

    ///Affichage du chargement du graphe.
    g.afficher_sommet();
    g.afficher_arc();

//    std::cout << "\n            Projet Piscine" << std::endl;
//    std::cout<<"\n Sommet de depart : ";
//    std::cin >> depart;
//    std::cout<<"\n Sommet d'arrivee : ";
//    std::cin >> arrivee;
//
//    Appel de l'algorithme de Dijkstra
//    g.Dijkstra(depart,arrivee);

   //Choix du numéro du sommet initial
    std::cout<<std::endl<<"Numero du sommet initial : ";
    std::cin>>num;


    std::vector<int> arbre_BFS=g.BFS(num-1);
    //affichage des chemins obtenus
    std::cout<<std::endl<<std::endl<<"Parcours BFS a partir du sommet "<<num<<" :\n";
    g.afficherParcours(num,arbre_BFS);



    return 0;

}
