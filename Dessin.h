#ifndef DESSIN_H_INCLUDED
#define DESSIN_H_INCLUDED

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <algorithm>
#include <conio.h>

class Dessin
{
private:

public:
    Dessin();
    virtual ~Dessin();

    void fond_Neige();
    void Dessin_skieur();
    void Dessin_sapin(int lig,int col);
    void couleur(int indice);
    void setColor(int couleurFond);
    void gotoLigCol(int lig, int col);
    void cadre();
    void petit_cadre();

    void couleur_type(std::string texte);


};

#endif // DESSIN_H_INCLUDED
