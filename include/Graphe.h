#ifndef GRAPHE_H
#define GRAPHE_H

#include<Sommet.h>
#include<Trajet.h>

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

    //Fonction d'affichage de notre chargement de fichier graphe.txt
    void afficher_sommet() const
    {
        std::cout<<"\n    Liste des sommets :\n"<<std::endl;
        for (auto s : m_sommets)
        {
            std::cout  <<s->getNum() << "     " <<s->getNom() << "     " << s->getAltitude() << std::endl;
        }
    }
// Fonction d'affichage des aretes
void afficher_arc() const;



void Dijkstra(int depart,int arrivee);



};

#endif // GRAPHE_H
