#include "Graphe.h"

///CONSTRUCTEUR GRAPHE / STATION

Graphe::Graphe(std::string nomFichier)
{
    ///CHARGEMENT DU FICHIER data_arcs.txt

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
        m_sommets.push_back(new Sommet{numero_sommet}); //On cree le sommet
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

    ///CHARGEMENT des informations des trajets
    for(int i = 0 ; i < taille ; i++)
    {
        ifs>>id_trajet>>nom_trajet>>type_trajet>>sommet_1>>sommet_2>>capacityArc;

        extremites.first = m_sommets[sommet_1-1];
        extremites.second = m_sommets[sommet_2-1];

        m_trajets.push_back(new Trajet{extremites,id_trajet,nom_trajet,type_trajet,capacityArc});//creation trajet

    }

    double poids;
    std::pair<Sommet*,double> tampon;

    //Pour chaque trajet cree
    for(auto x : m_trajets)
    {
        //On calcule son poids en minutes et on lui attribue
        poids = x->calcul_poids();
        x->setPoids(poids);
        extremites = x->getExtremites();

        tampon.first = extremites.second;
        tampon.second = poids;
        //On ajoute au sommet leur successeurs pour l'algo de dijkstra en particulier
        m_sommets[extremites.first->getNum()-1]->ajouterSucc(tampon);
    }

    for(int k=0; k<(int)m_trajets.size(); k++) //parcourt de tous les trajets
    {
        m_trajets[k]->setFlot(m_trajets[k]->getCapacity());
    }

    sauvegarde_trajets = m_trajets;
    sauvegarde_sommets = m_sommets;

    ///PARTIE ADMIN BONUS TOUCHOUSSE
    std::string trajet;

    std::ifstream fichier ("admin.txt");
    if(!fichier)
    {
        throw std::runtime_error("Impossible d'ouvrir en lecture admin.txt");
    }

    //On ouvre notre fichier admin.txt et on le lit jusqu'a la fin
    while(!fichier.eof())
    {
        fichier >> trajet;
        //On supprime les trajets qui sont indiques dans le fichier et on les sauvegarde
        for(int i = 0 ; i < (int)m_trajets.size() ; i++)
        {
            if (m_trajets[i]->getNom() == trajet)
            {

                m_trajets_fermes.push_back(trajet);
                m_trajets.erase(m_trajets.begin()+i);
            }
        }
    }

    //On ajoute a nos preferences tous les types de trajet possibles initialement

    m_preference.push_back("V");
    m_preference.push_back("R");
    m_preference.push_back("B");
    m_preference.push_back("N");
    m_preference.push_back("SURF");
    m_preference.push_back("KL");
    m_preference.push_back("TK");
    m_preference.push_back("TS");
    m_preference.push_back("TPH");
    m_preference.push_back("BUS");
    m_preference.push_back("TC");
    m_preference.push_back("TSD");

    m_preference_initial = m_preference; // On sauvegarde ces preferences initiales
}



