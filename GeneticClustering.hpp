#ifndef KMEANS_HPP
#define KMEANS_HPP

class Object;
class Population;

#include <vector>

using namespace std;

//======================================================================================================================
class GeneticClustering {
public:
    ~GeneticClustering();
    
    void Start();
    
private:
    Population* m_population;
    
    vector<Object*> m_objects;
    
    Population* CreateInitialPopulation();
    
    /* Pobiera obiekty z pliku. */
    void GetObjects();
};
//======================================================================================================================

#endif
