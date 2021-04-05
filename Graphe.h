#ifndef GRAPHE_H
#define GRAPHE_H

#include "Trajet.h"

#define valeur_max 9999


class Graphe
{

private :
    ///liste des sommets (vecteur de pointeurs sur Sommet)
    std::vector<Sommet*> m_sommets;
    std::vector<Trajet*> m_trajets;

public :
    /* La construction du réseau peut se faire à partir d'un fichier
     dont le nom est passé en paramètre
    Le fichier contient :  ordre, taille, liste des arcs et leurs poids*/
    Graphe(std::string nomFichier);
    /*destructeur*/
    virtual ~Graphe();

    //Fonction d'affichage de notre chargement de fichier graphe.txt
    void afficher_sommet() const;
    // Fonction d'affichage des aretes
    void afficher_arc() const;
    //Fonction d'affichage arbre

    void Dijkstra(int depart,int arrivee);
    std::vector<int> BFS(int num_S0);
    void afficherParcours(size_t num,const std::vector<int>& arbre);
    std::string Nom_Chemin_S1_S2(int s1,int s2);
    std::string Type_Chemin_S1_S2(int s1,int s2);
    double Poids_Chemin_S1_S2(int s1,int s2);



};

#endif // GRAPHE_H