void Graphe::Dijkstra(int depart,int arrivee,double condition_temps)
{

    ///initialisations
    //initialisation de notre tampon pair utilise dans l'algo.
    std::pair<Sommet*,double> p;

    bool comparaison;

    //on initialise le vecteur des predecesseurs pour chaque sommet avec la valeur -1.
    std::vector<int> pred(m_sommets.size(),-1);

    //Initialisation marquage des sommets pour les dijkstra multiples
    for(auto x : m_sommets)
    {
        x->setMarque(false);
    }

    //D???claration de la fonction de comparaison utilis???e par la priority_queue
    auto compare = [](std::pair<Sommet*,double> a, std::pair<Sommet*,double> b)
    {
        return b.first->getDistance() < a.first->getDistance();
    };

    //D???claration de la file de priorit???
    std::priority_queue<std::pair<Sommet*,double>, std::vector<std::pair<Sommet*,double>>, decltype(compare)> file(compare);

    //initialise tous les sommets ??? valeur th???oriquement infinie sauf celui de d???part.
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

    //Ins???rer le sommet initial dans la file avec une distance nul car on ne bouge pas.
    file.push(std::make_pair(m_sommets[depart-1],0));

    ///Boucle de recherche

    while (!file.empty()) //tant que notre file n'est pas vide on continue.
    {
        p = file.top(); //je consid???re que p est le successeur avec le plus cours chemin, je le choisi donc pour le reste de la boucle
        file.pop();//je le supprime pour ne pas retomber dessus

        if (p.first->getMarque()==false) // si le sommet n'a pas ??t?? marqu??
        {
            p.first->setMarque(true);

            for (auto succ : p.first->getSuccesseurs()) //pour chaque successeur :
            {
                //Boucle pour verifier qu'on utilise des trajets qu'on souhaite
                for(auto pref : m_preference)
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
                //Boucle pour verifier qu'on utilise des pistes qu'on souhaite
                for(auto x : piste_enlevee)
                {
                    if(trajet_avec_ses_succ(p.first,succ.first)->getNom() == x)
                    {
                        comparaison = false;
                        break;
                    }
                }

                if ((succ.first->getMarque()== false) && (comparaison == true)) // si le successeur n'a pas ???t??? marqu???
                {

                    // on calcule le chemin parcourure jusqu'au sommet s, on additionne le poid du chemin jusqu'??? ce predecesseur
                    double addition = p.first->getDistance() + succ.second;

                    if (addition < succ.first->getDistance()) //si ce chemin est plus petit que le pr???c???dent chemin enregistr??? alors on le remplace
                    {

                        succ.first->setDistance(addition); //on d???finit la nouvelle distance (plus courte) entre le d???part et ce nouveau point(qui est un successeur du sommet qu'on ???tudiait)
                        pred[succ.first->getNum()] =  p.first->getNum(); // On sauvegarde le predecesseur pour avoir une trace de notre chemin.
                        file.push(succ); //Enfin, on rajoute notre successeur ??? la file de priorit???.

                    }
                }
            }
        }
    }

    ///Affichage des resultats
    ///Valeur du poids

    if(m_sommets[arrivee-1]->getDistance()!=9999)
    {
        std::cout << "\n\n Le chemin le plus court entre le sommet "<< depart <<" et le sommet "<< arrivee <<" est de : ";
        couleur.couleur(6);
        std::cout << m_sommets[arrivee-1]->getDistance();
        couleur.couleur(15);
        std::cout << " minutes ";

        ///si il y a une condition de temps et qu'elle est respect??e
        if((m_sommets[arrivee-1]->getDistance()<=condition_temps) && (condition_temps!=0))
        {
            std::cout<<"<--";
            couleur.couleur(10);
            std::cout <<" TEMPS SUFFISANT "<< std::endl; //on affiche le plus court chemin entre les deux sommets choisis
            couleur.couleur(15);
        }

        ///si il y a une condition de temps et qu'elle n'est pas respect??e
        else if((m_sommets[arrivee-1]->getDistance()>condition_temps) && (condition_temps!=0))
        {
            double depassement=m_sommets[arrivee-1]->getDistance()-condition_temps;
            std::cout<<"<--";
            couleur.couleur(4);
            std::cout <<" TEMPS INSUFFISANT + "<<depassement<<" minutes" <<std::endl; //on affiche le plus court chemin entre les deux sommets choisis
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
    else
    {
        //Si aucun chemin n'est possible
        std::cout<<"\n"<<arrivee;
        couleur.couleur(4);
        std::cout<<" : Aucun chemins possibles\n\n";
        couleur.couleur(15);

    }




}

/// Parcours BFS
std::vector<int> Graphe::BFS(int num_S0)
{

    std::queue<Sommet*>pile;
    int indice;

    // pour le marquage
    std::vector<int> couleurs((int)m_sommets.size()+1,0);
    //pour noter les pr???d???cesseurs
    std::vector<int> predec((int)m_sommets.size()+1,-1);

    bool comparaison = true;

    //???tape initiale
    pile.push(m_sommets[num_S0-1]);
    couleurs[num_S0]=1;
    Sommet*so;

    //tant que la pile n'est pas vide
    while(!pile.empty())
    {
        so=pile.front();
        pile.pop();

        for(auto succ : so->getSuccesseurs()) //On parcourt tous les successeurs
        {
            //Verification des preferences de pistes/remontees
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
            //Verification des pistes enlevees
            for(auto x : piste_enlevee)
            {
                if(trajet_avec_ses_succ(so,succ.first)->getNom() == x)
                {
                    comparaison = false;
                    break;
                }
            }

            if((couleurs[succ.first->getNum()]==0) && (comparaison == true)) //s'il n'est pas marqu??? et pas dans preference
            {
                couleurs[succ.first->getNum()]=1; //on le marque
                predec[succ.first->getNum()]= so->getNum();
                pile.push(succ.first);//on le met dans la pile
                indice++;
            }
        }
    }
    //Permet de savoir si le chemin est vide pour l'affichage ulterieur
    sommet_avec_son_Id(num_S0)->setTaille_BFS(indice);
    //On retourne le resultat pour l'affichage suivant
    return predec;
}

//Fonction qui recupere le nom d'un trajet a partir de deux sommet
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

//Fonction qui recupere le nom d'un trajet a partir d'un ID de trajet

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
//Fonction qui recupere un sommet d'un trajet a partir d'un ID de sommet

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

//Fonction qui recupere un trajet a partir de son id

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

//Fonction qui recupere un trajet a partir de son nom

Trajet* Graphe::trajet_avec_son_nom(std::string nom)
{
    Trajet* tampon = NULL;

    for(int i=0; i<(int)sauvegarde_trajets.size(); i++)
    {
        if(sauvegarde_trajets[i]->getNom()==nom)
        {
            tampon=sauvegarde_trajets[i];
        }
    }
    return tampon;
}

//Fonction qui recupere un type de trajet a partir de deux sommets

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

//Fonction qui recupere un temps de trajet a partir de deux sommets

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

//Fonction qui recupere un numero de sommet a partir de son nom

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

//Fonction qui recupere un id de trajet a partir de son nom

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

//Fonction qui recupere un trajet a partir de deux sommets

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

///Fonction d'affichage du BFS pour un seul parcours

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

    std::cout<<"\n Temps : ";
    couleur.couleur(6);
    std::cout<<poids;
    couleur.couleur(15);
    std::cout<<" minutes"<<std::endl;

    if(poids==0)
    {
        //Si il n'y a pas de chemin possible
        system("cls");
        couleur.couleur(4);
        std::cout<<"\n\n   Aucun chemins possibles\n";
        couleur.couleur(15);
    }
}

///Fonction d'affichage BFS avec tous les parcours

void Graphe::afficherParcours(size_t num,const std::vector<int>& arbre)
{
    int tampon=sommet_avec_son_Id(num)->getTaille_BFS();
    if(tampon!=(int)num)
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
                    std::cout<<j<<"\n  Temps : ";
                    couleur.couleur(6);
                    std::cout<<poids;
                    couleur.couleur(15);
                    std::cout<<" minutes\n"<<std::endl;
                }
            }
        }
    }
    else
    {
        //Si les parcours ne sont pas possibles
        couleur.couleur(4);
        std::cout<<"Aucun chemins possibles \n";
        couleur.couleur(15);
    }
}

