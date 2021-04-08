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

    ///Liste des successeurs pour vérification
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



void Graphe::Dijkstra(int depart,int arrivee,std::vector<std::string> preference)
{

    ///initialisations
    //initialisation de notre tampon pair utilisé dans l'algo.
    std::pair<Sommet*,double> p;
    bool comparaison;
    Trajet* nvxTrajet;

    //on initialise le vecteur des predecesseurs pour chaque sommet avec la valeur -1.
    std::vector<int> pred(m_sommets.size(),-1);

    //Initialisation marquage des sommets pour les dijkstra multiples
    for(auto x : m_sommets)
    {
        x->setMarque(false);
    }

    //Déclaration de la fonction de comparaison utilisée par la priority_queue
    auto compare = [](std::pair<Sommet*,double> a, std::pair<Sommet*,double> b)
    {
        return b.second < a.second;
    };

    //Déclaration de la file de priorité
    std::priority_queue<std::pair<Sommet*,double>, std::vector<std::pair<Sommet*,double>>, decltype(compare)> file(compare);

    //initialise tous les sommets à valeur théoriquement infinie sauf celui de départ.
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

    //Insérer le sommet initial dans la file avec une distance nul car on ne bouge pas.
    file.push(std::make_pair(m_sommets[depart-1],0));

    ///Boucle de recherche

    while (!file.empty()) //tant que notre file n'est pas vide on continue.
    {
        p = file.top(); //je considére que p est le successeur avec le plus cours chemin, je le choisi donc pour le reste de la boucle
        file.pop();//je le supprime pour ne pas retomber dessus

        if (p.first->getMarque()==false) // si le sommet n'a pas été marqué
        {
            p.first->setMarque(true); //on marque le sommet

            for (auto succ : p.first->getSuccesseurs()) //pour chaque successeur :
            {
                Trajet* nvxTrajet = trajet_avec_ses_succ(p.first,succ.first);

                for(auto pref : preference)
                {
                    if(!pref.compare(nvxTrajet->getType()))
                    {
                        comparaison = true;
                        break;
                    }
                    else
                    {
                        comparaison = false;
                    }
                }


                if (succ.first->getMarque()== false && comparaison) // si le successeur n'a pas été marqué
                {
                    // on calcule le chemin parcourure jusqu'au sommet s, on additionne le poid du chemin jusqu'à ce predecesseur
                    double addition = p.first->getDistance() + succ.second;

                    if (addition < succ.first->getDistance()) //si ce chemin est plus petit que le précédent chemin enregistré alors on le remplace
                    {
                        succ.first->setDistance(addition); //on définit la nouvelle distance (plus courte) entre le départ et ce nouveau point(qui est un successeur du sommet qu'on étudiait)
                        pred[succ.first->getNum()] =  p.first->getNum(); // On sauvegarde le predecesseur pour avoir une trace de notre chemin.
                        file.push(succ); //Enfin, on rajoute notre successeur à la file de priorité.

                    }
                }
            }
        }
    }
    ///Affichage des résultats
    ///Valeur du poids
    std::cout<<"\n\n Le chemin le plus court entre le sommet "<< depart <<" et le sommet "<< arrivee <<" est de : "<< m_sommets[arrivee-1]->getDistance() << " minutes." << std::endl; //on affiche le plus court chemin entre les deux sommets choisis

    ///Chemin parcouru
    std::cout << "\n" << arrivee;

    int x = arrivee;

    //On parcourt notre vecteurs de predecesseurs.
    for (int p = pred[arrivee]; p != -1; p = pred[p])
    {
        if( p <= m_sommets.size())
        {

            std::cout << " <- " << Type_Chemin_S1_S2(p,x) << " " << Nom_Chemin_S1_S2(p,x) << " <- " << p;
            x = p;

        }

        else
        {
            break;
        }
    }
    std::cout << "\n";
}


