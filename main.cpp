#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <fstream>
#include <conio.h>
#include <windows.h>

#define valeur_max 9999


#include "Graphe.h"


void afficherTemps(int time)
{
    int minute=0, heure=0;
    heure = time/60;
    minute = time%60;
    std::cout <<heure<<"h "<<minute<<"minutes "<< std::endl;
}

bool fin_tache()
{
    std::cout<< "\nAppuyez sur Enter pour revenir au menu";

    while(getch() != 13)
    {
        Sleep(100);
    }

    return true;
}

int menu() // Fonction d'affichage du menu des choix
{
    /*int temps;
    std::cout<<"horloge ";
    std::cin >> temps;
    afficherTemps(temps); */

    int choix_menu;

    std::cout << "\n     Borne d'information - les Arcs" << std::endl;
    std::cout<<"\n  1. Affichage du chargement fichier" << std::endl;
    std::cout<<"  2. Information sur un trajet " << std::endl;
    std::cout<<"  3. Information sur un point de la station " << std::endl;
    std::cout<<"  4. Tous les plus courts chemins a partir d'un point " << std::endl;
    std::cout<<"  5. Chemin le plus rapide entre deux points " << std::endl;
    std::cout<<"  6. Preferences " << std::endl;
    std::cout<<"  7. Team ski de fond :) " << std::endl;
    std::cout<<"  8. Espace utilisateur " << std::endl;
    std::cout<<"  9. chemin avec le moins de monde " << std::endl;
    std::cout<<" 10. Quitter " << std::endl;

    std::cout<<"\n     Votre choix : ";
    std::cin >> choix_menu;

    return choix_menu;
}

int main()
{
    //Chargement de notre fichier texte
    Graphe g{"data_arcs.txt"};
    Dessin d;
    //d.fond_Neige();
    //d.cadre();
    //d.petit_cadre();
    //d.Dessin_sapin(11,13);
    //system("PAUSE");
    //d.gotoLigCol(9,0);
    //Initialisation variables
    int depart, arrivee, num1, num2, choix = 0;
    bool retour_menu = false, quitter = false;
    std::vector<Sommet*> vec_sommets = g.getSommets();

    std::cout << "\n            Projet piscine" << std::endl;

//    g.flots(9, 12);


    choix = menu();

    while(quitter == false)
    {

        switch(choix)
        {
        case 1 :
        {
            ///Affichage du chargement du graphe.
            g.afficher_sommet();
            g.afficher_arc();
            retour_menu = fin_tache();
            if(retour_menu)
            {
                system("cls");
                choix = menu();
            }

            break;


        }

        case 2:
        {
            g.infoTrajet();
            retour_menu = fin_tache();
            if(retour_menu)
            {
                system("cls");
                choix = menu();
            }

            break;

        }

        case 3:
        {
            g.infoSommet();
            retour_menu = fin_tache();
            if(retour_menu)
            {
                system("cls");
                choix = menu();
            }

            break;
        }

        case 4:
        {
            //Choix de l'algorithme en fonction des besoins
            std::cout << "\n   1. Chemins les plus courts en nombre de trajets differents" << std::endl;
            std::cout << "   2. Chemins les plus cours en minutes" << std::endl;
            std::cout << "\n   Votre choix : ";
            std::cin >> num1;

            std::cout<<std::endl<<"\n   Votre point de la station : ";
            std::cin>>num2;

            if (num1 == 1)
            {
                std::vector<int> arbre_BFS=g.BFS(num2,g.getPreference());
                //affichage des chemins obtenus
                std::cout<<std::endl<<std::endl<<"Parcours BFS a partir du sommet "<<num2<<" :\n" << std::endl;
                g.afficherParcours(num2,arbre_BFS);

            }

            if (num1 == 2)
            {

                for(int i = 0 ; i < (int)vec_sommets.size() ; ++i)
                {
                    if(num2 != vec_sommets[i]->getNum())
                    {

                        g.Dijkstra(num2,vec_sommets[i]->getNum(),g.getPreference());

                    }
                }
            }

            retour_menu = fin_tache();
            if(retour_menu)
            {
                system("cls");
                choix = menu();
            }

            break;
        }

        case 5:
        {
            //Choix de l'algorithme en fonction des besoins
            std::cout << "\n   1. Itineraire le plus rapide en nombre de trajets differents" << std::endl;
            std::cout << "   2. Itineraire le plus rapide en minutes" << std::endl;
            std::cout << "\n   Votre choix : ";
            std::cin >> num1;

            std::cout<<"\n Sommet de depart : ";
            std::cin >> depart;
            std::cout<<"\n Sommet d'arrivee : ";
            std::cin >> arrivee;

            if (num1 == 1)
            {
                std::vector<int> arbre_BFS=g.BFS(depart,g.getPreference());
                //affichage des chemins obtenus
                std::cout<<std::endl<<std::endl<<"Parcours BFS a partir du sommet "<<depart<<" :\n";
                g.afficher1ParcoursBFS(depart,arrivee,arbre_BFS);
            }

            if (num1 == 2)
            {
                //Appel de l'algorithme de Dijkstra
                g.Dijkstra(depart,arrivee,g.getPreference());
            }

            retour_menu = fin_tache();
            if(retour_menu)
            {
                system("cls");
                choix = menu();
            }

            break;

        }

        case 6:
        {
            g.personnaliser();
            retour_menu = fin_tache();
            if(retour_menu)
            {
                system("cls");
                choix = menu();
            }

            break;

        }

        case 7:
            {
                g.kruskal();
                retour_menu = fin_tache();
                if(retour_menu)
                {
                system("cls");
                choix = menu();
                }
                break;
            }

        case 8:
            {
                int choix_utilisateur;
                std::cout << "\n  ESPACE UTILISATEUR" << std::endl;
                std::cout << "\n  1. Se connecter" << std::endl;
                std::cout << "  2. Se deconnecter" << std::endl;
                std::cout << "  3. Sauvegarder ses preferences" << std::endl;
                std::cout << "\n  Votre choix : ";
                std::cin >> choix_utilisateur;

                if(choix_utilisateur == 1){g.connexion();}
                else if(choix_utilisateur == 2){g.deconnexion();}
                else if (choix_utilisateur == 3){g.sauvegarde();}

                retour_menu = fin_tache();
                if(retour_menu)
                {
                system("cls");
                choix = menu();
                }
                break;
            }

        case 9:
        {
            std::cout << "choisissez le numero de votre sommet de depart" << std::endl;
            std::cin >> depart;


            std::cout << "choisissez le numero votre sommet d'arrivee" << std::endl;
            std::cin >> arrivee;

            g.flots(depart, arrivee);

            retour_menu = fin_tache();
            if(retour_menu)
            {
                system("cls");
                choix = menu();
            }

            break;
        }
        case 10:
        {
            quitter = true; // QUITTER

            std::cout << "\n A bientot sur notre borne ! Bonne glisse :) !\n" << std::endl;

            break;
        }

        }

    }


    return 0;

}

