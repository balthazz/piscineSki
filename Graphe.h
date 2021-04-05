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
    void Dijkstra(int depart,int arrivee);
    void infoTrajet();
    void infoSommet();



};

#endif // GRAPHE_H