/// Parcours BFS
std::vector<int> Graphe::BFS(int num_S0,std::vector<std::string> preference)
{

    std::queue<Sommet*>pile;
    Trajet* nvxTrajet;
    // pour le marquage
    std::vector<int> couleurs((int)m_sommets.size(),0);
    //pour noter les prédécesseurs
    std::vector<int> predec((int)m_sommets.size(),-1);

    bool comparaison;

    //étape initiale
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
            nvxTrajet = trajet_avec_ses_succ(so,succ.first);

            for(auto pref : preference)
            {
                if(!pref.compare(nvxTrajet->getType()))
                {
                    comparaison = true;
                    break;
                }
                else
                {
                    comparaison = false;
                }
            }

            if(couleurs[succ.first->getNum()]==0 && comparaison) //s'il n'est pas marqué et pas dans preference
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
    for(int i=0; i<m_trajets.size(); i++)
    {
        std::pair<Sommet*,Sommet*> tampon=m_trajets[i]->getExtremites();
        if((tampon.first->getNum()==s1) && (tampon.second->getNum()==s2))
        {
            return m_trajets[i]->getNom();
        }
    }
}

std::string Graphe::nom_du_Trajet_avec_Id(int num)
{
    for(int i=0; i<m_trajets.size(); i++)
    {
        if(m_trajets[i]->getId()==num)
        {
            std::string tampon=m_trajets[i]->getNom();
            return tampon;
        }
    }
}

Sommet* Graphe::sommet_avec_son_Id(int num)
{
    for(int i=0; i<m_sommets.size(); i++)
    {
        if(m_sommets[i]->getNum()==num)
        {
            return m_sommets[i];
        }
    }
}

Trajet* Graphe::trajet_avec_son_Id(int id)
{
    Trajet* tampon;
    for(int i=0; i<m_trajets.size(); i++)
    {
        if(m_trajets[i]->getId()==id)
        {
            tampon=m_trajets[i];
            return tampon;
        }
    }
}

std::string Graphe::Type_Chemin_S1_S2(int s1,int s2)
{
    for(int i=0; i<m_trajets.size(); i++)
    {
        std::pair<Sommet*,Sommet*> tampon=m_trajets[i]->getExtremites();
        if((tampon.first->getNum()==s1) && (tampon.second->getNum()==s2))
        {
            return m_trajets[i]->getType();
        }
    }
}


double Graphe::Poids_Chemin_S1_S2(int s1,int s2)
{
    for(int i=0; i<m_trajets.size(); i++)
    {
        std::pair<Sommet*,Sommet*> tampon=m_trajets[i]->getExtremites();
        if((tampon.first->getNum()==s1) && (tampon.second->getNum()==s2))
        {
            return m_trajets[i]->getPoids();
        }
    }
}


int Graphe::Num_du_Sommet_avec_Nom(std::string nom)
{
    for(int i=0; i<m_sommets.size(); i++)
    {
        if(m_sommets[i]->getNom()==nom)
        {
            int tampon=m_sommets[i]->getNum();
            return tampon;
        }
    }
}


int Graphe::Id_du_Trajet_avec_Nom(std::string nom)
{
    for(int i=0; i<m_trajets.size(); i++)
    {
        if(m_trajets[i]->getNom()==nom)
        {
            int tampon=m_trajets[i]->getId();
            return tampon;
        }
    }
}

Trajet* Graphe::trajet_avec_ses_succ(Sommet* som1,Sommet* som2)
{
    for(int i=0; i<m_trajets.size(); i++)
    {
        if((m_trajets[i]->getExtremites().first==som1)&&(m_trajets[i]->getExtremites().second==som2))
        {
            return m_trajets[i];
        }
    }
}

void Graphe::afficher1ParcoursBFS(size_t num, size_t num2, std::vector<int>& arbre)
{
    double poids;
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
                    size_t tmp = j;
                    j=arbre[j];
                    if(i==num2)
                    {
                        tampon.push_back(j);
                    }
                }
            }
        }

    }
    for(int i=0; i<tampon.size(); i++)
    {
        int tmp;
        tmp=tampon[i+1];
        std::string m=Type_Chemin_S1_S2(tmp,tampon[i]);
        std::string n=Nom_Chemin_S1_S2(tmp,tampon[i]);
        if(tampon[i]!=num)
        {
            std::cout<<tampon[i]<<"<--";
            couleur.couleur_type(m);
            std::cout<<m<<" ";
            std::cout<<n<<" ";
            couleur.couleur(15);
            couleur.setColor(0);
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
    double poids;
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
    for(int i=0; i<m_trajets.size(); i++)
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
        std::cout << "\nErreur sur le trajet renseigne !\n" << std::endl;
    }
}


