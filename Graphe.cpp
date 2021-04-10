#include "Graphe.h"

Graphe::Graphe(std::string nomFichier)
{
    std::ifstream ifs{nomFichier};
    if (!ifs)
        throw std::runtime_error( "Impossible d'ouvrir en lecture " + nomFichier );
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture orientation du graphe");
    int ordre;
    ifs >> ordre;
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture ordre du graphe");

    int numero_sommet;
    double altitude;
    std::string nom;

    for(int i = 0 ; i < ordre ; i++)
    {
        //On associe chaque sommet a un successeur pair (sommet et poids).
        ifs>>numero_sommet>>nom>>altitude;
        m_sommets.push_back(new Sommet{numero_sommet});
        //numCC.push_back(i);
        m_sommets[i]->setNom(nom);
        m_sommets[i]->setAltitude(altitude);

    }

    int taille;
    ifs >> taille;
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture taille du graphe");


    int id_trajet,sommet_1,sommet_2;
    std::string nom_trajet,type_trajet;
    std::pair<Sommet*,Sommet*> extremites;
    double capacityArc;

    for(int i = 0 ; i < taille ; i++)
    {
        ifs>>id_trajet>>nom_trajet>>type_trajet>>sommet_1>>sommet_2>>capacityArc;

        extremites.first = m_sommets[sommet_1-1];
        extremites.second = m_sommets[sommet_2-1];

        m_trajets.push_back(new Trajet{extremites,id_trajet,nom_trajet,type_trajet,capacityArc});

    }

    double poids;
    std::pair<Sommet*,double> tampon;

    for(auto x : m_trajets)
    {
        poids = x->calcul_poids();
        //std::cout<<poids<<std::endl;
        x->setPoids(poids);
        extremites = x->getExtremites();

        tampon.first = extremites.second;
        tampon.second = poids;

        m_sommets[extremites.first->getNum()-1]->ajouterSucc(tampon);
    }

    sauvegarde_trajets = m_trajets;
    sauvegarde_sommets = m_sommets;

    std::string trajet;

    std::ifstream fichier ("admin.txt");
    if(!fichier)
        {throw std::runtime_error("Impossible d'ouvrir en lecture admin.txt");}

        while(!fichier.eof())
        {
            fichier >> trajet;

            for(int i = 0 ; i < (int)m_trajets.size() ; i++)
            {
                if (m_trajets[i]->getNom() == trajet)
                {
                    m_trajets.erase(m_trajets.begin()+i);
                }
            }
        }

///Liste des successeurs pour verification
//    for(auto y : m_sommets)
//    {
//        y->afficher();
//        std::cout << "\n" << std::endl;
//    }

    m_preference.push_back("V");
    m_preference.push_back("R");
    m_preference.push_back("B");
    m_preference.push_back("N");
    m_preference.push_back("TK");
    m_preference.push_back("SURF");
    m_preference.push_back("TS");
    m_preference.push_back("TPH");
    m_preference.push_back("BUS");
    m_preference.push_back("TC");
    m_preference.push_back("TSD");

    m_preference_initial = m_preference;
}



