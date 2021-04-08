#include "Dessin.h"

Dessin::Dessin()
{

}

//couleur texte
void Dessin::couleur(int indice)
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, indice);
}


//couleur fond
void Dessin::setColor(int couleurFond)
{
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, couleurFond * 16);
}


void Dessin::gotoLigCol(int lig, int col)
{
    COORD mycoord;
    mycoord.X = col;
    mycoord.Y = lig;
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), mycoord );
}

void Dessin::cadre()
{
    gotoLigCol(15,0);
    couleur(3);
    std::cout<<"---------------------------------------------------------------------------------------------------------\n";
    for(int i=0;i<40;i++)
    {
        gotoLigCol(16+i,0);
        std::cout<<"||                                                                                                     ||\n";
    }
    gotoLigCol(15+40,0);
    std::cout<<"-------------------------------------------------------------------------------------------------------\n";

}

void Dessin::petit_cadre()
{
    gotoLigCol(3,85);
    couleur(3);
    std::cout<<"--------------------------------\n";
    for(int i=0;i<10;i++)
    {
        gotoLigCol(4+i,85);
        std::cout<<"||                            ||\n";
    }
    gotoLigCol(3+10,85);
    std::cout<<"--------------------------------\n";

}

void Dessin::Dessin_sapin(int lig, int col)
{
    couleur(2);
    gotoLigCol(lig,col);
    std::cout<<"  /\\\     /\\\ "<<std::endl;
    gotoLigCol(lig+1,col);
    std::cout<<" /  \\\   /  \\\ "<<std::endl;
    gotoLigCol(lig+2,col);
    std::cout<<" /  \\\   /  \\\ "<<std::endl;
    gotoLigCol(lig+3,col);
    std::cout<<"/    \\\ /    \\\ "<<std::endl;
    gotoLigCol(lig+4,col);
    std::cout<<" `||`   `||` "<<std::endl;

     gotoLigCol(60,8);

}

void Dessin::fond_Neige()
{
    std::cout<<"      *    *           *.         *        .   .          *              *         *   *     .     .      *     *   .     .    *           *.   *   "<<std::endl;
    std::cout<<"      *    *           *    *           *.                      *           *   .      *     .     .      *     *   .     .    *           *.   *   .    *.  "<<std::endl;
    std::cout<<"              .   .                *              .            * .     *          *                    *   .     .             "<<std::endl;
    std::cout<<" .  *      *         *         *    *         *   .                    *   .     .   *   *     .     .      *     *   .     .    *           *.   *         .   "<<std::endl;
    std::cout<<"    *   .     .    *                    *   *       *              *   .             .                      *   .     .          *   *       *         *   .     .   "<<std::endl;
    std::cout<<"    * .               *         *   .     .   *   .     .   *         *     .     .      *     *   .     .    *           *.   *            *  "<<std::endl;
    std::cout<<"  .       *            *          .     .     *       *       *         *   .     .      *     .     .      *     *   .     .    *           *.   *                *  "<<std::endl;
    std::cout<<"      *    *     *   .                .            *   .     .   *          *     .     .      *     *   .     .    *           *.   *  *   .               .   *.   *           .     .   *      "<<std::endl;
    std::cout<<"     .        .    *                                *.   *    *    *           *.         *        .   .          *              *         *   *     .            .     .   *   *     .     .      *     *   .     .    *           *.   *       "<<std::endl;
    std::cout<<"     *     .     .      *     *   .     .    *           *.   *   .              *     .  *    *           *.         *        .   .          *              *         *   *     .       .      *     *   .     .    *           *.   *     *   .     .     .   *      "<<std::endl;
    std::cout<<"   *       *         *                            *       *         *                    .     .                *    .             * .     * "<<std::endl;
    std::cout<<" .  *       *     *          .     *     .          *   .     .             *                     *         *   .     .    "<<std::endl;
    std::cout<<"      *               *       *                                *   .                     .      .     .                   . "<<std::endl;
    std::cout<<"  *       *         *                     *            *              .            *                   *           *   .     .                  .    "<<std::endl;
    std::cout<<"                       *          *      *       *         *   .     .                *   .     .      *    "<<std::endl;
    std::cout<<"   *     . *.    *       *                                *       .     *     .     .      *     *   .     .    *           *.   *                    .   "<<std::endl;
    std::cout<<" .   *    *           *.         *        .   .          *              *         *   *     .   *      *         *         *    *          *     .     .      *     *   .     .    *           *.   *   *   .                    *   .     .   *        .   "<<std::endl;
    std::cout<<"    *    *           *.         *     *    *           *.         *        .   .          *              *         *   *     .       .   .          *              *         *   *     .    *   .     .    *                    *   *       *              *   .             .                      *   .     .          *   *       *         *   .     .   "<<std::endl;
    std::cout<<"    * .       *    *           *.         *        .   .          *              *         *   *     .            *         *   .     .   *   .     .   *              *     .     .      *     *   .     .    *           *.   *       *  "<<std::endl;
    std::cout<<" .  *      *         *         *    *       *     .     .      *    *           *.         *        .   .          *              *         *   *     .    *     *   .     .    *           *.   *      *   .                    *   .     .   *        .   "<<std::endl;
    std::cout<<"    *   .     .    *                    *   *       *              *   .             .                      *   .     .          *   *       *         *   .     .   "<<std::endl;
    std::cout<<"    * .               *         *   .    *     .     .      *     *   .     .    *           *.   *     .   *   .     .   *                 *  "<<std::endl;
    std::cout<<"  .         *    *           *.         *        .   .          *              *         *   *     .  *   *    *       *    *           *.         *        .   .          *              *         *   *     .        *.         *        .   .          *              *         *   *     .             *          .     .     *       *       *         *   .     .               *     .     .      *     *   .     .    *           *.   *       *  "<<std::endl;
    std::cout<<"      *    *      *    *           *.         *        .   .          *              *         *   *     .   *   .                .            *   .     .   *        *   .   *     .     .      *     *   .     .    *           *.   *                .   *.   *           .     .   *      "<<std::endl;
    std::cout<<"     .        .    *                *    *           *    *           *.         *        .   .          *              *         *   *     .    *.         *        .   .          *              *         *   *     .                    *.   *            .     .   *      *     .     .      *     *   .     .    *           *.   *    "<<std::endl;
    std::cout<<"     *     .   *    *           *.         *        .   .          *              *         *   *     .      .      *     *   .     .    *           *.   *   *     .     .      *     *   .     .    *           *.   *    .               *   .     .     .   *      "<<std::endl;
    std::cout<<"  .       *            *          .     .     *       *       *         *   .     .           *     .     .      *     *   .     .    *           *.   *           *  "<<std::endl;
    std::cout<<"      *    *     *   .                .            *   .     .   *        *   .               .   *.   *           .     .   *      "<<std::endl;
    std::cout<<"     .        .    *                                *.   *            .     .   *       *     .     .      *     *   .     .    *           *.   *   "<<std::endl;
    std::cout<<"     *     .     .      *     *   .     .    *      *     .     .      *     *   .     .    *           *.   *         *.   *   .               *   .     .     .   *      "<<std::endl;

}

void Dessin::couleur_type(std::string texte)
{
    if(texte=="B")
    {
       couleur(9);
    }
    else if(texte=="R")
    {
       couleur(12);
    }
    else if(texte=="N")
    {
       couleur(5);
    }
    else if((texte!="B") && (texte!="R") && (texte!="N") )
    {
       setColor(8);
    }

}

Dessin::~Dessin()
{
    //dtor
}
