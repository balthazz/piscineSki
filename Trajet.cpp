#include "Trajet.h"

Trajet::Trajet(std::pair<Sommet*,Sommet*> extremites,int id, std::string nom, std::string type,double capacity)
{
    m_extremites=extremites;
    m_id=id;
    m_nom=nom;
    m_type=type;
    m_capacity=capacity;

    if ((m_type == "N") || (m_type == "R") || (m_type == "B") || (m_type == "V") || (m_type == "SURF"))
    {
        m_descente = true;
    }

    else
    {
        m_descente = false;
    }
    //ctor
}

void Trajet::afficher()
{

    std::cout << m_id << "  "<< m_nom << "   " << m_type << "  " <<m_extremites.first->getNum() << " <--> " << m_extremites.second->getNum();
}

double Trajet::calcul_poids()
{
    double altitude_S1_S2;
    altitude_S1_S2 = (m_extremites.second->getAltitude() - m_extremites.first->getAltitude())/100;

    double poids = 0;

    if(altitude_S1_S2 < 0)
    {
        altitude_S1_S2 = altitude_S1_S2*(-1);
    }

    if (m_type=="V")
    {
        poids = 5*altitude_S1_S2;
    }
    else if (m_type=="B")
    {
        poids = 4*altitude_S1_S2;
    }
    else if (m_type=="R")
    {
        poids = 3*altitude_S1_S2;
    }
    else if (m_type=="N")
    {
        poids = 2*altitude_S1_S2;
    }
    else if (m_type=="KL")
    {
        poids = 0.167*altitude_S1_S2;
    }
    else if (m_type=="SURF")
    {
        poids = 10*altitude_S1_S2;
    }
    else if(m_type=="TPH")
    {
        poids = 2*altitude_S1_S2+4;
    }
    else if(m_type=="TC")
    {
        poids = 3*altitude_S1_S2+2;
    }
    else if(m_type=="TSD")
    {
        poids = 3*altitude_S1_S2+1;
    }
    else if(m_type=="TS")
    {
        poids = 4*altitude_S1_S2+1;
    }
    else if(m_type=="TK")
    {
        poids = 4*altitude_S1_S2+1;
    }
    else if(m_type=="BUS")
    {
        if((m_nom=="navette2000-1600") || (m_nom=="navette1600-2000"))
        {
            poids = 40;
        }
        else
        {
            poids = 30;
        }
    }

         return poids;
}

Trajet::~Trajet()
{
    //dtor
}
