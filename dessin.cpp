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

void Dessin::les_arcs(int lig,int col)
{
    couleur(15);
    gotoLigCol(lig,col);
    std::cout<<" _       _____   _____            ___   _____    _____   _____  \n";
    gotoLigCol(lig+1,col);
    std::cout<<"| |     | ____| /  ___/          /   | |  _  \\\  /  ___| /  ___/ \n";
    gotoLigCol(lig+2,col);
    std::cout<<"| |     | |__   | |___          / /| | | |_| |  | |     | |___  \n";
    gotoLigCol(lig+3,col);
    std::cout<<"| |     |  __|  \\\___  \\\        / / | | |  _  /  | |     \\\___  \\\ \n";
    gotoLigCol(lig+4,col);
    std::cout<<"| |___  | |___   ___| |       / /  | | | | \\\ \\\  | |___   ___| | \n";
    gotoLigCol(lig+5,col);
    std::cout<<"|_____| |_____| /_____/      /_/   |_| |_|  \\\_\\\ \\\_____| /_____/ \n";


}

void Dessin::cadre(int lig,int col)
{
    gotoLigCol(lig,col);
    couleur(3);
    std::cout<<"---------------------------------------------------------------------------------------------------------\n";
    for(int i=0; i<40; i++)
    {
        gotoLigCol(lig+1+i,col);
        std::cout<<"||                                                                                                     ||\n";
    }
    gotoLigCol(lig+40,col);
    std::cout<<"-------------------------------------------------------------------------------------------------------\n";

}

