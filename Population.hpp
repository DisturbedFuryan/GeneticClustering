#ifndef POPULATION_HPP
#define POPULATION_HPP

class Individual;
class Object;

#include <vector>

using namespace std;

//======================================================================================================================
class Population {
public:
    Population( const vector<Object*>* objects );
    ~Population();
    
    void CalculateFitnessForIndividuals();
    
    void SelectPairsAndCrossThem();
    
    void Mutate();
    
    void SendBestResultToFile();
    
    void Print() const;

private:
    Individual** m_individuals;
    
    /* Ilość elementów, jakie zawiera pojedynczy osobnik. */
    unsigned m_individualSize;
    
    /* Najlepsza i najgorsza jakość osobników w populacji. */
    double m_bestFitness, m_worstFitness;
    
    /* Wybiera rodzica. Można wskazać unikanego osobnika, żeby uniknąć krzyżowania danego osobnika z samym sobą. */
    Individual* FindParent( const Individual* avoidedParent = nullptr ) const;
    
    /* Krzyżuje osobnika a z b zapisując wynik do destinationA i destinationB. */
    void Crossover( Individual* a, Individual* b, int* destinationA, int* destinationB );
};
//======================================================================================================================

#endif
