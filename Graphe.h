#ifndef GRAPHE_H
#define GRAPHE_H

#include "Trajet.h"
#include <algorithm>
#include <conio.h>

#define valeur_max 9999


class Graphe
{

private :
    ///liste des sommets (vecteur de pointeurs sur Sommet)
    std::vector<Sommet*> m_sommets;
    std::vector<Trajet*> m_trajets;
    std::vector<Sommet*> sauvegarde_sommets;
    std::vector<Trajet*> sauvegarde_trajets;
    std::vector<std::string> m_preference;
    std::vector<std::string> m_preference_initial;

    //std::vector<int> numCC;

public :
    /* La construction du r�seau peut se faire � partir d'un fichier
     dont le nom est pass� en param�tre
    Le fichier contient :  ordre, taille, liste des arcs et leurs poids*/
    Graphe(std::string nomFichier);
    /*destructeur*/
    virtual ~Graphe();

    std::vector<Sommet*> getSommets(){return m_sommets;};
    std::vector<Trajet*> getTrajets(){return m_trajets;};

    std::vector<Sommet*> getSauvegarde_sommets(){return sauvegarde_sommets;};
    std::vector<Trajet*> getSauvegarde_trajets(){return sauvegarde_trajets;};
    //Fonction d'affichage de notre chargement de fichier graphe.txt
    void afficher_sommet() const;
    // Fonction d'affichage des aretes
    void afficher_arc() const;
    //Fonction d'affichage arbre

    void Dijkstra(int depart,int arrivee);

    std::vector<int> BFS(int num_S0,std::vector<std::string> preference);
    void afficherParcours(size_t num,const std::vector<int>& arbre);
    void afficher1ParcoursBFS(size_t num, size_t num2, std::vector<int>& arbre);
    double Poids_Chemin_S1_S2(int s1,int s2);
    //void infoTrajet();
    void infoSommet();

    std::vector<Sommet*> Avoir_So2_avec_So1(int s1);

    std::vector<std::string> getPreference(){return m_preference;}
    void setPreference(std::vector<std::string> preference){m_preference=preference;}
    std::vector<std::string> getPreference_initial(){return m_preference_initial;}
    void setPreference_initial(std::vector<std::string> preference_initial){m_preference_initial=preference_initial;}
    ///Donne le nom du trajet entre deux sommet donnés
    std::string Nom_Chemin_S1_S2(int s1,int s2);
    ///Donne le type du trajet entre deux sommet donnés
    std::string Type_Chemin_S1_S2(int s1,int s2);
    ///Donne le numero du sommet a partir de son nom
    int Num_du_Sommet_avec_Nom(std::string nom);
    ///Donne le numero du Trajet a partir de son nom
    int Id_du_Trajet_avec_Nom(std::string nom);
    ///Donne le nom du Trajet a partir de son numero
    std::string nom_du_Trajet_avec_Id(int num);
    ///Donne l'objet Trajet avec son numero
    Trajet* trajet_avec_son_Id(int id);
    void Effacer_successeur_sommet_quand_efface_trajet(Trajet* trajet_efface);
    Trajet* trajet_avec_ses_succ(Sommet* som1,Sommet* som2);




    ///Donne les nom des trajets entrants et sortant d'un sommet donné
    void infoTrajet();
    void personnaliser();
    void kruskal();


};

#endif // GRAPHE_H
