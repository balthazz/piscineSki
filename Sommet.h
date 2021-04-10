#ifndef SOMMET_H
#define SOMMET_H

#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <fstream>


class Sommet
{
private :
    int m_num;
    std::string m_nom;
    double m_altitude;
    bool m_marque = false;
    double m_distance;
    int m_taille_BFS;
    int m_numCC;
    ///chaque sommet possède la liste de ses successeurs et de ses prédecesseurs(des vecteurs de pointeurs sur Sommet)
    std::vector<std::pair<Sommet*,double>> m_predecesseurs;
    std::vector<std::pair<Sommet*,double>> m_successeurs;

public :
    /*constructeur*/
    Sommet(int num);
    virtual ~Sommet();
    /*accesseurs*/
    int getNum()const
    {
        return m_num;
    }
    ///accesseur : pour la liste des successeurs
    std::vector<std::pair<Sommet*,double>>& getSuccesseurs()
    {
        return m_successeurs;
    }
    std::vector<std::pair<Sommet*,double>>& getPredecesseurs()
    {
        return m_predecesseurs;
    }
    void setSuccesseur(std::vector<std::pair<Sommet*,double>>& val){m_successeurs=val;}
    void setPrede(std::vector<std::pair<Sommet*,double>>& val){m_predecesseurs=val;}
    int getTaille_BFS(){return m_taille_BFS;}
    void setTaille_BFS(int taille){m_taille_BFS=taille;}
    /* pour ajouter un successeur à la liste*/
    void ajouterSucc(std::pair<Sommet*,double> p)
    {
        m_successeurs.push_back(p);
    }
    void ajouterPred(std::pair<Sommet*,double> p)
    {
        m_predecesseurs.push_back(p);
    }
    void setMarque(bool valeur)
    {
        m_marque = valeur;
    }

    bool getMarque()const
    {
        return m_marque;
    }

    void setDistance(double value)
    {
        m_distance = value;
    }

    double getDistance()const
    {
        return m_distance;
    }
    void setNom(std::string nom)
    {
        m_nom = nom;
    }

    std::string getNom()const
    {
        return m_nom;
    }

    void setAltitude(double altitude)
    {
        m_altitude = altitude;
    }

    double getAltitude()const
    {
        return m_altitude;
    }

    void setNumCC(int numCC)
    {
        m_numCC = numCC;
    }

    int getNumCC()
    {
        return m_numCC;
    }

    /* méthode d'affichage*/

        void afficher() const;

};

#endif // SOMMET_H
