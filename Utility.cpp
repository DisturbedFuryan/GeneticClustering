#include "Utility.hpp"
#include <limits>

using namespace std;


const bool Utility::debug = false;
    
const int Utility::k = 3;

const int Utility::populationSize = 20;

const int Utility::generationsNumber = 1000;

const double Utility::mutationProbability = 0.005d;

const int Utility::uniqueInitialPopulationTries = 100;

const int Utility::maxCrossoverTries = 1000;

const double Utility::infinity = numeric_limits<double>::max();


bool Utility::IsAssignationCorrect( const int* assignation, const unsigned size ) {
    bool isHere[k];
    
    for ( int i = 0; i < k; ++i ) {
        isHere[i] = false;
    }
    
    bool isCorrect;
    
    for ( unsigned u = 0; u < size; ++u ) {
        isHere[assignation[u]] = true;
        
        isCorrect = true;
        
        for ( int i = 0; i < k; ++i ) {
            if ( !isHere[i] ) {
                isCorrect = false;
            }
        }
        
        if ( isCorrect ) {
            return true;
        }
    }
    
    return false;
}
