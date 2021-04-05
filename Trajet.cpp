#include "Trajet.h"

Trajet::Trajet(std::pair<Sommet*,Sommet*> extremites,int id, std::string nom, std::string type)
{
    m_extremites=extremites;
    m_id=id;
    m_nom=nom;
    m_type=type;
    //ctor
}

void Trajet::afficher()
{

    std::cout << m_id << "  "<< m_nom << "   " << m_type << "  " <<m_extremites.first->getNum() << " <--> " << m_extremites.second->getNum();
}

void Trajet::calcul_poids()
{
    double altitude_S1_S2;
    altitude_S1_S2 = (m_extremites.second->getAltitude() - m_extremites.first->getAltitude())/100;

    if(altitude_S1_S2 < 0)
    {
        altitude_S1_S2 = altitude_S1_S2*(-1);
    }

    if (m_type=="V")
    {
        m_poids=5*altitude_S1_S2;
    }
    else if (m_type=="B")
    {
        m_poids=4*altitude_S1_S2;
    }
    else if (m_type=="R")
    {
        m_poids=3*altitude_S1_S2;
    }
    else if (m_type=="N")
    {
        m_poids=2*altitude_S1_S2;
    }
    else if (m_type=="KL")
    {
        m_poids=(1/60)*altitude_S1_S2;
    }
    else if (m_type=="SURF")
    {
        m_poids=10*altitude_S1_S2;
    }
    else if(m_type=="TPH")
    {
        m_poids=2*altitude_S1_S2+4;
    }
    else if(m_type=="TC")
    {
        m_poids=3*altitude_S1_S2+2;
    }
    else if(m_type=="TSD")
    {
        m_poids=3*altitude_S1_S2+1;
    }
    else if(m_type=="TS")
    {
        m_poids=4*altitude_S1_S2+1;
    }
    else if(m_type=="TPH")
    {
        m_poids=4*altitude_S1_S2+1;
    }
    else if(m_type=="BUS")
    {
        if(m_nom=="navette2000-1600" ||m_nom=="navette1600-2000")
        {
            m_poids=40;
        }
        else
        {
            m_poids=30;
        }
    }


}

Trajet::~Trajet()
{
    //dtor
}
