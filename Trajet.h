#ifndef TRAJET_H
#define TRAJET_H

#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <fstream>
#include "Sommet.h"

//Définition de la classe arete
class Trajet{

  private:
      std::pair<Sommet*,Sommet*> m_extremites;
      int m_id;
      std::string m_nom;
      std::string m_type;
      double m_poids;
      bool m_marque;

  public:
      //Constructeur
      Trajet(std::pair<Sommet*,Sommet*> extremites,int id, std::string nom, std::string type);

      virtual ~Trajet();

    //Accesseurs
    void setPoids(double poids)
    {m_poids = poids;}

    double getPoids()
    {return m_poids;}

    void setMarque(double marque)
    {m_marque = marque;}

    double getMarque()
    {return m_marque;}

    void setExtremites(std::pair<Sommet*,Sommet*> extremites)
    {m_extremites = extremites;}

    std::pair<Sommet*,Sommet*> getExtremites()
    {return m_extremites;}

    int getId()
    {return m_id;}

    void setId(int id)
    {m_id = id;}

    std::string getNom()
    {return m_nom;}

    void setNom(std::string nom)
    {m_nom = nom;}

    std::string getType()
    {return m_type;}

    void setType(std::string type)
    {m_type = type;}

    void afficher();

    void calcul_poids();

};

#endif // TRAJET_H
