#include "Sommet.h"

Sommet::Sommet(int num)
{
    m_num=num;
    //ctor
}

void Sommet::afficher() const
 {
        std::cout<<"     sommet "<<m_num<<" : \n";
        for (auto s : m_successeurs)
        {
            std::cout<<"\nDistance du sommet " << s.first->getNum()<<" : ";
            std::cout<<s.second<<" minutes";
        }
}

Sommet::~Sommet()
{
    //dtor
}
