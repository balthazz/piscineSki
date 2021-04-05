#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <fstream>

#define valeur_max 9999

class Sommet
{
private :
    int m_num;
    std::string m_nom;
    double m_altitude;
    bool m_marque = false;
    double m_distance;
    ///chaque sommet possède la liste de ses successeurs et de ses prédecesseurs(des vecteurs de pointeurs sur Sommet)
    std::vector<std::pair<Sommet*,double>> m_predecesseurs;
    std::vector<std::pair<Sommet*,double>> m_successeurs;

public :
    /*constructeur*/
    Sommet(int num):m_num{num} {};
    /*accesseurs*/
    int getNum()const
    {
        return m_num;
    }
    ///accesseur : pour la liste des successeurs
    std::vector<std::pair<Sommet*,double>>& getSuccesseurs()
    {
        return m_successeurs;
    }
    std::vector<std::pair<Sommet*,double>>& getPredecesseurs()
    {
        return m_predecesseurs;
    }
    /* pour ajouter un successeur à la liste*/
    void ajouterSucc(std::pair<Sommet*,double> p)
    {
        m_successeurs.push_back(p);
    }
    void ajouterPred(std::pair<Sommet*,double> p)
    {
        m_predecesseurs.push_back(p);
    }
    void setMarque(bool valeur)
    {
        m_marque = valeur;
    }

    bool getMarque()const
    {
        return m_marque;
    }

    void setDistance(double value)
    {
        m_distance = value;
    }

    double getDistance()const
    {
        return m_distance;
    }
    void setNom(std::string nom)
    {
        m_nom = nom;
    }

    std::string getNom()const
    {
        return m_nom;
    }

    void setAltitude(double altitude)
    {
        m_altitude = altitude;
    }

    double getAltitude()const
    {
        return m_altitude;
    }


    /* méthode d'affichage*/
    void afficher() const
    {
        std::cout<<"     sommet "<<m_num<<" : \n";
        for (auto s : m_successeurs)
        {
            std::cout<<"\nDistance du sommet " << s.first->getNum()<<" : ";
            std::cout<<s.second<<" poids";
        }
    }

};


//Définition de la classe arete
class Trajet{

  private:
      std::pair<Sommet*,Sommet*> m_extremites;
      int m_id;
      std::string m_nom;
      std::string m_type;
      double m_poids;
      bool m_marque;

  public:
      //Constructeur
      Trajet(std::pair<Sommet*,Sommet*> extremites,int id, std::string nom, std::string type):m_extremites{extremites},m_id{id},m_nom{nom},m_type{type}{};

    //Accesseurs
    void setPoids(double poids)
    {m_poids = poids;}

    double getPoids()
    {return m_poids;}

    void setMarque(double marque)
    {m_marque = marque;}

    double getMarque()
    {return m_marque;}

    void setExtremites(std::pair<Sommet*,Sommet*> extremites)
    {m_extremites = extremites;}

    std::pair<Sommet*,Sommet*> getExtremites()
    {return m_extremites;}

    int getId()
    {return m_id;}

    void setId(int id)
    {m_id = id;}

    std::string getNom()
    {return m_nom;}

    void setNom(std::string nom)
    {m_nom = nom;}

    std::string getType()
    {return m_type;}

    void setType(std::string type)
    {m_type = type;}

    void afficher()
    {

        std::cout << m_id << "  "<< m_nom << "   " << m_type << "  " <<m_extremites.first->getNum() << " <--> " << m_extremites.second->getNum();
    }

};

class Graphe
{

private :
    ///liste des sommets (vecteur de pointeurs sur Sommet)
    std::vector<Sommet*> m_sommets;
    std::vector<Trajet*> m_trajets;

public :
    /* La construction du réseau peut se faire à partir d'un fichier
     dont le nom est passé en paramètre
    Le fichier contient :  ordre, taille, liste des arcs et leurs poids*/
    Graphe(std::string nomFichier)
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

            extremites.first = m_sommets[sommet_1];
            extremites.second = m_sommets[sommet_2];

            m_trajets.push_back(new Trajet{extremites,id_trajet,nom_trajet,type_trajet});

        }

    }
    /*destructeur*/
    ~Graphe()
    {
        for (auto s : m_sommets)
            delete s;
    }

    //Fonction d'affichage de notre chargement de fichier graphe.txt
    void afficher_sommet() const
    {
        std::cout<<"\n    Liste des sommets :\n"<<std::endl;
        for (auto s : m_sommets)
        {
            std::cout  <<s->getNum() << "     " <<s->getNom() << "     " << s->getAltitude() << std::endl;
        }
    }

    void afficher_arc() const // Fonction d'affichage des aretes
    {
        std::cout<<"\n    Liste des trajets :\n"<<std::endl;

        for (auto s : m_trajets)
        {
            std::cout << "\n";
            s->afficher();

        }
            std::cout << "\n";
    }


    void Dijkstra(int depart,int arrivee)
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

};





int main()
{
    int depart, arrivee;
    Graphe g{"data_arcs.txt"};

    ///Affichage du chargement du graphe.
    g.afficher_sommet();
    g.afficher_arc();


//    std::cout << "\n            Projet Piscine" << std::endl;
//    std::cout<<"\n Sommet de depart : ";
//    std::cin >> depart;
//    std::cout<<"\n Sommet d'arrivee : ";
//    std::cin >> arrivee;

    ///Appel de l'algorithme de Dijkstra
    //g.Dijkstra(depart,arrivee);

    return 0;

}