void Graphe::Dijkstra(int depart,int arrivee,std::vector<std::string> preference,int condition_temps)
{

    ///initialisations
    //initialisation de notre tampon pair utilis� dans l'algo.
    std::pair<Sommet*,double> p;
    std::pair<Sommet*,double> tampon;

    bool comparaison;

    int temps=9999;

    if(condition_temps>0)
    {
        temps=condition_temps;
    }

    //on initialise le vecteur des predecesseurs pour chaque sommet avec la valeur -1.
    std::vector<int> pred(m_sommets.size(),-1);

    //Initialisation marquage des sommets pour les dijkstra multiples
    for(auto x : m_sommets)
    {
        x->setMarque(false);
    }

    //D�claration de la fonction de comparaison utilis�e par la priority_queue
    auto compare = [](std::pair<Sommet*,double> a, std::pair<Sommet*,double> b)
    {
        return b.first->getDistance() < a.first->getDistance();
    };

    //D�claration de la file de priorit�
    std::priority_queue<std::pair<Sommet*,double>, std::vector<std::pair<Sommet*,double>>, decltype(compare)> file(compare);

    //initialise tous les sommets � valeur th�oriquement infinie sauf celui de d�part.
    for (auto s : m_sommets)
    {
        if (s->getNum()== depart)
        {
            s->setDistance(0);
        }
        else
        {
            s->setDistance(valeur_max);
        }
    }

    //Ins�rer le sommet initial dans la file avec une distance nul car on ne bouge pas.
    file.push(std::make_pair(m_sommets[depart-1],0));

    ///Boucle de recherche

    while (!file.empty()) //tant que notre file n'est pas vide on continue.
    {
         p = file.top(); //je consid�re que p est le successeur avec le plus cours chemin, je le choisi donc pour le reste de la boucle
         file.pop();//je le supprime pour ne pas retomber dessus

         if (p.first->getMarque()==false) // si le sommet n'a pas été marqué
            {
              p.first->setMarque(true);

            for (auto succ : p.first->getSuccesseurs()) //pour chaque successeur :
            {

                for(auto pref : preference)
                {
                    if(trajet_avec_ses_succ(p.first,succ.first)->getType() == pref)
                    {
                        comparaison = true;
                        break;
                    }
                    else
                    {
                        comparaison = false;
                    }
                }

                for(auto x : piste_enlevee)
               {
                if(trajet_avec_ses_succ(p.first,succ.first)->getNom() == x)
                {
                    comparaison = false;
                    break;
                }
               }

                if ((succ.first->getMarque()== false) /*&& (comparaison == true)*/) // si le successeur n'a pas �t� marqu�
                {

                    // on calcule le chemin parcourure jusqu'au sommet s, on additionne le poid du chemin jusqu'� ce predecesseur
                    double addition = p.first->getDistance() + succ.second;

                    if (addition < succ.first->getDistance()) //si ce chemin est plus petit que le pr�c�dent chemin enregistr� alors on le remplace
                    {

                        succ.first->setDistance(addition); //on d�finit la nouvelle distance (plus courte) entre le d�part et ce nouveau point(qui est un successeur du sommet qu'on �tudiait)
                        pred[succ.first->getNum()] =  p.first->getNum(); // On sauvegarde le predecesseur pour avoir une trace de notre chemin.
                        file.push(succ); //Enfin, on rajoute notre successeur � la file de priorit�.

                    }
                }
           }
        }
    }

    ///Affichage des r�sultats
    ///Valeur du poids

    std::cout << "\n\n Le chemin le plus court entre le sommet "<< depart <<" et le sommet "<< arrivee <<" est de : ";
    couleur.couleur(6);
    std::cout << m_sommets[arrivee-1]->getDistance();
    couleur.couleur(15);
    std::cout << " minutes ";

    ///si il y a une condition de temps et qu'elle est respectée
    if((m_sommets[arrivee-1]->getDistance()<=temps) && condition_temps!=0)
    {
        std::cout<<"<--";
        couleur.couleur(10);
        std::cout <<" TEMPS SUFFISANT "<< std::endl; //on affiche le plus court chemin entre les deux sommets choisis
        couleur.couleur(15);
    }

    ///si il y a une condition de temps et qu'elle n'est pas respectée
    else if((m_sommets[arrivee-1]->getDistance()>temps) && condition_temps!=0)
    {
        int depassement=m_sommets[arrivee-1]->getDistance()-temps;
        std::cout<<"<--";
        couleur.couleur(4);
        std::cout <<" TEMPS INSUFFISANT  +"<<depassement<<" minutes" <<std::endl; //on affiche le plus court chemin entre les deux sommets choisis
        couleur.setColor(0);
        couleur.couleur(15);
    }
    ///Chemin parcouru



  std::cout << "\n\n" << arrivee;


    int x = arrivee;

    //On parcourt notre vecteurs de predecesseurs.
    for (int p = pred[arrivee]; p != -1; p = pred[p])
    {
            std::cout << " <- ";
            couleur.couleur_type(Type_Chemin_S1_S2(p,x));
            std::cout << Type_Chemin_S1_S2(p,x) << " " << Nom_Chemin_S1_S2(p,x);
            couleur.setColor(0);
            couleur.couleur(15);
            std::cout << " <- " << p;
            x = p;
    }

    std::cout << "\n";




}

/// Parcours BFS
std::vector<int> Graphe::BFS(int num_S0)
{

    std::queue<Sommet*>pile;

    // pour le marquage
    std::vector<int> couleurs((int)m_sommets.size()+1,0);
    //pour noter les pr�d�cesseurs
    std::vector<int> predec((int)m_sommets.size()+1,-1);

    bool comparaison = true;

    //�tape initiale
    pile.push(m_sommets[num_S0-1]);
    couleurs[num_S0]=1;
    Sommet*so;

    //tant que la pile n'est pas vide
    while(!pile.empty())
    {
        so=pile.front();
        pile.pop();

        for(auto succ : so->getSuccesseurs())
        {
            for(auto pref : m_preference)
            {
                if(trajet_avec_ses_succ(so,succ.first)->getType() == pref)
                {
                    comparaison = true;
                    break;
                }
                else
                {
                    comparaison = false;
                }
            }

            for(auto x : piste_enlevee)
            {
                if(trajet_avec_ses_succ(so,succ.first)->getNom() == x)
                {
                    comparaison = false;
                    break;
                }
            }

            if((couleurs[succ.first->getNum()]==0) && (comparaison == true)) //s'il n'est pas marqu� et pas dans preference
            {
                couleurs[succ.first->getNum()]=1; //on le marque
                predec[succ.first->getNum()]= so->getNum();
                pile.push(succ.first);//on le met dans la pile
            }
        }
    }
    return predec;
}