void Graphe::infoSommet()
{
    std::string nomSommet;
    std::cout<<"\n   Sur quel numero de sommet souhaitez vous etre renseigne ?"<< std::endl;
    std::cout<<"\n   Votre choix : ";
    std::cin >> nomSommet;
    std::vector<std::string> Trajet_entrant;
    std::vector<std::string> Trajet_sortant;
    for(int i=0; i<m_trajets.size(); i++)
    {
        if(m_trajets[i]->getExtremites().first->getNom()==nomSommet)
        {
            Trajet_sortant.push_back(m_trajets[i]->getNom());
        }
        if(m_trajets[i]->getExtremites().second->getNom()==nomSommet)
        {
            Trajet_entrant.push_back(m_trajets[i]->getNom());
        }
    }
    std::cout<<"\n Trajets qui arrivent a la station "<<nomSommet<<" : ";
    for(int i=0; i<Trajet_entrant.size(); i++)
    {
        std::cout<<Trajet_entrant[i]<<"  ";
    }
    std::cout<<"\n\n Trajets qui partent a la station "<<nomSommet<<" : ";
    for(int i=0; i<Trajet_sortant.size(); i++)
    {
        std::cout<<Trajet_sortant[i]<<"  ";
    }
    std::cout<<"\n\n\n";

}


void Graphe::personnaliser()
{
    int choix_preference;
    std::string choix_niveau;
    std::string remontee;

    std::cout<<"\n    PARCOURS PERSONNALISE \n\n";
    std::cout<<"  1. Preferences des pistes" << std::endl;
    std::cout<<"  2. Preferences des remontees" << std::endl;
    std::cout<<"\n   Votre choix : ";
    std::cin >> choix_preference;

    if(choix_preference == 1)
    {
        std::cout<<"\n  Quel est votre niveau ?  \n\n  debutant   =>   Vertes + Bleues    \n  intermediaire   =>   Bleues + Rouges + Snowparks  \n  expert   =>   Tout, meme les noires :)!\n";
        std::cout<<"\n  Votre choix : ";

        std::cin>>choix_niveau;

        if(choix_niveau=="debutant")
        {
            for(int i = 0 ; i < m_preference.size() ; i++)
            {
                if ((m_preference[i] == "N") || (m_preference[i] == "R") || (m_preference[i] == "SURF"))
                {
                    m_preference.erase(m_preference.begin()+i);
                }
            }
        }


        if(choix_niveau=="intermediaire")
        {
            m_preference.push_back("R");
            m_preference.push_back("SURF");

            for(int i = 0 ; i < m_preference.size() ; i++)
            {
                if (m_preference[i] == "N")
                {
                    m_preference.erase(m_preference.begin()+i);
                }
            }
        }


        if(choix_niveau=="expert")
        {
            m_preference.push_back("V");
            m_preference.push_back("B");
            m_preference.push_back("R");
            m_preference.push_back("N");
            m_preference.push_back("SURF");
        }

        //std::cout<<"\n nouveau nombre de sommets : "<<m_trajets.size()<<std::endl;

//    for(auto y : m_sommets)
//    {
//        y->afficher();
//        std::cout << "\n" << std::endl;
//    }
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


            for(int i = 0 ; i < m_preference.size() ; i++)
            {
                if (m_preference[i] == remontee)
                {
                    m_preference.erase(m_preference.begin()+i);
                }
            }

            system("cls");

            std::cout << "\n   Liste des remontees : " << std::endl;
            std::cout << "\n 1. Teleski \n 2. Telesiege \n 3. Telepherique \n 4. Telecabine \n 5. Telesiege Debrayable \n 6. Bus" << std::endl;
            std::cout << "\n  Votre choix (Retour -> Enter) : ";

        }



    }
}

