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
    /* La construction du r�seau peut se faire � partir d'un fichier
     dont le nom est pass� en param�tre
    Le fichier contient :  ordre, taille, liste des arcs et leurs poids*/
    Graphe(std::string nomFichier);
    /*destructeur*/
    virtual ~Graphe();

    std::vector<Sommet*> getSommets(){return m_sommets;};
    std::vector<Trajet*> getTrajets(){return m_trajets;};
    //Fonction d'affichage de notre chargement de fichier graphe.txt
    void afficher_sommet() const;
    // Fonction d'affichage des aretes
    void afficher_arc() const;
    //Fonction d'affichage arbre

    void Dijkstra(int depart,int arrivee);

    std::vector<int> BFS(int num_S0);
    void afficherParcours(size_t num,const std::vector<int>& arbre);
    double Poids_Chemin_S1_S2(int s1,int s2);
    //void infoTrajet();
    void infoSommet();

    std::vector<Sommet*> Avoir_So2_avec_So1(int s1);

    ///Donne le nom du trajet entre deux sommet donnés
    std::string Nom_Chemin_S1_S2(int s1,int s2);
    ///Donne le type du trajet entre deux sommet donnés
    std::string Type_Chemin_S1_S2(int s1,int s2);
    ///Donne le numero du sommet a partir de son nom
    int Num_du_Sommet_avec_Nom(std::string nom);
    ///Donne le numero du Trajet a partir de son nom
    int Id_du_Trajet_avec_Nom(std::string nom);

    ///Donne les nom des trajets entrants et sortant d'un sommet donné
    void infoTrajet();



};

#endif // GRAPHE_H