std::string Graphe::Nom_Chemin_S1_S2(int s1,int s2)
{
    std::string resultat;

    for(int i=0; i<(int)m_trajets.size(); i++)
    {
        std::pair<Sommet*,Sommet*> tampon=m_trajets[i]->getExtremites();
        if((tampon.first->getNum()==s1) && (tampon.second->getNum()==s2))
        {
            resultat = m_trajets[i]->getNom();
        }
    }

    return resultat;
}

std::string Graphe::nom_du_Trajet_avec_Id(int num)
{
    std::string resultat;

    for(int i=0; i<(int)m_trajets.size(); i++)
    {
        if(m_trajets[i]->getId()==num)
        {
            std::string tampon=m_trajets[i]->getNom();
            resultat = tampon;
        }
    }

    return resultat;
}

Sommet* Graphe::sommet_avec_son_Id(int num)
{
    Sommet * resultat = NULL;

    for(int i=0; i<(int)m_sommets.size(); i++)
    {
        if(m_sommets[i]->getNum()==num)
        {
            resultat = m_sommets[i];
        }
    }

    return resultat;
}

Trajet* Graphe::trajet_avec_son_Id(int id)
{
    Trajet* tampon = NULL;

    for(int i=0; i<(int)m_trajets.size(); i++)
    {
        if(m_trajets[i]->getId()==id)
        {
            tampon=m_trajets[i];
        }
    }
    return tampon;
}

std::string Graphe::Type_Chemin_S1_S2(int s1,int s2)
{
    std::string resultat;

    for(int i=0; i<(int)m_trajets.size(); i++)
    {
        std::pair<Sommet*,Sommet*> tampon=m_trajets[i]->getExtremites();
        if((tampon.first->getNum()==s1) && (tampon.second->getNum()==s2))
        {
            resultat = m_trajets[i]->getType();
        }
    }
    return resultat;
}


double Graphe::Poids_Chemin_S1_S2(int s1,int s2)
{
    double resultat = 0;

    for(int i=0; i<(int)m_trajets.size(); i++)
    {
        std::pair<Sommet*,Sommet*> tampon=m_trajets[i]->getExtremites();
        if((tampon.first->getNum()==s1) && (tampon.second->getNum()==s2))
        {
            resultat = m_trajets[i]->getPoids();
        }
    }

    return resultat;
}


int Graphe::Num_du_Sommet_avec_Nom(std::string nom)
{
    int resultat = 0;

    for(int i=0; i<(int)m_sommets.size(); i++)
    {
        if(m_sommets[i]->getNom()==nom)
        {
            int tampon=m_sommets[i]->getNum();
            resultat = tampon;
        }
    }

    return resultat;
}


int Graphe::Id_du_Trajet_avec_Nom(std::string nom)
{
    int resultat = 0;

    for(int i=0; i<(int)m_trajets.size(); i++)
    {
        if(m_trajets[i]->getNom()==nom)
        {
            int tampon=m_trajets[i]->getId();
            resultat = tampon;
        }
    }
    return resultat;
}

Trajet* Graphe::trajet_avec_ses_succ(Sommet* som1,Sommet* som2)
{
    Trajet * resultat = NULL;

    for(int i=0; i<(int)m_trajets.size(); i++)
    {
        if((m_trajets[i]->getExtremites().first==som1)&&(m_trajets[i]->getExtremites().second==som2))
        {
            resultat = m_trajets[i];
        }
    }

    return resultat;
}

void Graphe::afficher1ParcoursBFS(size_t num, size_t num2, std::vector<int>& arbre)
{
    double poids = 0;
    std::vector<int> tampon;

    for(size_t i=0; i<arbre.size(); ++i)
    {
        if(i==num2)
        {
            tampon.push_back(i);
        }
        if(i!=num)
        {
            if(arbre[i]!=-1)
            {
                size_t j=arbre[i];
                if(i==num2)
                {
                    tampon.push_back(j);
                }

                while(j!=num)
                {
                    j=arbre[j];
                    if(i==num2)
                    {
                        tampon.push_back(j);
                    }
                }
            }
        }

    }
    for(int i=0; i<(int)tampon.size(); i++)
    {
        int tmp;
        tmp=tampon[i+1];
        std::string m=Type_Chemin_S1_S2(tmp,tampon[i]);
        std::string n=Nom_Chemin_S1_S2(tmp,tampon[i]);
        if(tampon[i]!=(int)num)
        {
            std::cout<<tampon[i]<<"<--";
            couleur.couleur_type(m);
            std::cout<<m<<" ";
            std::cout<<n<<" ";
            couleur.setColor(0);
            couleur.couleur(15);
            std::cout<<"<--";
            poids=poids+Poids_Chemin_S1_S2(tmp,tampon[i]);
        }
        else
        {
            std::cout<<tampon[i];
            poids=poids+Poids_Chemin_S1_S2(tmp,tampon[i]);
        }
    }
    std::cout<<"\n Temps : "<<poids<<" minutes"<<std::endl;
}