void Dessin::petit_cadre(int lig,int col)
{
    gotoLigCol(lig,col);
    couleur(3);
    std::cout<<"-----------------------------------------\n";
    for(int i=0; i<10; i++)
    {
        gotoLigCol(lig+1+i,col);
        std::cout<<"||                                     ||\n";
    }
    gotoLigCol(lig+10,col);
    std::cout<<"-----------------------------------------\n";

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


void Dessin::petit_skieur(int lig,int col)
{
    couleur(15);
    gotoLigCol(lig,col);
    std::cout<<" ,_ o\n";
    gotoLigCol(lig+1,col);
    std::cout<<"/ //\\\,\n";
    gotoLigCol(lig+2,col);
    std::cout<<" \\\>> |\n";
    gotoLigCol(lig+3,col);
    std::cout<<"   \\\\,,\n";
}

void Dessin::petit_skieur3(int lig,int col)
{
    couleur(15);
    gotoLigCol(lig,col);
    std::cout<<"   o _, \n";
    gotoLigCol(lig+1,col);
    std::cout<<"  ./\\\\\ \\\n";
    gotoLigCol(lig+2,col);
    std::cout<<"  | <</\n";
    gotoLigCol(lig+3,col);
    std::cout<<" ..//\n";
}

void Dessin::petit_skieur2(int lig,int col)
{
    gotoLigCol(lig,col);
    std::cout<<"   X__O\n";
    gotoLigCol(lig+1,col);
    std::cout<<" \\\ (\\\(\n";
    gotoLigCol(lig+2,col);
    std::cout<<"  \\\/  `\n";
    gotoLigCol(lig+3,col);
    std::cout<<"    \\\,\n";

}




void Dessin::skieur_2(int lig,int col)
{
    couleur(15);
    gotoLigCol(lig,col);
    std::cout<<"                /----|       .         .\n";
    gotoLigCol(lig+1,col);
    std::cout<<"  .            /     [   .        .         .\n";
    gotoLigCol(lig+2,col);
    std::cout<<"         ______|---- _|__     .        .\n";
    gotoLigCol(lig+3,col);
    std::cout<<".     _--    --\\\_<\\\_//   \\\-----           .\n";
    gotoLigCol(lig+4,col);
    std::cout<<"     _  _--___   \\\__/     ___  -----_ **     *\n";
    gotoLigCol(lig+5,col);
    std::cout<<"*  _- _-      --_         /  [ ----__  --_  *\n";
    gotoLigCol(lig+6,col);
    std::cout<<"*/__-      .    [           _[  *** --_  [*\n";
    gotoLigCol(lig+7,col);
    std::cout<<"  [*/ .          __[/-----__/   [**     [*/\n";
    gotoLigCol(lig+8,col);
    std::cout<<"        .     /--  /            /\n";
    gotoLigCol(lig+9,col);
    std::cout<<"     .        /   /   /[----___/        .\n";
    gotoLigCol(lig+10,col);
    std::cout<<"             /   /*[  !   /==/              .\n";
    gotoLigCol(lig+11,col);
    std::cout<<"  .         /   /==[   |/==/      .\n";
    gotoLigCol(lig+12,col);
    std::cout<<"          _/   /=/ | _ |=/   .               .\n";
    gotoLigCol(lig+13,col);
    std::cout<<"         /_   //  / _ _//              .\n";
    gotoLigCol(lig+14,col);
    std::cout<<" .       [ '//    |__//    .    .            .\n";
    gotoLigCol(lig+15,col);
    std::cout<<"        /==/  .  /==/                .\n";
    gotoLigCol(lig+16,col);
    std::cout<<"      /==/     /==/                       .\n";
    gotoLigCol(lig+17,col);
    std::cout<<"    /==/     /==/       .       .    .\n";
    gotoLigCol(lig+18,col);
    std::cout<<" _/==/    _/==/            .\n";
    gotoLigCol(lig+19,col);
    std::cout<<" [|*      [|*                   \n";
    gotoLigCol(0,0);

}

void Dessin::skieur_3(int lig,int col)
{
    gotoLigCol(lig,col);
    std::cout<<"                                          .-.\n";
    gotoLigCol(lig+1,col);
    std::cout<<"                                        .'   `.\n";
    gotoLigCol(lig+2,col);
    std::cout<<"                                       /       \\n";
    gotoLigCol(lig+3,col);
    std::cout<<"                                       |         |\n";
    gotoLigCol(lig+4,col);
    std::cout<<"                                ___    /__.------.\n";
    gotoLigCol(lig+5,col);
    std::cout<<"                             .-'   `-./   |###_##|\n";
    gotoLigCol(lig+6,col);
    std::cout<<"                           .'        |..  `--',`-'\n";
    gotoLigCol(lig+7,col);
    std::cout<<"                         .'          |\         /\n";
    gotoLigCol(lig+8,col);
    std::cout<<"                        /           /  \ . `._.'.\n";
    gotoLigCol(lig+9,col);
    std::cout<<"                       /          .'    \   .    `.\n";
    gotoLigCol(lig+10,col);
    std::cout<<"                      /        ..'       `._)`.   |\n";
    gotoLigCol(lig+11,col);
    std::cout<<"                     |   .  /_.'              |   |\n";
    gotoLigCol(lig+12,col);
    std::cout<<"                    /     ./`.                |   |\n";
    gotoLigCol(lig+13,col);
    std::cout<<"                    |     /   `.              |  /\n";
    gotoLigCol(lig+14,col);
    std::cout<<"                   /     /      `-.           / /\n";
    gotoLigCol(lig+15,col);
    std::cout<<"                  /`.   /`.       .`-._      / /\n";
    gotoLigCol(lig+16,col);
    std::cout<<"                 /   `./   `-.         `--._/ /||\\n";
    gotoLigCol(lig+17,col);
    std::cout<<"                 |    /       `-.          / / || \\n";
    gotoLigCol(lig+18,col);
    std::cout<<"                 | / /           `-.__    / /  ||  |  ..\n";
    gotoLigCol(lig+19,col);
    std::cout<<"                  `./                 `--/ /  //   |  ||\\n";
    gotoLigCol(lig+20,col);
    std::cout<<"                    |                   / /  //   /   || |\n";
    gotoLigCol(lig+21,col);
    std::cout<<"                    |                  / /  //   /   //  /\n";
    gotoLigCol(lig+22,col);
    std::cout<<"                   /          //       |/  //   /   //  /\n";
    gotoLigCol(lig+23,col);
    std::cout<<"                  /          //        /  //   /   //  /\n";
    gotoLigCol(lig+24,col);
    std::cout<<"                  |         /         /  //   /   //  /\n";
    gotoLigCol(lig+25,col);
    std::cout<<"                 /         /         |  //   /   //  /\n";
    gotoLigCol(lig+26,col);
    std::cout<<"                 |         |         | //   /   //  /\n";
    gotoLigCol(lig+27,col);
    std::cout<<"                 |        /         / //   /   //  /\n";
    gotoLigCol(lig+28,col);
    std::cout<<"                 |        |        | //   /   //  /\n";
    gotoLigCol(lig+29,col);
    std::cout<<"                / \\\       |        |//   /   //  /\n";
    gotoLigCol(lig+30,col);
    std::cout<<"               |          |        //   /   //  /\n";
    gotoLigCol(lig+31,col);
    std::cout<<"               | /       /|       //   /   //  /\n";
    gotoLigCol(lig+32,col);
    std::cout<<"              /         | |      //   /   //  /\n";
    gotoLigCol(lig+33,col);
    std::cout<<"              |      |  | |     //   /   //  /\n";
    gotoLigCol(lig+34,col);
    std::cout<<"              |     \\\ / / \\\   //   /   //  /\n";
    gotoLigCol(lig+35,col);
    std::cout<<"              |       |  |    //   /   //  /\n";
    gotoLigCol(lig+36,col);
    std::cout<<"              |       |  |   //   /   //  /\n";
    gotoLigCol(lig+37,col);
    std::cout<<"              | |    /   |  //   /   //  /\n";
    gotoLigCol(lig+38,col);
    std::cout<<"              | |   |    | //   /   //  /\n";
    gotoLigCol(lig+39,col);
    std::cout<<"              |     |    |//   /|  //  /\n";
    gotoLigCol(lig+40,col);
    std::cout<<"              | |   |    //   / | //  /\n";
    gotoLigCol(lig+41,col);
    std::cout<<"              | |   |   //   / / //  /\n";
    gotoLigCol(lig+42,col);
    std::cout<<"              |/   /   //   /==_//  /\n";
    gotoLigCol(lig+43,col);
    std::cout<<"              | .=(_.-//   /  .//  /\n";
    gotoLigCol(lig+44,col);
    std::cout<<"              |/     //   /   //  / \n";
    gotoLigCol(lig+45,col);
    std::cout<<"              |     //   /   //  / \n";
    gotoLigCol(lig+46,col);
    std::cout<<"              \. --//   /  _//  /\n";
    gotoLigCol(lig+47,col);
    std::cout<<"               \__//   / \_//  /\n";
    gotoLigCol(lig+48,col);
    std::cout<<"                 //   /   //  /\n";
    gotoLigCol(lig+49,col);
    std::cout<<"                //   /   //  /\n";
    gotoLigCol(lig+50,col);
    std::cout<<"               //   /   //  /\n";
    gotoLigCol(lig+51,col);
    std::cout<<"              //   /   //  /\n";
    gotoLigCol(lig+52,col);
    std::cout<<"             //   /   //  /\n";
    gotoLigCol(lig+53,col);
    std::cout<<"            //   /   //  /\n";
    gotoLigCol(lig+54,col);
    std::cout<<"           //   /   //  /\n";
    gotoLigCol(lig+55,col);
    std::cout<<"         //   /   //  /\n";
    gotoLigCol(lig+56,col);
    std::cout<<"        //   /   //  /\n";
    gotoLigCol(lig+57,col);
    std::cout<<"    //   /   //  /\n";
    gotoLigCol(lig+58,col);
    std::cout<<"  `/___/   `/__/\n";
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
    else if(texte == "V")
    {
        couleur(10);
    }
    else if((texte!="B") && (texte!="R") && (texte!="N") )
    {
        setColor(8);
    }

}

void Dessin::dessin_menu()
{
    fond_Neige();
    cadre(13,0);
    petit_cadre(2,75);
    Dessin_sapin(9,13);
    les_arcs(2,1);
    petit_skieur(15,65);
    petit_skieur(47,85);
    petit_skieur3(35,68);
    petit_skieur2(22,85);
}

void Dessin::utilisateur()
{

}

Dessin::~Dessin()
{
    //dtor
}