//Fonction affichage des informations d'un sommet
void Graphe::afficher_sommet() const
{

    std::cout<<"\n    Liste des sommets :\n"<<std::endl;
    for (auto s : m_sommets)
    {
        std::cout  <<s->getNum() << "     " <<s->getNom() << "     " << s->getAltitude() << std::endl;
    }
}

//Fonction affichage des informations d'un trajet

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

//Algorithme pour avoir les informations concernant un trajet

void Graphe::infoTrajet()
{
    //Initialisations variables
    int passage = 0;
    std::string nomTrajet;
    std::cout<<"\n   Sur quel trajet souhaitez vous etre renseigne ?"<< std::endl;
    std::cout<<"\n   Votre choix : ";
    std::cin >> nomTrajet;

    //On recupere les informations puis on les affiche directement si il y en a
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
    if(passage == 0) //Si on rentre un trajet qui n'existe pas
    {
        std::cout << "\n   Erreur sur le trajet renseigne !\n" << std::endl;
    }
}

//Algorithme qui permet d'avoir toutes les inforamtions concernant un point de la station

void Graphe::infoSommet()
{
    //Initialisations variables
    std::string nomSommet;
    std::cout<<"\n   Sur quel numero de sommet souhaitez vous etre renseigne ?"<< std::endl;
    std::cout<<"\n   Votre choix : ";
    std::cin >> nomSommet;
    bool passage = false;
    std::vector<std::string> Trajet_entrant;
    std::vector<std::string> Trajet_sortant;

    //On recupere les informations
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
    //Si il y en a on les affiche
    if(passage == true)
    {
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
        std::cout<<"\n\n\n";
    }

    else
    {
        //Si le numero de point de station est errone
        std::cout << "\n   Ce numero de sommet n'existe pas...\n" << std::endl;
    }

}

