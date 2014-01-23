#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <cstdlib>

//======================================================================================================================
class Utility {
public:
    static const bool debug;
    
    static const int k;
    
    static const int populationSize;
    
    static const int generationsNumber;
    
    static const double mutationProbability;
    
    static const int uniqueInitialPopulationTries;
    
    static const int maxCrossoverTries;
    
    static const double infinity;
    
    static int RandBetween( const long min, const long max ) { return ( ( rand() % ( max - min + 1 ) ) + min ); }
    
    /* Losuje wartości z przedziału (0;1). */
    static double RandSelect() { return static_cast<double>( rand() / ( RAND_MAX * 1.0d ) ); }
    
    /* Sprawdza, czy w podanym przyporządkowaniu zostały wzięte pod uwagę wszystkie k klastry. */
    static bool IsAssignationCorrect( const int* assignation, const unsigned size );
};
//======================================================================================================================

#endif