void Graphe::kruskal()
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

    while(resultat_krustal.size() < m_sommets.size()-4) // Tant que notre arbre de résultats est inférieur au nombre de sommets -1
    {
        tampon = trajets_pistes[nombre_tour]->getExtremites(); // On récupere les extremites de l'arete de poids le plus haut

        if(tampon.first->getNumCC() != tampon.second->getNumCC()) //On vérifie si elle peut être sélectionnée
        {
            resultat_krustal.push_back(trajets_pistes[nombre_tour]); // Si oui, on ajoute l'arete à notre arbre de poids couvrant minimum

            //Check et actualisation des numCC

            int x = tampon.second->getNumCC(); // On sauvegarde notre point de comparaison

            for(int k = 0 ; k < m_sommets.size() ; k++) // Pour tous les sommets
            {
                if (m_sommets[k]->getNumCC() == x) // Si les NumCC sont égaux, on remplace
                {
                    m_sommets[k]->setNumCC(tampon.first->getNumCC());
                }
            }
        }

        nombre_tour++; // On incrémente le nombre de tour
    }


    //Affichage du résultat !

    std::cout << "\n Chemin maximum couvrant la station : \n" << std::endl;

    for(auto x : resultat_krustal)
    {
        poidsTotal += x->getPoids(); //On incrémente le poids total pour chaque sommet dans l'arbre

        x->afficher(); //On affiche les arêtes et leurs poids

        std::cout << "\n";
    }

    std::cout << "\n Le temps total est de : " << poidsTotal << " minutes" << std::endl; // Affichage du poids total de l'arbre

//        std::cout << trajets_pistes.size() << std::endl;
//        std::cout << resultat_krustal.size() << std::endl;
}

void Graphe::connexion()
{
    std::string nom, date;
    int taille_pref;
    std::string valeur_pref;
    std::vector<std::string> tampon_pref;
    std::string tampon_nom,tampon_date;
    std::pair<std::string,std::string> idendite_user, identite_lambda;

    std::cout << "\n CONNEXION ESPACE UTILISATEUR" << std::endl;
    std::cout << "\n  Nom de famille : ";
    std::cin >> nom;
    std::cout << "\n  Date de naissance (JJ/MM/AAAA) : ";
    std::cin >> date;

    idendite_user.first = nom;
    idendite_user.second = date;

    std::ifstream ifs{"users.txt"};
    if (!ifs)
        throw std::runtime_error( "Impossible d'ouvrir en lecture le fichier users.txt" );

    do
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


    }
    while((ifs) || (idendite_user != identite_lambda));

    if(idendite_user == identite_lambda)
    {
        m_preference = tampon_pref;

        m_connexion = true;

        std::cout << "\n  Vous etes connecte !" << std::endl;
    }

    else
    {
        std::cout << "\n  Vous n'etes pas enregistre !" << std::endl;
    }
}

void Graphe::deconnexion()
{
    if(m_connexion == true)
    {
        m_preference = m_preference_initial;
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

//        for(int i = 0 ; i < m_preference_initial ; i++)
//        {
//            for(int j = 0 ; j < m_preference ; j++)
//            {
//                if
//            }
//        }

    std::cout << "\n  SAUVEGARDE DE VOS DONNEES\n" << std::endl;
    std::cout << "  Votre nom : ";
    std::cin >> nom;
    std::cout << "  Votre date de naissance (JJ/MM/AAAA) : ";
    std::cin >> date;

    std::ofstream fichier("users.txt", std::ios::out | std::ios::app);
    //fichier.seekg(0, std::ios::end);

    fichier << nom;
    fichier << date;
    fichier << "\n" << m_preference.size();

    for(auto x : m_preference)
    {
        fichier<< "\n";
        fichier << x;
    }

    fichier << "\n";

    std::cout << "\n  Sauvegarde reussie !" << std::endl;
}





Graphe::~Graphe()
{
    for (auto s : m_sommets)
        delete s;
    //dtor
}
