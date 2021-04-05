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

Trajet::~Trajet()
{
    //dtor
}
