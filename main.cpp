#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <fstream>
#include <conio.h>
#include <windows.h>

#define valeur_max 9999

#include "Graphe.h"

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

    std::string c;
    bool passage = false;
    Dessin d;
    d.dessin_menu();

   do{
    d.gotoLigCol(18,4);
    std::cout << "     BORNE D'INFORMATION - LES ARCS";
    d.gotoLigCol(20,4);
    std::cout<<"  1. Affichage du chargement fichier";
    d.gotoLigCol(22,4);
    std::cout<<"  2. Information sur un trajet ";
    d.gotoLigCol(24,4);
    std::cout<<"  3. Information sur un point de la station ";
    d.gotoLigCol(26,4);
    std::cout<<"  4. Tous les plus courts chemins a partir d'un point ";
    d.gotoLigCol(28,4);
    std::cout<<"  5. Chemin le plus rapide entre deux points ";
    d.gotoLigCol(30,4);
    std::cout<<"  6. Preferences ";
    d.gotoLigCol(32,4);
    std::cout<<"  7. Team ski de fond :slight_smile: ";
    d.gotoLigCol(34,4);
    std::cout<<"  8. Espace utilisateur ";
    d.gotoLigCol(36,4);
    std::cout<<"  9. chemin avec le moins de monde ";
    d.gotoLigCol(38,4);
    std::cout<<" 10. Quitter ";

    d.gotoLigCol(40,7);
    std::cout<<"     VOTRE CHOIX : ";
    std::cin >> c;
    if((c == "1") ||(c == "2") ||(c == "3") ||(c == "4") ||(c == "5") ||(c == "6") ||(c == "7") ||(c == "8") ||(c == "9") ||(c == "10"))
    {
        passage = true;
        break;
    }
    else{std::cout << "\n Choix non valide ! "<<std::endl; Sleep(1000); system("cls");}

  }while(passage == false);

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

    //g.flots(1,7);

      choix = menu();
      int choix_menu = atoi(choix.c_str());

    while(quitter == false)
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
            }

            break;


        }

        case 2:
        {
            system("cls");

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
            system("cls");

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

            bool passage = false;
            //Choix de l'algorithme en fonction des besoins
            do{

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
                break;
            }

            else{std::cin.ignore(); std::cout << "\n   Choisissez une option valide...\n" << std::endl; Sleep(1000);}

            }while(passage == false);

            std::cout<<std::endl<<"\n   Votre point de la station : ";
            std::cin>>num2;

            if (num1 == 1)
            {
                std::vector<int> arbre_BFS=g.BFS(num2);
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

                        g.Dijkstra(num2,vec_sommets[i]->getNum(),g.getPreference(),0);

                    }
                }
            }

            if (num1 == 3)
            {
                int temps;
                std::cout<<"\nVous souhaitez arriver dans combien de temps ?  minutes: ";
                std::cin>>temps;
                for(int i = 0 ; i < (int)vec_sommets.size() ; ++i)
                {
                    if(num2 != vec_sommets[i]->getNum())
                    {

                        //Appel de l'algorithme de Dijkstra
                        g.Dijkstra(num2,vec_sommets[i]->getNum(),g.getPreference(),temps);

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
            bool passage = false;

            do{

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
            else{std::cin.ignore(); std::cout << "\n   Choisissez une option valide...\n" << std::endl; Sleep(1000);}

            }while(passage == false);

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
                g.Dijkstra(depart,arrivee,g.getPreference(),0);
            }

             if (num1 == 3)
            {
                int temps;
                std::cout<<"\nVous souhaitez arriver dans combien de temps ?  minutes: ";
                std::cin>>temps;
                //Appel de l'algorithme de Dijkstra
                g.Dijkstra(depart,arrivee,g.getPreference(),temps);
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
            system("cls");

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
                system("cls");

                g.ski_de_fond();
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

             bool passage = false;
             std::string choix_utilisateur;
            //Choix de l'algorithme en fonction des besoins

            do{

            system("cls");

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

            else{std::cin.ignore(); std::cout << "\n   Choisissez une option valide...\n" << std::endl; Sleep(1000);}

            }while(passage == false);


                if(choix_utilisateur == "1"){g.connexion();}
                else if(choix_utilisateur == "2"){g.deconnexion();}
                else if (choix_utilisateur == "3"){g.sauvegarde();}

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
            system("cls");

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

