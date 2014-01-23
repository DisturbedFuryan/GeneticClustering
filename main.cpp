#include "GeneticClustering.hpp"
#include <cstdlib>
#include <ctime>


//**************************************************************************************************
int main( int argc, char** argv ) {
    // Wsparcie dla generatora liczb losowych ze strony czasu.
    srand( time( nullptr ) );
    
    GeneticClustering geneticClustering;
    
    geneticClustering.Start();
    
    return 0;
}
//**************************************************************************************************
