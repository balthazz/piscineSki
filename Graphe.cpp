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

    for(int i = 0 ; i < taille ; i++)
    {
        ifs>>id_trajet>>nom_trajet>>type_trajet>>sommet_1>>sommet_2;

        extremites.first = m_sommets[sommet_1-1];
        extremites.second = m_sommets[sommet_2-1];

        m_trajets.push_back(new Trajet{extremites,id_trajet,nom_trajet,type_trajet});

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

    ///Liste des successeurs pour vérification
//    for(auto y : m_sommets)
//    {
//        y->afficher();
//        std::cout << "\n" << std::endl;
//    }

}


void Graphe::Dijkstra(int depart,int arrivee)
{

    ///initialisations
    //initialisation de notre tampon pair utilisé dans l'algo.
    std::pair<Sommet*,double> p;

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

                if (succ.first->getMarque()== false) // si le successeur n'a pas été marqué
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
    std::cout<<"\n Le chemin le plus court entre le sommet "<< depart <<" et le sommet "<< arrivee <<" est de : "<< m_sommets[arrivee-1]->getDistance() << " minutes." << std::endl; //on affiche le plus court chemin entre les deux sommets choisis

    ///Chemin parcouru
    std::cout << "\n Le chemin est le suivant : " << arrivee;

    int x = arrivee;

    //On parcourt notre vecteurs de predecesseurs.
    for (int p = pred[arrivee]; p != -1; p = pred[p])
    {
        if( p <= m_sommets.size())
        {

        std::cout << " <- " << Type_Chemin_S1_S2(p,x) << " " << Nom_Chemin_S1_S2(p,x) << " <- " << p;
        x = p;

        }

        else{break;}
    }
    std::cout << "\n";
}


/// Parcours BFS
std::vector<int> Graphe::BFS(int num_S0)
{
    std::queue<Sommet*>pile;
    // pour le marquage
    std::vector<int> couleurs((int)m_sommets.size(),0);
    //pour noter les prédécesseurs
    std::vector<int> predec((int)m_sommets.size(),-1);
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
            if(couleurs[succ.first->getNum()]==0) //s'il n'est pas marqué
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

Trajet* Graphe::trajet_avec_son_Id(int id)
{
    Trajet* tampon;
    for(int i=0;i<m_trajets.size();i++)
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

/*void Graphe::Effacer_successeur_sommet_quand_efface_trajet(Trajet* trajet_efface)
{
    std::vector<std::pair<Sommet*,double>> succTampon;
    trajet_efface->getExtremites().first;
    for(int i=0;i<m_sommets.size();i++)
    {
        if(m_sommets[i]->getSuccesseurs()!=trajet_efface->getExtremites().second)
        {
            succTampon=
        }
    }
} */

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
    for(int i=0;i<tampon.size();i++)
    {
        int tmp;
        tmp=tampon[i+1];
        std::string m=Type_Chemin_S1_S2(tmp,tampon[i]);
        std::string n=Nom_Chemin_S1_S2(tmp,tampon[i]);
        if(tampon[i]!=num)
        {
            std::cout<<tampon[i]<<"<--"<<m<<" "<<n<<"<--";
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
                std::cout<<Type_Chemin_S1_S2(j,i)<<" "<<Nom_Chemin_S1_S2(j,i)<<" <- ";
                poids=Poids_Chemin_S1_S2(j,i);
                while(j!=num)
                {
                    std::cout<<j<<"<- ";
                    size_t tmp = j;
                    j=arbre[j];
                    std::string m=Type_Chemin_S1_S2(j,tmp);
                    std::string n=Nom_Chemin_S1_S2(j,tmp);
                    poids=poids+Poids_Chemin_S1_S2(j,tmp);
                    std::cout<<m<<" "<<n<<" <-";
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
    std::cout<<"\nSur quel trajet souhaitez vous etre renseigne ?"<< std::endl;
    std::cout<<"\nVotre choix : ";
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
    std::cout<<"Sur quel numero de sommet souhaitez vous etre renseigne ?"<< std::endl;
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
    std::string choix_niveau;
    std::vector<Trajet*> trajet_personnalise;
    std::vector<Sommet*> copie = m_sommets;
    std::vector<Sommet*> newSommet;
    std::cout<<"\n PARCOURS PERSONNALISE \n\n";
    std::cout<<"Quel est votre niveau ?  debutant ( Verte + Bleue )   intermediaire ( Bleue + Rouge )  expert ( Rouge + Noire ) \n";
    std::cin>>choix_niveau;
    if(choix_niveau=="debutant")
    {
        for(int i=0;i<m_trajets.size();i++)
        {
            if((m_trajets[i]->getType()=="V") || (m_trajets[i]->getType()=="B"))
            {
                trajet_personnalise.push_back(m_trajets[i]);
            }
            else if((m_trajets[i]->getType()=="KL") || (m_trajets[i]->getType()=="SURF") || (m_trajets[i]->getType()=="TPH")|| (m_trajets[i]->getType()=="TC")|| (m_trajets[i]->getType()=="TSD")|| (m_trajets[i]->getType()=="TS")|| (m_trajets[i]->getType()=="TK")|| (m_trajets[i]->getType()=="BUS"))
            {
                trajet_personnalise.push_back(m_trajets[i]);
            }
        }
        m_trajets=trajet_personnalise;

        for(int i=0;i<m_trajets.size();i++)
        {
            std::cout<<m_trajets[i]->getType()<<" "<<m_trajets[i]->getNom()<<"\n";
        }
        std::cout<<m_trajets.size()<<"\n";

    }
    if(choix_niveau=="intermediaire")
    {
        for(int i=0;i<m_trajets.size();i++)
        {
            if((m_trajets[i]->getType()=="V") || (m_trajets[i]->getType()=="B") || (m_trajets[i]->getType()=="R"))
            {
                trajet_personnalise.push_back(m_trajets[i]);
            }
            else if((m_trajets[i]->getType()=="KL") || (m_trajets[i]->getType()=="SURF") || (m_trajets[i]->getType()=="TPH")|| (m_trajets[i]->getType()=="TC")|| (m_trajets[i]->getType()=="TSD")|| (m_trajets[i]->getType()=="TS")|| (m_trajets[i]->getType()=="TK")|| (m_trajets[i]->getType()=="BUS"))
            {
                trajet_personnalise.push_back(m_trajets[i]);
            }
        }
        m_trajets=trajet_personnalise;

        for(int i=0;i<m_trajets.size();i++)
        {
            std::cout<<m_trajets[i]->getType()<<" "<<m_trajets[i]->getNom()<<"\n";
        }
        std::cout<<m_trajets.size()<<"\n";
    }
    if(choix_niveau=="expert")
    {
        for(int i=0;i<m_trajets.size();i++)
        {
            if((m_trajets[i]->getType()=="V") || (m_trajets[i]->getType()=="B") || (m_trajets[i]->getType()=="R") || (m_trajets[i]->getType()=="N"))
            {
                trajet_personnalise.push_back(m_trajets[i]);
            }
        }
        m_trajets=trajet_personnalise;

        for(int i=0;i<m_trajets.size();i++)
        {
            std::cout<<m_trajets[i]->getType()<<" "<<m_trajets[i]->getNom()<<"\n";
        }
        std::cout<<m_trajets.size()<<"\n";
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
        std::sort(trajets_pistes.begin(),trajets_pistes.end(),[](Trajet* a, Trajet* b){return a->getPoids() > b->getPoids();});

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

Graphe::~Graphe()
{
    for (auto s : m_sommets)
        delete s;
    //dtor
}
