#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <fstream>
#include <conio.h>
#include <windows.h>

#define valeur_max 9999

#include "Graphe.h"

// Fonction qui permet le retour au menu quand on appuie sur Enter apres chaque action
bool fin_tache()
{
    std::cout<< "\nAppuyez sur Enter pour revenir au menu";

    while(getch() != 13)
    {
        Sleep(100);
    }

    return true;
}

std::string menu() // Fonction d'affichage du menu des choix
{
    // Initialisations
    std::string c;
    bool passage = false;
    Dessin d;
    //Créartion de notre graphe/Station
    Graphe g{"data_arcs.txt"};
    //Affichage artistique


    do
    {
        //Affiche artistique du menu (dedicace a emilie)
        d.dessin_menu();
        //On affiche ici si toutes les pistes sont ouvertes (cf. Bonus Touchousse)
    if(g.getTrajets_fermes().size()==0)
    {
        d.gotoLigCol(3,78);
        d.couleur(10);
        std::cout<<"Aucunes pistes ou remontees fermees ";
        d.couleur(15);
    }
    //Si ce n'est pas le cas on affiches les pistes/remontees fermees.
    else if(g.getTrajets_fermes().size()>0)
    {
        d.gotoLigCol(3,78);
        d.couleur(12);
        std::cout<<"Attention pistes/remontees fermees !";

        for(int i=0; i< (int)g.getTrajets_fermes().size(); i++)
        {
            std::string tampon=g.getTrajets_fermes()[i];
            std::string tampon2=g.trajet_avec_son_nom(tampon)->getType();
            d.couleur_type(tampon2);
            d.gotoLigCol(5+i,80);
            std::cout<<"- "<<g.getTrajets_fermes()[i]<<std::endl;
            d.setColor(0);
            d.couleur(15);
        }

    }

        //Affichage du menu des choix utilisateurs Avec blindage total
        d.gotoLigCol(16,4);
        std::cout << "     BORNE D'INFORMATION - LES ARCS";
        d.gotoLigCol(19,4);
        std::cout<<"  1. Affichage du chargement fichier";
        d.gotoLigCol(21,4);
        std::cout<<"  2. Information sur un trajet ";
        d.gotoLigCol(23,4);
        std::cout<<"  3. Information sur un point de la station ";
        d.gotoLigCol(25,4);
        std::cout<<"  4. Tous les plus courts chemins a partir d'un point ";
        d.gotoLigCol(27,4);
        std::cout<<"  5. Chemin le plus rapide entre deux points ";
        d.gotoLigCol(29,4);
        std::cout<<"  6. Preferences ";
        d.gotoLigCol(31,4);
        std::cout<<"  7. Team ski de fond :) ";
        d.gotoLigCol(33,4);
        std::cout<<"  8. Espace utilisateur ";
        d.gotoLigCol(35,4);
        std::cout<<"  9. Chemin avec le moins de skieurs ";
        d.gotoLigCol(37,4);
        std::cout<<" 10. Quitter ";

        d.gotoLigCol(39,7);

        std::cout<<"     VOTRE CHOIX : ";
        std::cin >> c;

        if((c == "1") ||(c == "2") ||(c == "3") ||(c == "4") ||(c == "5") ||(c == "6") ||(c == "7") ||(c == "8") ||(c == "9") ||(c == "10"))
        {
            passage = true;
        }
        else
        {   //Si on ecrit un autre choix que un choix valide = > message d'erreur
            std::cout << "\n Choix non valide ! "<<std::endl;
            Sleep(1000);
            system("cls");
        }

    }
    while(passage == false);

    return c;
}