void Graphe::afficherParcours(size_t num,const std::vector<int>& arbre)
{
    double poids = 0;

    for(size_t i=0; i<arbre.size(); ++i)
    {
        if(i!=num)
        {
            if(arbre[i]!=-1)
            {
                std::cout<<i<<"<- ";
                size_t j=arbre[i];
                couleur.couleur_type(Type_Chemin_S1_S2(j,i));
                std::cout<<Type_Chemin_S1_S2(j,i)<<" "<<Nom_Chemin_S1_S2(j,i);
                couleur.setColor(0);
                couleur.couleur(15);
                std::cout<<" <- ";
                poids=Poids_Chemin_S1_S2(j,i);
                while(j!=num)
                {
                    std::cout<<j<<"<- ";
                    size_t tmp = j;
                    j=arbre[j];
                    std::string m=Type_Chemin_S1_S2(j,tmp);
                    std::string n=Nom_Chemin_S1_S2(j,tmp);
                    poids=poids+Poids_Chemin_S1_S2(j,tmp);
                    couleur.couleur_type(m);
                    std::cout<<m<<" "<<n;
                    couleur.setColor(0);
                    couleur.couleur(15);
                    std::cout<<" <-";
                }
                std::cout<<j<<"\n  Temps : "<<poids<<" minutes\n"<<std::endl;
            }
        }
    }
}


void Graphe::afficher_sommet() const
{

    std::cout<<"\n    Liste des sommets :\n"<<std::endl;
    for (auto s : m_sommets)
    {
        std::cout  <<s->getNum() << "     " <<s->getNom() << "     " << s->getAltitude() << std::endl;
    }
}


void Graphe::afficher_arc() const
{
    std::cout<<"\n    Liste des trajets :\n"<<std::endl;

    for (auto s : m_trajets)
    {
        std::cout << "\n";
        s->afficher();

    }
    std::cout << "\n";
}


void Graphe::infoTrajet()
{
    int passage = 0;
    std::string nomTrajet;
    std::cout<<"\n   Sur quel trajet souhaitez vous etre renseigne ?"<< std::endl;
    std::cout<<"\n   Votre choix : ";
    std::cin >> nomTrajet;
    for(int i=0; i<(int)m_trajets.size(); i++)
    {
        if(m_trajets[i]->getNom()==nomTrajet)
        {
            passage++;
            std::string depart=m_trajets[i]->getExtremites().first->getNom();
            std::string arrive=m_trajets[i]->getExtremites().second->getNom();
            std::cout<<"\n Depart : "<<depart<<"\n Arrive : "<<arrive<<"\n"<<std::endl;
        }

    }
    if(passage == 0)
    {
        std::cout << "\n   Erreur sur le trajet renseigne !\n" << std::endl;
    }
}


void Graphe::infoSommet()
{
    std::string nomSommet;
    std::cout<<"\n   Sur quel numero de sommet souhaitez vous etre renseigne ?"<< std::endl;
    std::cout<<"\n   Votre choix : ";
    std::cin >> nomSommet;
    bool passage = false;
    std::vector<std::string> Trajet_entrant;
    std::vector<std::string> Trajet_sortant;

    for(int i=0; i<(int)m_trajets.size(); i++)
    {
        if(m_trajets[i]->getExtremites().first->getNom()==nomSommet)
        {
            passage = true;
            Trajet_sortant.push_back(m_trajets[i]->getNom());
        }
        if(m_trajets[i]->getExtremites().second->getNom()==nomSommet)
        {
            passage = true;
            Trajet_entrant.push_back(m_trajets[i]->getNom());
        }
    }

    if(passage == true){
    std::cout<<"\n Trajets qui arrivent a la station "<<nomSommet<<" : ";
    for(unsigned i=0; i<Trajet_entrant.size(); i++)
    {
        std::cout<<Trajet_entrant[i]<<"  ";
    }
    std::cout<<"\n\n Trajets qui partent a la station "<<nomSommet<<" : ";
    for(unsigned i=0; i<Trajet_sortant.size(); i++)
    {
        std::cout<<Trajet_sortant[i]<<"  ";
    }
    std::cout<<"\n\n\n";}

    else{std::cout << "\n   Ce numero de sommet n'existe pas...\n" << std::endl;}

}


