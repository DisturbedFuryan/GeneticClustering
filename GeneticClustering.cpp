#include "GeneticClustering.hpp"
#include "Object.hpp"
#include "Population.hpp"
#include "Timer.hpp"
#include "Utility.hpp"
#include <fstream>
#include <iostream>


GeneticClustering::~GeneticClustering() {
    delete m_population;
    
    for ( vector<Object*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it ) {
        delete *it;
    }
}


void GeneticClustering::Start() {
    Timer timer;
    
    GetObjects();
    
    m_population = CreateInitialPopulation();
    
    timer.Start();
    
    for ( int i = 0; i < Utility::generationsNumber; ++i ) {
        m_population->CalculateFitnessForIndividuals();
        
        timer.Pause();
        cout << "\nCREATING " << ( i + 1 ) << " GENERATION...\n\n";
        timer.Unpause();
        
        m_population->SelectPairsAndCrossThem();
        
        if ( Utility::debug ) {/////////////////////////////////////////////////////////////////////////////////////////
            cout << "\nPopulation after crossover:\n";
            m_population->Print();
            cout << endl;
        }///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        if ( i < ( Utility::generationsNumber - 1 ) ) {  // Ostatnie pokolenie nie przechodzi mutacji.
            m_population->Mutate();
            
            if ( Utility::debug ) {/////////////////////////////////////////////////////////////////////////////////////
                cout << "\nPopulation after mutation:\n";
                m_population->Print();
                cout << endl;
            }///////////////////////////////////////////////////////////////////////////////////////////////////////////
        }
    }
    
    m_population->CalculateFitnessForIndividuals();
    
    timer.Pause();
    
    cout << "Program grouped the objects in " << timer.GetTime() << " s\n";
    
    m_population->SendBestResultToFile();
}


Population* GeneticClustering::CreateInitialPopulation() {
    Population* initialPopulation = new Population( &m_objects );
    
    if ( Utility::debug ) {/////////////////////////////////////////////////////////////////////////////////////////////
        initialPopulation->Print();
    }///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    return initialPopulation;
}


void GeneticClustering::GetObjects() {
    ifstream in("objects.txt");
    double x, y;
    
    while ( in >> x >> y ) {
        Object* object = new Object( x, y );
        m_objects.push_back( object );
    }
    
    if ( Utility::debug ) {/////////////////////////////////////////////////////////////////////////////////////////////
        cout << "Number of created objects: " << m_objects.size() << endl;
        for ( vector<Object*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it ) {
            (*it)->Print();
        }
    }///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