int main()
{
    //Chargement de notre fichier texte
    Graphe g{"data_arcs.txt"};
    Dessin d;
    //Initialisation variables
    int depart, arrivee,num1, num2;
    std::string choix, num;
    bool retour_menu = false, quitter = false;
    std::vector<Sommet*> vec_sommets = g.getSommets();

    //Appel menu
    choix = menu();
    //Conversion string vers int pour faciliter les blindages de saisies
    int choix_menu = atoi(choix.c_str());

    while(quitter == false)//Tant qu'on ne quitte pas l'application
    {

        switch(choix_menu)
        {
        case 1 :
        {
            system("cls");

            ///Affichage du chargement du graphe.
            g.afficher_sommet();
            g.afficher_arc();
            retour_menu = fin_tache();

            if(retour_menu)
            {
                system("cls");
                choix = menu();
                choix_menu = atoi(choix.c_str());
            }
            break;
        }

        case 2:
        {
            system("cls");
            ///Appel de l'algo pour avoir des informations sur un trajet
            g.infoTrajet();
            retour_menu = fin_tache();
            if(retour_menu)
            {
                system("cls");
                choix = menu();
                choix_menu = atoi(choix.c_str());
            }
            break;
        }

        case 3:
        {
            system("cls");
            ///Appel de l'ago pour avoir des informations sur un point
            g.infoSommet();
            retour_menu = fin_tache();
            if(retour_menu)
            {
                system("cls");
                choix = menu();
                choix_menu = atoi(choix.c_str());
            }

            break;
        }

        case 4:
        {

            bool passage = false;
            //Choix de l'algorithme en fonction des besoins
            do
            { // BLINDAGE

                system("cls");

                std::cout << "\n   1. Chemins les plus courts en nombre de trajets differents" << std::endl;
                std::cout << "   2. Chemins les plus cours en minutes" << std::endl;
                std::cout << "   3. Chemins les plus cours en minutes +CONTRAINTE DE TEMPS" << std::endl;
                std::cout << "\n   Votre choix : ";
                std::cin >> num;

                if((num == "1") || (num == "2") || (num == "3"))
                {
                    num1 = atoi(num.c_str());
                    passage = true;
                }

                else
                {
                    std::cin.ignore();
                    std::cout << "\n   Choisissez une option valide...\n" << std::endl;
                    Sleep(1000);
                }

            }
            while(passage == false);

            std::cout<<std::endl<<"\n   Votre point de la station : ";
            std::cin>>num2;

            if (num1 == 1)
            {   ///APPEL DU BFS
                std::vector<int> arbre_BFS=g.BFS(num2);
                //affichage des chemins obtenus
                std::cout<<std::endl<<std::endl<<"Parcours BFS a partir du sommet "<<num2<<" :\n" << std::endl;
                ///AFFICHAGE DES RESULTATS
                g.afficherParcours(num2,arbre_BFS);

            }

            if (num1 == 2)
            {

                for(int i = 0 ; i < (int)vec_sommets.size() ; ++i)
                {
                    if(num2 != vec_sommets[i]->getNum())
                    {
                        ///APPEL DE MULTIPLE DIJKSTRA
                        g.Dijkstra(num2,vec_sommets[i]->getNum(),0);

                    }
                }
            }

            if (num1 == 3)
            {   //Bonus Contrainte de temps

                double temps;
                std::cout<<"\n   Vous souhaitez arriver dans combien de temps ?" << std::endl;
                std::cout<<"\n   Temps en minutes : ";
                std::cin>>temps;

                for(int i = 0 ; i < (int)vec_sommets.size() ; ++i)
                {
                    if(num2 != vec_sommets[i]->getNum())
                    {

                        //Appel de l'algorithme de Dijkstra
                        g.Dijkstra(num2,vec_sommets[i]->getNum(),temps);

                    }
                }
            }

            retour_menu = fin_tache();
            if(retour_menu)
            {
                system("cls");
                choix = menu();
                choix_menu = atoi(choix.c_str());
            }

            break;
        }

        case 5:
        {
            bool passage = false;

            do
            {

                system("cls");

                //Choix de l'algorithme en fonction des besoins
                std::cout << "\n   1. Itineraire le plus rapide en nombre de trajets differents" << std::endl;
                std::cout << "   2. Itineraire le plus rapide en minutes" << std::endl;
                std::cout << "   3. Chemins les plus cours en minutes +CONTRAINTE DE TEMPS" << std::endl;

                std::cout << "\n   Votre choix : ";
                std::cin >> num;

                if((num == "1") || (num == "2") || (num == "3"))
                {
                    num1 = atoi(num.c_str());
                    passage = true;
                    break;
                }
                else
                {
                    std::cin.ignore();
                    std::cout << "\n   Choisissez une option valide...\n" << std::endl;
                    Sleep(1000);
                }

            }
            while(passage == false);

            std::cout<<"\n Sommet de depart : ";
            std::cin >> depart;
            std::cout<<"\n Sommet d'arrivee : ";
            std::cin >> arrivee;

            if (num1 == 1)
            {
                std::vector<int> arbre_BFS=g.BFS(depart);
                //affichage des chemins obtenus
                std::cout<<std::endl<<std::endl<<"Parcours BFS a partir du sommet "<<depart<<" :\n";
                g.afficher1ParcoursBFS(depart,arrivee,arbre_BFS);
            }

            if (num1 == 2)
            {
                //Appel de l'algorithme de Dijkstra
                g.Dijkstra(depart,arrivee,0);
            }

            if (num1 == 3)
            {
                double temps;
                std::cout<<"\n   Vous souhaitez arriver dans combien de temps ?" << std::endl;
                std::cout<<"\n   Temps en minutes : ";
                std::cin>>temps;
                //Appel de l'algorithme de Dijkstra
                g.Dijkstra(depart,arrivee,temps);
            }

            retour_menu = fin_tache();
            if(retour_menu)
            {
                system("cls");
                choix = menu();
                choix_menu = atoi(choix.c_str());
            }

            break;

        }

        case 6:
        {
            system("cls");

            ///Appel du menu pour les preferences de l'utilisateur
            g.personnaliser();
            retour_menu = fin_tache();
            if(retour_menu)
            {
                system("cls");
                choix = menu();
                choix_menu = atoi(choix.c_str());

            }

            break;

        }

        case 7:
        {
            system("cls");

            ///Bonus Ski de fond avec un kruskal inverse
            g.ski_de_fond();
            retour_menu = fin_tache();
            if(retour_menu)
            {
                system("cls");
                choix = menu();
                choix_menu = atoi(choix.c_str());
            }
            break;
        }

        case 8:
        {

            bool passage = false;
            std::string choix_utilisateur;
            //Choix de l'algorithme en fonction des besoins

            do
            { //Blindage

                system("cls");

                d.skieur_3(6,58);
                d.gotoLigCol(5,8);

                ///Menu espace utilisateur

                std::cout << "\n  ESPACE UTILISATEUR" << std::endl;
                std::cout << "\n  1. Se connecter" << std::endl;
                std::cout << "  2. Se deconnecter" << std::endl;
                std::cout << "  3. Sauvegarder ses preferences" << std::endl;
                std::cout << "\n  Votre choix : ";
                std::cin >> choix_utilisateur;

                if((choix_utilisateur == "1") || (choix_utilisateur == "2") || (choix_utilisateur == "3"))
                {
                    passage = true;
                    break;
                }

                else
                {
                    std::cin.ignore();
                    std::cout << "\n   Choisissez une option valide...\n" << std::endl;
                    Sleep(1000);
                }

            }
            while(passage == false);


            if(choix_utilisateur == "1")
            {
                g.connexion(); ///APPEL FONCTION CONNEXION UTILISATEUR
            }
            else if(choix_utilisateur == "2")
            {
                g.deconnexion(); ///APPEL FONCTION DECONNEXION UTILISATEUR
            }
            else if (choix_utilisateur == "3")
            {
                g.sauvegarde();///APPEL FONCTION SAUVEGARDE UTILISATEUR
            }

            retour_menu = fin_tache();
            if(retour_menu)
            {
                system("cls");
                choix = menu();
                choix_menu = atoi(choix.c_str());
            }
            break;
        }

        case 9:
        {
             ///FLOTS MAXIMUM
            system("cls");



            std::cout << "\n            CHOIX DU CHEMIN AVEC LE MOINS DE SKIEUR " << std::endl<< std::endl<< std::endl;

            std::cout << "      choisissez le numero de votre sommet de depart" << std::endl<< std::endl<<"   votre choix : ";
            std::cin >> depart;
            std::cout  << std::endl << std::endl;


            std::cout << "      choisissez le numero votre sommet d'arrivee" << std::endl<< std::endl<<"   votre choix : ";
            std::cin >> arrivee;

            g.flots(depart, arrivee);

            retour_menu = fin_tache();
            if(retour_menu)
            {
                system("cls");
                choix = menu();
                choix_menu = atoi(choix.c_str());
            }

            break;
        }
        case 10:
        {
            quitter = true; // QUITTER

            std::cout << "\n     A bientot sur notre borne ! Bonne glisse :) !\n" << std::endl;

            break;
        }

        }

    }


    return 0;

}