void Graphe::personnaliser()
{
    int choix_preference;
    std::string choix;
    std::string choix_niveau;
    std::string remontee;
    bool trajet_ok = false;
    bool passage = false;

            do{

            system("cls");

            std::cout<<"\n    PARCOURS PERSONNALISE \n\n";
            std::cout<<"  1. Preferences des pistes" << std::endl;
            std::cout<<"  2. Preferences des remontees" << std::endl;
            std::cout<<"  3. Exclure des pistes" << std::endl;
            std::cout<<"\n   Votre choix : ";

            std::cin >> choix;

            if((choix == "1") || (choix == "2") || (choix == "3"))
            {
                choix_preference = atoi(choix.c_str());
                passage = true;
                break;
            }
            else{std::cin.ignore(); std::cout << "\n   Choisissez une option valide...\n" << std::endl; Sleep(1000);}


            }while(passage == false);

    if(choix_preference == 1)
    {
        std::cout<<"\n  Quel est votre niveau ?  \n\n  debutant   =>   Vertes + Bleues    \n  intermediaire   =>   Bleues + Rouges + Snowparks  \n  expert   =>   Tout, meme les noires :)!\n";
        std::cout<<"\n  Votre choix : ";

        while(getch() != 13)
        {

        std::cin>> choix_niveau;

        if(choix_niveau=="debutant")
        {
            for(int i = 0 ; i < (int)m_preference.size() ; i++)
            {
                if ((m_preference[i] == "N") || (m_preference[i] == "R") || (m_preference[i] == "SURF"))
                {
                    m_preference.erase(m_preference.begin()+i);
                }
            }
            break;
        }


        else if(choix_niveau=="intermediaire")
        {
            m_preference.push_back("R");
            m_preference.push_back("SURF");

            for(int i = 0 ; i < (int)m_preference.size() ; i++)
            {
                if (m_preference[i] == "N")
                {
                    m_preference.erase(m_preference.begin()+i);
                }
            }
            break;
        }


        else if(choix_niveau=="expert")
        {
            m_preference.push_back("R");
            m_preference.push_back("N");
            m_preference.push_back("SURF");
            break;
        }

        else{
                std::cout << "\n  Choix non valide ! " << std::endl; Sleep(1000);
            }

        system("cls");

        std::cout<<"\n  Quel est votre niveau ?  \n\n  debutant   =>   Vertes + Bleues    \n  intermediaire   =>   Bleues + Rouges + Snowparks  \n  expert   =>   Tout, meme les noires :)!\n";
        std::cout<<"\n  Votre choix : ";
      }
    }

    if (choix_preference == 2)
    {
        std::cout << "\n   Liste des remontees  : " << std::endl;
        std::cout << "\n 1. Teleski \n 2. Telesiege \n 3. Telepherique \n 4. Telecabine \n 5. Telesiege Debrayable \n 6. Bus \n 7. Reset" << std::endl;
        std::cout << "\n  Que voulez vous retirer ? (Retour -> Enter) : ";

        while(getch() != 13)
        {

            std::cin >> remontee;

            if(remontee == "Teleski")
            {
                remontee = "TK";
            }
            else if(remontee == "Telesiege")
            {
                remontee = "TS";
            }
            else if(remontee == "Telecabine")
            {
                remontee = "TC";
            }
            else if(remontee == "Telesiege Debrayable")
            {
                remontee = "TSD";
            }
            else if(remontee == "Bus")
            {
                remontee = "BUS";
            }
            else if(remontee == "Reset")
            {
                m_preference.push_back("TK");
                m_preference.push_back("TS");
                m_preference.push_back("TC");
                m_preference.push_back("TSD");
                m_preference.push_back("BUS");
            }

            else{
                std::cout << "\n  Choix non valide ! " << std::endl; Sleep(1000);
            }

            for(int i = 0 ; i < (int)m_preference.size() ; i++)
            {
                if (m_preference[i] == remontee)
                {
                    m_preference.erase(m_preference.begin()+i);
                }
            }

            system("cls");

            std::cout << "\n   Liste des remontees : " << std::endl;
            std::cout << "\n 1. Teleski \n 2. Telesiege \n 3. Telepherique \n 4. Telecabine \n 5. Telesiege Debrayable \n 6. Bus \n 7. Reset" << std::endl;
            std::cout << "\n  Votre choix (Retour -> Enter) : ";

        }

    }

    if(choix_preference == 3)
    {
        std::cout << "\n   Tapez Reset -> pour remettre toutes les pistes" << std::endl;
        std::cout << "   Nom de la piste a retirer (Retour -> Enter) : ";

        while(getch() != 13)
        {

            std::cin >> remontee;


            if(remontee == "Reset")
            {
                if(piste_enlevee.size() != 0)
                {
                    for(int i = 0 ; i < (int)piste_enlevee.size() ; i++)
                    {
                        piste_enlevee.erase(piste_enlevee.begin()+i);
                    }
                }
            }

            else
            {
                for(auto x : m_trajets)
                {
                    if (x->getNom() == remontee)
                    {
                        trajet_ok = true;
                        piste_enlevee.push_back(remontee);
                        std::cout << "\n   Piste retiree !" << std::endl;
                        Sleep(500);

                        break;

                    }

                }

                if(trajet_ok == false)
                {
                    std::cout << "\n  Cette piste n'existe pas..." << std::endl;
                    Sleep(1000);
                }
            }


            system("cls");

            std::cout << "\n   Tapez Reset -> pour remettre toutes les pistes" << std::endl;
            std::cout << "   Nom de piste a retirer (Retour -> Enter) : ";
          }




    }



}