//Fonction qui gere les preferences de l'utilisateur ainsi que la sauvegarde de celle ci.

void Graphe::personnaliser()
{
    //Initialisation variables
    int choix_preference;
    std::string choix;
    std::string choix_niveau;
    std::string remontee;
    bool trajet_ok = false;
    bool passage = false;

    do
    {
        //Blindage de saisie

        system("cls");

        //Affichage menu
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
        else
        {
            std::cin.ignore();
            std::cout << "\n   Choisissez une option valide...\n" << std::endl;
            Sleep(1000);
        }


    }
    while(passage == false);

    if(choix_preference == 1)
    {
        //Choix des niveaux de pistes pour l'utilisateur
        std::cout<<"\n  Quel est votre niveau ?  \n\n  debutant   =>   Vertes + Bleues    \n  intermediaire   =>   Bleues + Rouges + Snowparks  \n  expert   =>   Tout, meme les noires :)!\n";
        std::cout<<"\n  Votre choix : ";

        while(getch() != 13)
        {
            //En fonction de son choix, on modifie le vecteur m_preference en supprimant les types de pistes souhaites.

            std::cin>> choix_niveau;

            ///NIVEAU DEBUTANT (pas de noirs/rouges/snowpark)

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

            ///NIVEAU INTERMEDIAIRE (pas de noires)

            else if(choix_niveau=="intermediaire")
            {
                //Au cas ou on rajoute ce qui aurait pu etre retire avec le choix debutant
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

            ///NIVEAU EXPERT (tout)

            else if(choix_niveau=="expert")
            {
                //On rajoute tout ce qui aurait pu etre enleve
                m_preference.push_back("R");
                m_preference.push_back("N");
                m_preference.push_back("SURF");
                break;
            }

            else
            {
                // Si le choix est non valide
                std::cout << "\n  Choix non valide ! " << std::endl;
                Sleep(1000);
            }

            system("cls");

            std::cout<<"\n  Quel est votre niveau ?  \n\n  debutant   =>   Vertes + Bleues    \n  intermediaire   =>   Bleues + Rouges + Snowparks  \n  expert   =>   Tout, meme les noires :)!\n";
            std::cout<<"\n  Votre choix : ";
        }
    }

    if (choix_preference == 2)
    {
        ///Categorie preferences de remontees
        //MENU
        std::cout << "\n   Liste des remontees  : " << std::endl;
        std::cout << "\n 1. Teleski \n 2. Telesiege \n 3. Telepherique \n 4. Telecabine \n 5. Telesiege Debrayable \n 6. Bus \n 7. Reset" << std::endl;
        std::cout << "\n  Que voulez vous retirer ? (Retour -> Enter) : ";

        while(getch() != 13)
        {

            std::cin >> remontee;

            //CONVERSION DU NOM rentre EN nom des types de remontees classiques
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
                //RESET POUR TOUT REMETTRE
                m_preference.push_back("TK");
                m_preference.push_back("TS");
                m_preference.push_back("TC");
                m_preference.push_back("TSD");
                m_preference.push_back("BUS");
            }

            else
            {
                //SI LE CHOIX MENU EST NON VALIDE
                std::cout << "\n  Choix non valide ! " << std::endl;
                Sleep(1000);
            }
            //ON EFFACE dans les preferences les types de remontees choisies
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
        ///Categorie BONUS exclure des pistes
        //MENU
        std::cout << "\n   Tapez Reset -> pour remettre toutes les pistes" << std::endl;
        std::cout << "   Nom de la piste a retirer (Retour -> Enter) : ";

        while(getch() != 13)
        {

            std::cin >> remontee;

            //COMMANDE reset pour tout remettre
            if(remontee == "Reset")
            {
                if(piste_enlevee.size() != 0)
                {
                    piste_enlevee.clear();
                }
            }

            else
            {
                //on verifie que les trajets choisis existe si oui on les ajoute au conteneur de piste enlevee
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
                    //SINON message d'erreur
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
///BONUS personnel ski de fond avec kruskal inverse

void Graphe::ski_de_fond()
{
    //Initialisation des variables principales
    std::vector<Trajet*> resultat_krustal; //Notre arbre vide
    std::pair<Sommet*,Sommet*> tampon;
    std::vector<Trajet*> trajets_pistes;
    double poidsTotal = 0;
    int nombre_tour = 0;

    //On ajoute que les trajets qui sont des descentes
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

    while(resultat_krustal.size() < m_sommets.size()-4) // Tant que notre arbre de r???sultats est inf???rieur au nombre de sommets -1
    {
        tampon = trajets_pistes[nombre_tour]->getExtremites(); // On r???cupere les extremites de l'arete de poids le plus haut

        if(tampon.first->getNumCC() != tampon.second->getNumCC()) //On v???rifie si elle peut ???tre s???lectionn???e
        {
            resultat_krustal.push_back(trajets_pistes[nombre_tour]); // Si oui, on ajoute l'arete ??? notre arbre de poids couvrant minimum

            //Check et actualisation des numCC

            int x = tampon.second->getNumCC(); // On sauvegarde notre point de comparaison

            for(int k = 0 ; k < (int)m_sommets.size() ; k++) // Pour tous les sommets
            {
                if (m_sommets[k]->getNumCC() == x) // Si les NumCC sont ???gaux, on remplace
                {
                    m_sommets[k]->setNumCC(tampon.first->getNumCC());
                }
            }
        }

        nombre_tour++; // On incremente le nombre de tour
    }


    //Affichage du resultat !

    std::cout << "\n Chemin maximum couvrant la station pour le ski de fond :) \n" << std::endl;

    int passage = 0;

    for(auto x : resultat_krustal)
    {
        passage++;

        poidsTotal += x->getPoids();
        couleur.couleur_type(x->getType());
        std::cout << x->getType() << " " << x->getNom();
        couleur.setColor(0);
        couleur.couleur(15);

        if(passage != (int)resultat_krustal.size())
        {
            std::cout << " <-> ";
        }

        if(passage%2)
        {
            std::cout<<"\n";
        }
    }


    std::cout << "\n Le temps total est de : ";
    couleur.couleur(6);
    std::cout << poidsTotal;
    couleur.couleur(15);
    std::cout << " minutes" << std::endl;
}

///FONCTIONS POUR LES FLOTS MAXIMUM
void Graphe::prepareSourcesFord(int depart, int arrivee,bool marque[ORDRE])
{
    std::vector<int> Trajet_source;
    std::vector<int> Trajet_puit;
    for(int i=0; i<(int)m_trajets.size(); i++) //parcours des trajets
    {
        if(m_trajets[i]->getExtremites().second->getNum()==depart) //si c'est un trajet qui arrive ??? la source
        {
            Trajet_source.push_back(m_trajets[i]->getExtremites().first->getNum());
        }
        if(m_trajets[i]->getExtremites().first->getNum()==arrivee) //si c'est un trajet qui part du puit
        {
            Trajet_puit.push_back(m_trajets[i]->getExtremites().second->getNum());
        }
    }
    for(unsigned i=0; i<Trajet_source.size(); i++) // marquage de tous les trajets arrivant ??? la source
    {
        marque[Trajet_source[i]]=true;
    }

    for(unsigned i=0; i<Trajet_puit.size(); i++) // marquage de tous les trajets sortant du puit
    {
        marque[Trajet_puit[i]]=true;
    }
}

bool bfsFord(int GraphePass[ORDRE][ORDRE], int depart, int arrivee, int parent[])
{
    bool marque[ORDRE]; //tableau de marquage
    for (int i = 0; i < ORDRE; i++)
    {
        marque[i]=false;  //initialise tous les sommets a non marques
    }


    std::queue<int> fichier;
    fichier.push(depart);
    parent[depart] = -1;
    marque[depart] = true;


    // loop du bfs
    while (!fichier.empty())
    {
        int i = fichier.front();
        fichier.pop();

        for (int j = 0; j < ORDRE; j++)
        {
            if (marque[j] == false && GraphePass[i][j] > 0)
            {
                if (j == arrivee)
                {
                    parent[j] = i;
                    return true;
                }
                parent[j] = i;
                fichier.push(j);
                marque[j] = true;
            }
        }
    }
    return false;
}

//retourne le flot max
int Graphe::fordFulkerson(int graphe[ORDRE][ORDRE], int depart, int arrivee)
{
    int i, j;
    int flotMinimum = INT_MAX; // initialisation du flot min a une valeur tr??s grande
    int flotMaximum = 0; // initialisation du flot max a 0
    std::vector <int> predFlotMin; //permet d'enregistrer le chemin avec le moins de skieurs
    std::vector <int> predFlot; //stock le chemin de la boucle while
    predFlot.push_back(arrivee); //initialisation avec le sommet de d??part

// cr??ation d'un graphe de passage
    int GraphePass[ORDRE][ORDRE];
    for (i = 0; i < ORDRE; i++)
        for (j = 0; j< ORDRE; j++)
            GraphePass[i][j] = graphe[i][j]; //on recupere dans ce graphe le matrie d'adjacence

    int parent[ORDRE]; // tableau qui enregistre les chemins parcourus de la source au puit



    while (bfsFord(GraphePass, depart, arrivee, parent))
    {
        int flotChemin = INT_MAX ; //on le met a une valeur tr??s grande (l'entier maximum)

        for (j = arrivee; j != depart; j = parent[j]) //on parcourt les predecesseurs dans le sens inverse
        {
            i = parent[j];
            flotChemin = std::min(flotChemin, GraphePass[i][j]); //on ne garde que le plus petit flot
            predFlot.push_back(i);

            for(int k=0; k<(int)m_trajets.size(); k++) //parcourt de tous les trajets
            {
                std::pair<Sommet*,Sommet*> tampon=m_trajets[k]->getExtremites();
                if(tampon.first->getNum()==i && tampon.second->getNum()==j) //on cherche le trajet correspondant ?? notre chemin
                {
                    if (flotChemin<m_trajets[k]->getFlot()) //si le flot est plus petit que le flot existant alors on acualise le flot du chemin
                        m_trajets[k]->setFlot(flotChemin);
//
//                    std::cout << "le flot de "<< m_trajets[k]->getNom()<< "passant par "<<i<<" et "<<j<<" est de "<<m_trajets[k]->getFlot() << " flot chemin : "<< flotChemin << std::endl;
                }
            }
        }

        for (j = arrivee; j != depart; j = parent[j])
        {
            i = parent[j];
            GraphePass[i][j] -= flotChemin;
            GraphePass[j][i] += flotChemin;
        }

        if (flotMinimum>flotChemin)
        {
            flotMinimum = flotChemin;
            predFlotMin = predFlot; //on remplace le chemin avec le plus petit flot

            predFlot.clear();
            predFlot.push_back(arrivee);
        }



        flotMaximum += flotChemin; //actualise le flot maximum
    }
    std::cout <<std::endl<<"Pour skier en toute tranquilite nous vous conseillons de suivre : "<<std::endl;

    Trajet* tamponTr;
    std::cout<<"\n";

    ///affichage chemin a parcourir
    for(int k=0; k<(int)predFlotMin.size()-1; k++)
    {

        tamponTr=trajet_avec_ses_succ(sommet_avec_son_Id(predFlotMin[k+1]),sommet_avec_son_Id(predFlotMin[k]));
        std::cout<<predFlotMin[k];
        std::cout<<" <- ";
        couleur.couleur_type(tamponTr->getType());
        std::cout<<tamponTr->getType()<<" ";
        std::cout<<tamponTr->getNom();
        couleur.setColor(0);
        couleur.couleur(15);
        std::cout<<" <- ";

    }
            std::cout<<depart;




    std::cout <<std::endl<<"\n Il n'y a jamais plus de ";
    couleur.couleur(6);
    std::cout<<flotMinimum;
    couleur.couleur(15);
    std::cout<<" skieurs par heure qui suivent ce chemin !";
    return flotMaximum;//retourne le flot maximum
}

void Graphe::flots (int depart, int arrivee)
{
    //on initialise tous les flots a la valeur max (= valeur de leur capacite)
    for(int k=0; k<(int)m_trajets.size(); k++) //parcourt de tous les trajets
    {
        m_trajets[k]->setFlot(m_trajets[k]->getCapacity());
    }

    int matAdj[ORDRE][ORDRE];

    // chargement de la matrice d'adjacence a partir du graphe
    for (int i=0; i<ORDRE; i++)
    {
        for (int j=0; j<ORDRE; j++)
        {
            matAdj[i][j] = 0; //on remplit la matrice de 0 (=il n'y a aucun lien entre les points
        }
    }
    Graphe gr{"data_arcs.txt"};

    for(int i=0; i<(int)m_trajets.size(); i++)
    {
        std::pair<Sommet*,Sommet*> tampon=m_trajets[i]->getExtremites();
        matAdj[tampon.first->getNum()][tampon.second->getNum()] = m_trajets[i]->getCapacity();// parours des arretes , pour chaque arrete on entre la capacitee
    }

    int maxFlot;
    maxFlot = gr.fordFulkerson(matAdj,depart,arrivee);  //appel de l'algo de ford-fulkerson

    if (maxFlot!=0)
    {
        std::cout<<std::endl<<std::endl<<"Information de la station : en prenant en compte tous le chemins reliants ces deux points"<< std::endl<<"le nombre maximum de skieurs pouvant se rendre de "<<depart<<" a "<<arrivee<<" est de "<< maxFlot <<" skieurs par heure "<<std::endl;
    }
    else
    {
        std::cout << std::endl <<" il n'est pas possible de relier ces deux points "<< std::endl;
    }
}

///FONCTION DE CONNEXION pour l'utilisateur

void Graphe::connexion()
{
    ///INITIALISATIONS VARIABLES
    std::string nom, date, nom_trajet;
    int taille_pref, nb_piste_enlevee;
    bool trajet_ok = false;
    std::string valeur_pref, piste;
    std::vector<std::string> tampon_pref, tampon_piste;
    std::string tampon_nom,tampon_date;
    std::pair<std::string,std::string> identite_user, identite_lambda;
    std::pair<std::string,std::string> identite_admin = std::make_pair("Touchousse","08/06/2001");

    system("cls");

    //MENU
    std::cout << "\n    CONNEXION ESPACE UTILISATEUR" << std::endl;
    std::cout << "\n  Nom de famille : ";
    std::cin >> nom;
    std::cout << "  Date de naissance (JJ/MM/AAAA) : ";
    std::cin >> date;

    //On stock les informations utilisateur dans une pair
    identite_user.first = nom;
    identite_user.second = date;

    if(identite_user == identite_admin)
    {
        ///CATEGORIE BONUS CONNEXION ADMIN M.TOUCHOUSSE

        std::cout << "\n    ADMIN CONNECTED";
        Sleep(1500);
        system("cls");

        //MENU ADMIN
        std::cout << "\n  Nom de la piste / remontee a exclure \n  => Commandes : save ou reset" << std::endl;
        std::cout << "\n  Votre choix : ";

        while(getch() != 13)
        {
            std::cin >> nom_trajet;

            if (nom_trajet == "save")//FONCTION SAVE DES INFORMATIONS ENREGISTRES PAR L'ADMIN
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

            else if(nom_trajet == "reset") //FONCTION RESET DES INFORMATIONS RENTREES PAR L'ADMIN
            {

                piste_enlevee.clear();  //On vide le vecteur de piste enlevee
                m_trajets = sauvegarde_trajets; //et on remet tous les trajets initiaux

                std::ofstream fichier ("admin.txt");

                std::cout << "\n   Reset reussi !" << std::endl;
                break;

            }

            else
            {

                for(auto x : m_trajets)
                {
                    if (x->getNom() == nom_trajet)
                    {
                        //On verifie que les trajets existent si oui on enleve les pistes
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

    else
    {
        //On ouvre le fichier en lecture
        std::ifstream ifs{"users.txt",std::ios::in};
        if (!ifs)
            throw std::runtime_error( "Impossible d'ouvrir en lecture le fichier users.txt" );

        while(!ifs.eof())//Tant que le fichier n'est pas termin??
        {
            //On reinitialise les vecteurs contenant les informations sur les pistes exclues et preferences.
            tampon_piste.clear();
            tampon_pref.clear();

            ifs >> tampon_nom; //On enregistre les informations
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
            //Si l'identite existe, on sort de la boucle.
            if(identite_user == identite_lambda)
            {
                break;
            }

        }

        //On enregistre toutes les informations obtenues.
        if(identite_user == identite_lambda)
        {
            m_preference = tampon_pref;
            piste_enlevee = tampon_piste;

            m_connexion = true;

            std::cout << "\n  Connexion reussie !" << std::endl;
        }
        //Sinon message d'erreur de connexion
        else if(identite_user != identite_admin)
        {
            std::cout << "\n  Vous n'etes pas enregistre !" << std::endl;
        }
    }
}
//Fonction de deconnexion de l'utilisateur
void Graphe::deconnexion()
{
    if(m_connexion == true)
    {
        //on reinitalise tout.

        m_preference = m_preference_initial;

        piste_enlevee.clear();

        std::cout << "\n  Deconnexion reussie !" << std::endl;
    }

    else
    {
        //Si on est pas connectes, message d'erreur.
        std::cout << "\n  Vous n'etes meme pas connectes..." << std::endl;
    }
}

///Fonction de sauvegarde des donnees de l'utilisateur
void Graphe::sauvegarde()
{
    //Initialisation variables
    std::string nom,date;
    std::vector<std::string> pref;

    //Menu
    std::cout << "\n  SAUVEGARDE DE VOS DONNEES\n" << std::endl;
    std::cout << "  Votre nom : ";
    std::cin >> nom;
    std::cout << "  Votre date de naissance (JJ/MM/AAAA) : ";
    std::cin >> date;

    //Ouverture du fichier en ecriture.
    std::ofstream fichier("users.txt", std::ios::out | std::ios::app);

    //Voir si on est au debut du fichier pour mettre un retour a la ligne ou non
    int position = fichier.tellp();

    fichier << "\n";

    //Sauvegarde de toutes les informations (nom,date de naissance, preferences, pistes exclues)
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

    if (piste_enlevee.size() != 0)
    {
        fichier <<"\n";

        for(auto x : piste_enlevee)
        {
            fichier << x;
            fichier << "\n";
        }
    }

    std::cout << "\n  Sauvegarde reussie !" << std::endl;
}

///DESTRUCTEUR DU GRAPHE
Graphe::~Graphe()
{
    for (auto s : m_sommets)
        delete s;
    //dtor
}
