#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <fstream>

#define valeur_max 9999

#include "Graphe.h"


int main()
{
    int depart, arrivee;
    Graphe g{"data_arcs.txt"};

    ///Affichage du chargement du graphe.
    g.afficher_sommet();
    g.afficher_arc();

    std::cout << "\n            Projet Piscine" << std::endl;

//    g.infoTrajet();
    g.infoSommet();

//    std::cout<<"\n Sommet de depart : ";
//    std::cin >> depart;
//    std::cout<<"\n Sommet d'arrivee : ";
//    std::cin >> arrivee;

    //Appel de l'algorithme de Dijkstra
    //g.Dijkstra(depart,arrivee);

    return 0;

}