void Graphe::ski_de_fond()
{
    //Initialisation des variables principales
    std::vector<Trajet*> resultat_krustal; //Notre arbre vide
    std::pair<Sommet*,Sommet*> tampon;
    std::vector<Trajet*> trajets_pistes;
    double poidsTotal = 0;
    int nombre_tour = 0;


    for(auto x : m_trajets)
    {
        if (x->getDescente() == true)
        {
            trajets_pistes.push_back(x);
        }
    }

    //Tri du vecteur d'arrete par ordre croissant de poids
    std::sort(trajets_pistes.begin(),trajets_pistes.end(),[](Trajet* a, Trajet* b)
    {
        return a->getPoids() > b->getPoids();
    });

    while(resultat_krustal.size() < m_sommets.size()-4) // Tant que notre arbre de r�sultats est inf�rieur au nombre de sommets -1
    {
        tampon = trajets_pistes[nombre_tour]->getExtremites(); // On r�cupere les extremites de l'arete de poids le plus haut

        if(tampon.first->getNumCC() != tampon.second->getNumCC()) //On v�rifie si elle peut �tre s�lectionn�e
        {
            resultat_krustal.push_back(trajets_pistes[nombre_tour]); // Si oui, on ajoute l'arete � notre arbre de poids couvrant minimum

            //Check et actualisation des numCC

            int x = tampon.second->getNumCC(); // On sauvegarde notre point de comparaison

            for(int k = 0 ; k < (int)m_sommets.size() ; k++) // Pour tous les sommets
            {
                if (m_sommets[k]->getNumCC() == x) // Si les NumCC sont �gaux, on remplace
                {
                    m_sommets[k]->setNumCC(tampon.first->getNumCC());
                }
            }
        }

        nombre_tour++; // On incr�mente le nombre de tour
    }


    //Affichage du r�sultat !

    std::cout << "\n Chemin maximum couvrant la station : \n" << std::endl;

    for(auto x : resultat_krustal)
    {
        poidsTotal += x->getPoids(); //On incr�mente le poids total pour chaque sommet dans l'arbre

        x->afficher(); //On affiche les ar�tes et leurs poids

        std::cout << "\n";
    }

    for(auto x : resultat_krustal)
    {
            std::cout << " <- ";
            couleur.couleur_type(x->getType());
            std::cout << x->getType() << " " << x->getNom();
            couleur.setColor(0);
            couleur.couleur(15);
    }

    std::cout << "\n Le temps total est de : " << poidsTotal << " minutes" << std::endl; // Affichage du poids total de l'arbre

//        std::cout << trajets_pistes.size() << std::endl;
//        std::cout << resultat_krustal.size() << std::endl;
}

bool Graphe::fordFulkBfs(int graphEc[ORDRE][ORDRE], int depart, int arrivee, int pred[]) // renvoie true si il y a une chemin de la source vers le puit dans le graphe d'�cart
{
    bool marque[ORDRE];
    for (int j=0; j>ORDRE; j++)
    {
        marque[j]=false; //initialisation de tous les somments � 0
    }
//std::cout << "spoutnik ";
//    std::vector<int> Trajet_source;
//    std::vector<int> Trajet_puit;
//    for(int i=0; i<(int)m_trajets.size(); i++) //parcours des trajets
//    {
//        if(m_trajets[i]->getExtremites().second->getNum()==depart) //si c'est un trajet qui arrive � la source
//        {
//            Trajet_source.push_back(m_trajets[i]->getExtremites().first->getNum());
//        }
//        if(m_trajets[i]->getExtremites().first->getNum()==arrivee) //si c'est un trajet qui part du puit
//        {
//            Trajet_puit.push_back(m_trajets[i]->getExtremites().second->getNum());
//        }
//    }
//    for(unsigned i=0; i<Trajet_source.size(); i++) // marquage de tous les trajets arrivant � la source
//    {
//        marque[Trajet_source[i]]=true;
//    }
//
//    for(unsigned i=0; i<Trajet_puit.size(); i++) // marquage de tous les trajets sortant du puit
//    {
//        marque[Trajet_puit[i]]=true;
//    }
    //marquage des flot entrant dansla source
    //marquage des arretes sortant du puit

    std::queue<int> file;
    file.push(depart);
    marque[depart]=true; //on marque le premier sommet (la source)
    pred[depart]=-1;

    while (!file.empty()) //loop
    {
        int i = file.front();
        file.pop();
        for (int j=0; j<ORDRE; j++)
        {
            std::cout << " j "<<j<<" i "<<i<<" graphe ecart = "<<graphEc[j][i]<<" marquee? " <<marque[j]<<std::endl;
//              std::cout <<" graphe ecart = "<<graphEc[i][j]<< "         j="<<j<<" i="<<i<<std::endl;
            if((marque[j]== false)&&(graphEc[i][j]>0))
            {
                if (j==arrivee)
                {
                    pred[j] = i; //  il y a un chemin entre la source et le puit
                    std::cout << "fiiin";
                    return true;
                }

                file.push(j);
                pred[j] = i;
                marque[j] = true;
            }
        }
    }
    return false;
}

