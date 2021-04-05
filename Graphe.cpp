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

    //Liste des successeurs pour vérification
    for(auto y : m_sommets)
    {
        y->afficher();
        std::cout << "\n" << std::endl;
    }

}

///Affichage arborecsence
void afficherParcours(size_t num,const std::vector<int>& arbre)
{
    for(size_t i=0; i<arbre.size(); ++i)
    {
        if(i!=num)
        {
            if(arbre[i]!=-1)
            {
                std::cout<<i<<" <-- ";
                size_t j=arbre[i];
                while(j!=num)
                {
                    std::cout<<j<<" <-- ";
                    j=arbre[j];
                }
                std::cout<<j<<std::endl;
            }
        }
    }
}

void Graphe::Dijkstra(int depart,int arrivee)
{

    ///initialisations

    //initialisation de notre tampon pair utilisé dans l'algo.
    std::pair<Sommet*,double> p;

    //on initialise le vecteur des predecesseurs pour chaque sommet avec la valeur -1.
    std::vector<int> pred(m_sommets.size(),-1);

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
    std::cout<<"\n Le chemin le plus court entre le sommet "<< depart <<" et le sommet "<< arrivee <<" est de : "<< m_sommets[arrivee]->getDistance() << " minutes." << std::endl; //on affiche le plus court chemin entre les deux sommets choisis

    ///Chemin parcouru
    std::cout << "\n Le chemin est le suivant : " << arrivee;

    int x = arrivee;

    //On parcourt notre vecteurs de predecesseurs.
    for (int p = pred[arrivee]; p != -1; p = pred[p])
    {

        std::cout << " <- " << Type_Chemin_S1_S2(p,x) << " " << Nom_Chemin_S1_S2(p,x) << " <- " << p;
        x = p;
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
    for(int i=0;i<m_trajets.size();i++)
    {
        std::pair<Sommet*,Sommet*> tampon=m_trajets[i]->getExtremites();
        if((tampon.first->getNum()==s1) && (tampon.second->getNum()==s2))
           {
               return m_trajets[i]->getNom();
           }
    }
}
std::string Graphe::Type_Chemin_S1_S2(int s1,int s2)
{
    for(int i=0;i<m_trajets.size();i++)
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
    for(int i=0;i<m_trajets.size();i++)
    {
        std::pair<Sommet*,Sommet*> tampon=m_trajets[i]->getExtremites();
        if((tampon.first->getNum()==s1) && (tampon.second->getNum()==s2))
           {
               return m_trajets[i]->getPoids();
           }
    }
}


void Graphe::afficherParcours(size_t num,const std::vector<int>& arbre)
{
    double poids;
    for(size_t i=0; i<arbre.size(); ++i)
    {
        //for(int n=0;n<m_sommets.size();n++)
        //

            if(i!=num)
            {
                //size_t tmp=arbre[i];
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
                        poids=poids+Poids_Chemin_S1_S2(j,i);
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

void Graphe::infoTrajet() //question 4.3 petit 3-a
{
    std::string nomTrajet;
    std::cout<<"Sur quel trajet souhaitez vous etre renseigne ?"<< std::endl;
    std::cin >> nomTrajet;
    for (auto s : m_trajets)
    {
        if (s->getNom().compare(nomTrajet) == 0)
        {
            std::cout << std::endl << s->getNom() <<" "<< s->getType() << " part de " << s->getExtremites().first->getNum()<< " et arrive a " << s->getExtremites().second->getNum()<<std::endl;
        }
    }

}

void Graphe::infoSommet()//question 4.3 petit 3-b
{

    int nomSommet;
    std::cout<<"Sur quel numero de sommet souhaitez vous etre renseigne ?"<< std::endl;
    std::cin >> nomSommet;
     std::cout<<"infos sur "<< m_sommets[nomSommet-1]->getNom()<<" : "<< std::endl;
std::vector<std::pair<Sommet*,double>> tampon1 = m_sommets[nomSommet-1]->getSuccesseurs();
     std::cout<<"trajets arrivant a "<< m_sommets[nomSommet-1]->getNom()<<" : "<< std::endl;
       for (auto s : tampon1)
{
         for (auto p : m_trajets)
         {
             if ((p->getExtremites().second->getNum()==m_sommets[nomSommet-1]->getNum())&&(p->getExtremites().first->getNum() == s.first->getNum()))
                 {
                     std::cout<<"piste : "<< p->getNom() << " "<<p->getType()<<std::endl;
                 }
         }
     }

     std::cout<<"trajets partant de "<< m_sommets[nomSommet-1]->getNom()<<" : "<< std::endl;

     for (auto s : tampon1)
     {
         for (auto p : m_trajets)
         {

             if ((p->getExtremites().first->getNum()==m_sommets[nomSommet-1]->getNum())&&(p->getExtremites().second->getNum() == s.first->getNum()))
                 {
std::cout<<"piste : "<< p->getNom()<<" "<<p->getType() <<std::endl;
                 }

         }
     }


//    for (auto s : m_sommets[nomSommet-1]->getSuccesseurs())
//    {
//      std::cout<<"piste : "<< m_trajets[m_sommets[nomSommet-1],s]->getNom() <<std::endl;
//    }
}

Graphe::~Graphe()
{
    for (auto s : m_sommets)
        delete s;
    //dtor
}
