#ifndef INDIVIDUAL_HPP
#define INDIVIDUAL_HPP

class Object;
class Vector;

#include <vector>

using namespace std;

//======================================================================================================================
class Individual {
public:
    static bool AreSame(const Individual& a, const Individual& b);
    
    Individual(const vector<Object*>* objects);
    ~Individual();
    
    /* Sprawdza jakość osobnika. */
    double CheckFitness();
    
    /* Sprawdza czy osobnik jest poprawny, to jest - przyporządkowuje elementy do wszystkich dostępnych klastrów. */
    bool IsCorrect() const;
    
    void Print() const;
    
    double Quality() const { return m_fitness; }
    unsigned Identifier() const { return m_id; }
    int* AssignedClusters() const { return m_proposedClusters; }
    Object** Objects() const { return m_points; }
    Vector** Centers() const { return m_respectiveCenters; }

private:
    /* Jakość osobnika. */
    double m_fitness;
    
    /* Elementy, którym będą przyporządkowywane klastry. */
    Object** m_points;
    
    /* Tablica zawierająca przyporządkowanie elementów do klastrów. */
    int* m_proposedClusters;
    
    /* Środki dla każdego z klastrów. */
    Vector** m_respectiveCenters;
    
    /* Liczba elementów w osobniku. */
    unsigned m_size;
    
    static unsigned m_individualsNumber;
    
    unsigned m_id;
    
    void FindCenters();
};
//======================================================================================================================

#endif