int Graphe::fordFulkerson (int graphe[ORDRE][ORDRE], int depart, int arrivee)
{
    int i, j;
    int graphEc[ORDRE][ORDRE];
    for (int i=0; i<ORDRE; i++)
    {
        for (int j=0; j<ORDRE; j++)
        {
            graphEc[i][j] = graphe[i][j] ;
        }
    }
//    for (int i=0; i<ORDRE; i++)
//    {
//        for (int j=0; j<ORDRE; j++)
//        {
//            std::cout << graphEc[i][j] <<" ";
//        }
//        std::cout <<std::endl;
//    }

    int pred [ORDRE]; //ce tableau enregistre le chemin via les predecesseurs
    int flotMaximum=0; //on initialise le flot max � 0

    while(fordFulkBfs(graphEc, depart, arrivee, pred))
    {
        int flotDuChemin = 0;
        for(j = arrivee; j != depart; j = pred[j])
        {
            i=pred[j];
            flotDuChemin = std::min(flotDuChemin, graphEc[i][j]);

        }
        for(j = arrivee; j != depart; j = pred[j])
        {
            i=pred[j];
            graphEc[i][j]-=flotMaximum;
            graphEc[j][i]+=flotMaximum;
        }
        flotMaximum+=flotDuChemin;
    }
    return flotMaximum;
}

void Graphe::flots (int depart, int arrivee)
{
    std::cout << "\n probleme des flots maximums" << std::endl;
    // chargement de la matrice d'adjacence � partir du graphe
    int matAdj[ORDRE][ORDRE]= {
        {0,0,0,0,0,0,0},
        {4,0,0,0,0,0,0},
        {9,0,0,0,0,0,0},
        {0,3,0,0,0,0,0},
        {0,2,8,0,0,0,0},
        {0,0,0,1,4,0,0},
        {0,0,0,3,10,1,0}
    };

//    for (int i=0; i<ORDRE; i++)
//    {
//        for (int j=0; j<ORDRE; j++)
//        {
//            matAdj[i][j] = 0; //on remplit la matrice de 0 (=il n'y a aucun lien entre les points
//        }
//    }
//    for(int i=0; i<(int)m_trajets.size(); i++)
//    {
//        std::pair<Sommet*,Sommet*> tampon=m_trajets[i]->getExtremites();
//        matAdj[tampon.first->getNum()][tampon.second->getNum()] = m_trajets[i]->getCapacity();// parours des arretes , pour chaque arrete on entre la capacit�
//        std::cout <<  matAdj[tampon.first->getNum()][tampon.second->getNum()] <<" "<< std::endl;
//    }
//
//    for (int i=0; i<ORDRE; i++)
//    {
//        for (int j=0; j<ORDRE; j++)
//        {
//            std::cout << matAdj[i][j] <<" ";
//        }
//        std::cout <<std::endl;
//    }


    int maxFlot;
    maxFlot = fordFulkerson(matAdj,depart,arrivee);  //appel de l'algo de ford-fulkerson

    if (maxFlot!=0)
    {
        std::cout  <<"le flot possible maximum est "<< maxFlot << std::endl;
    }
    else
    {
        std::cout  <<" le flot entre ces 2 points est inexistant "<< std::endl;
    }


}

