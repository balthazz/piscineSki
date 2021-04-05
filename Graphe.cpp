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

    for(auto y : m_sommets)
    {
        y->afficher();
        std::cout << "\n" << std::endl;
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
    file.push(std::make_pair(m_sommets[depart],0));

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
    std::cout<<"\n Le chemin le plus court entre le sommet "<< depart <<" et le sommet "<< arrivee <<" est de : "<< m_sommets[arrivee]->getDistance() << " poids." << std::endl; //on affiche le plus court chemin entre les deux sommets choisis

    ///Chemin parcouru
    std::cout << "\n Le chemin est le suivant : " << arrivee;

    //On parcourt notre vecteurs de predecesseurs.
    for (int p = pred[arrivee]; p != -1; p = pred[p])
    {
        std::cout << " <- " << p;
    }
    std::cout << "\n";
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

Graphe::~Graphe()
{
    for (auto s : m_sommets)
        delete s;
    //dtor
}