void Graphe::connexion()
{
    std::string nom, date, nom_trajet;
    int taille_pref, nb_piste_enlevee;
    bool trajet_ok = false;
    std::string valeur_pref, piste;
    std::vector<std::string> tampon_pref, tampon_piste;
    std::string tampon_nom,tampon_date;
    std::pair<std::string,std::string> identite_user, identite_lambda;
    std::pair<std::string,std::string> identite_admin = std::make_pair("Touchousse","08/06/2001");

    system("cls");

    std::cout << "\n    CONNEXION ESPACE UTILISATEUR" << std::endl;
    std::cout << "\n  Nom de famille : ";
    std::cin >> nom;
    std::cout << "  Date de naissance (JJ/MM/AAAA) : ";
    std::cin >> date;

    identite_user.first = nom;
    identite_user.second = date;

    if(identite_user == identite_admin)
    {
        std::cout << "\n    ADMIN CONNECTED";
        Sleep(1500);
        system("cls");

        std::cout << "\n  Nom de la piste / remontee a exclure \n  => Commandes : save ou reset" << std::endl;
        std::cout << "\n  Votre choix : ";

        while(getch() != 13)
        {
            std::cin >> nom_trajet;

            if (nom_trajet == "save")
        {
            std::ofstream ifs{"admin.txt", std::ios::out | std::ios::app};

            if (!ifs)
            throw std::runtime_error( "Impossible d'ouvrir en ecriture le fichier admin.txt" );

                for(auto x : piste_enlevee)
                {
                    ifs << "\n";
                    ifs << x;
                }

            std::cout << "\n   Sauvegarde reussie !" << std::endl;
            break;
        }

        else if(nom_trajet == "reset")
        {

            for(int i = 0 ; i < (int)piste_enlevee.size() ; i++)
            {
                piste_enlevee.erase(piste_enlevee.begin()+i);
            }

            m_trajets = sauvegarde_trajets;

            std::ofstream fichier ("admin.txt");

                std::cout << "\n   Reset reussi !" << std::endl;
                break;

        }

        else{

            for(auto x : m_trajets)
                {
                    if (x->getNom() == nom_trajet)
                    {
                        trajet_ok = true;
                        piste_enlevee.push_back(nom_trajet);
                        std::cout << "\n   Piste retiree !" << std::endl;
                        Sleep(500);

                        break;

                    }

                }

                if(trajet_ok == false)
                {
                    std::cout << "\n  Cette piste n'existe pas..." << std::endl;
                    Sleep(1000);
                }

                system("cls");

                std::cout << "\n  Nom de la piste / remontee a exclure \n  => Commandes : save ou reset" << std::endl;
                std::cout << "\n  Votre choix : ";
        }
        }

    }

    std::ifstream ifs{"users.txt",std::ios::in};
    if (!ifs)
        throw std::runtime_error( "Impossible d'ouvrir en lecture le fichier users.txt" );

    else{

    std::ifstream ifs{"users.txt",std::ios::in};
    if (!ifs)
    throw std::runtime_error( "Impossible d'ouvrir en lecture le fichier users.txt" );

    while(!ifs.eof())
    {
        ifs >> tampon_nom;
        ifs >> tampon_date;

        identite_lambda.first = tampon_nom;
        identite_lambda.second = tampon_date;

        ifs>>taille_pref;

        for(int i = 0 ; i < taille_pref ; i++)
        {
            ifs>>valeur_pref;
            tampon_pref.push_back(valeur_pref);
        }

        ifs >> nb_piste_enlevee;

        if(nb_piste_enlevee != 0)
        {
            for(int i = 0 ; i < nb_piste_enlevee ; i++)
            {
                ifs >> piste;
                tampon_piste.push_back(piste);
            }
        }

        if(identite_user == identite_lambda)
        {
            break;
        }

    }

    if(identite_user == identite_lambda)
    {
        m_preference = tampon_pref;
        piste_enlevee = tampon_piste;

        m_connexion = true;

        std::cout << "\n  Connexion reussie !" << std::endl;
    }

    else if(identite_user != identite_admin)
    {
        std::cout << "\n  Vous n'etes pas enregistre !" << std::endl;
    }
    }
}

void Graphe::deconnexion()
{
    if(m_connexion == true)
    {
        m_preference = m_preference_initial;

        for(int i = 0 ; i < (int)piste_enlevee.size() ; i++)
        {
            piste_enlevee.erase(piste_enlevee.begin()+i);
        }

        std::cout << "\n  Deconnexion reussie !" << std::endl;
    }

    else
    {
        std::cout << "\n  Vous n'etes meme pas connectes..." << std::endl;
    }
}

void Graphe::sauvegarde()
{
    std::string nom,date;
    std::vector<std::string> pref;

    std::cout << "\n  SAUVEGARDE DE VOS DONNEES\n" << std::endl;
    std::cout << "  Votre nom : ";
    std::cin >> nom;
    std::cout << "  Votre date de naissance (JJ/MM/AAAA) : ";
    std::cin >> date;

    std::ofstream fichier("users.txt", std::ios::out | std::ios::app);

    int position = fichier.tellp();

    if(position != 0)
    {
        fichier << "\n";
    }

    fichier << nom;
    fichier << "\n"<< date;
    fichier << "\n" << m_preference.size();

    for(auto x : m_preference)
    {
        fichier<< "\n";
        fichier << x;
    }

    fichier << "\n";
    fichier << piste_enlevee.size();
    fichier <<"\n";

    if (piste_enlevee.size() != 0)
    {
        for(auto x : piste_enlevee)
        {
            fichier << x;
            fichier << "\n";
        }
    }

    std::cout << "\n  Sauvegarde reussie !" << std::endl;
}

Graphe::~Graphe()
{
    for (auto s : m_sommets)
        delete s;
    //dtor
}
