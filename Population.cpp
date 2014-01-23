#include "Individual.hpp"
#include "Object.hpp"
#include "Population.hpp"
#include "Utility.hpp"
#include <iostream>
#include <fstream>


Population::Population( const vector<Object*>* objects ) : m_worstFitness( Utility::infinity ) {
    // Tworzenie populacji.
    
    m_individuals = new Individual*[Utility::populationSize];
    
    bool isUnique;
    
    for ( int i = 0; i < Utility::populationSize; ++i ) {
        m_individuals[i] = new Individual( objects );
        
        // Staramy się utworzyć różnych od siebie osobników.
        for ( int j = 0; j < Utility::uniqueInitialPopulationTries; ++j ) {
            isUnique = true;
            
            // Czy stworzony osobnik jest wyjątkowy? Porównujemy go z utworzonymi poprzednio.
            for ( int k = 0; k < i; ++k ) {
                if ( Individual::AreSame( *m_individuals[i], *m_individuals[k] ) ) {
                    isUnique = false;
                    break;
                }
            }
            
            // Tak, to przerywamy pętlę.
            if ( isUnique ) {
                break;
            }
            
            // Nie, usuwamy stworzonego osobnika i próbujemy z następnym.
            delete m_individuals[i];
            m_individuals[i] = new Individual( objects );
        }
    }
    
    // Zapamiętanie rozmiaru pojedynczego osobnika.
    m_individualSize = objects->size();
}


Population::~Population() {
    // Usunięcie wszystkich osobników z pamięci.
    for ( int i = 0; i < Utility::populationSize; ++i ) {
        delete m_individuals[i];
    }
    delete[] m_individuals;
}


void Population::CalculateFitnessForIndividuals() {
    m_bestFitness = Utility::infinity;  // Wstępnie nieskończoność, czyli najgorsza jakość.
    m_worstFitness = 0.0d;  // Wstępnie 0, czyli najlepsza jakość.
    
    double fitness;
    
    for ( int i = 0; i < Utility::populationSize; ++i ) {
        fitness = m_individuals[i]->CheckFitness();
        
        if ( fitness > m_worstFitness ) {
            m_worstFitness = fitness;
        }
        
        if ( fitness < m_bestFitness ) {
            m_bestFitness = fitness;
        }
        
        if ( Utility::debug ) {/////////////////////////////////////////////////////////////////////////////////////////
            cout << "        Fitness for individual " << i << ": " << fitness << endl;
        }///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    
    if (Utility::debug) {///////////////////////////////////////////////////////////////////////////////////////////////
        cout << "Worst fitness: " << m_worstFitness << "\nBest fitness: " << m_bestFitness << endl;
    }///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}


void Population::SelectPairsAndCrossThem() {
    //------------------------------------------------------------------------------------------------------------------
    // Wybór par. 
    //------------------------------------------------------------------------------------------------------------------
    
    int pairsNumber = ( Utility::populationSize / 2 );
    
    // Tablica ze wskażnikami do wybranych osobników.
    Individual* pairs[pairsNumber][2];
    
    bool isUniquePair;  // Czy wybrana para nie została wybrana już wcześniej.
    
    for ( int i = 0; i < pairsNumber; ++i ) {
        isUniquePair = false;
        
        while ( !isUniquePair ) {
            // Losowy wybór i-tej pary.
            pairs[i][0] = FindParent();
            pairs[i][1] = FindParent( pairs[i][0] );
            
            // Sprawdzenie, czy para nie została już wybrana wcześniej. Jeśli tak, to losujemy kolejną.
            
            isUniquePair = true;
            
            for ( int j = 0; j < i; ++j ) {
                if ( ( ( pairs[j][0] == pairs[i][0] ) && ( pairs[j][1] == pairs[i][1] ) )
                     || ( ( pairs[j][0] == pairs[i][1] ) && ( pairs[j][1] == pairs[i][0] ) ) ) {
                    isUniquePair = false;
                    
                    break;  // Już wiemy, że dana para wystąpiła wcześniej.
                }
            }
        }
        
        if ( Utility::debug ) {/////////////////////////////////////////////////////////////////////////////////////////
            cout << "Selected " << ( i + 1 ) << " pair: individual " << pairs[i][0]->Identifier()
                 << " and individual " << pairs[i][1]->Identifier() << endl;
        }///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    
    //------------------------------------------------------------------------------------------------------------------
    // Skrzyżowanie wybranych par.
    //------------------------------------------------------------------------------------------------------------------
    
    // Tablica z nowymi przyporządkowaniami dla wszystkich osobników.
    int newAssignations[Utility::populationSize][m_individualSize];
    
    // Wykonujemy krzyżowanie każdej z par. Wyniki zapisujemy w wyżej utworzonej tablicy.
    for ( int i = 0; i < pairsNumber; ++i ) {
        Crossover( pairs[i][0], pairs[i][1], newAssignations[i * 2], newAssignations[( i * 2 ) + 1] );
    }
    
    // Podmieniamy stare przyporządkowania na nowe w całej populacji.
    for ( int i = 0; i < pairsNumber; ++i ) {
        for ( unsigned u = 0; u < m_individualSize; ++u ) {
            m_individuals[i * 2]->AssignedClusters()[u] = newAssignations[i * 2][u];
            m_individuals[( i * 2 ) + 1]->AssignedClusters()[u] = newAssignations[( i * 2 ) + 1][u];
        }
    }
}


void Population::Mutate() {
    // Przechodzimy kolejne elementy w każdym z osobników.
    // Każdy element ma szansę doznania mutacji z prawdopodobieństwem równym Utility::mutationProbability.
    for ( int i = 0; i < Utility::populationSize; ++i ) {
        for ( unsigned u = 0; u < m_individualSize; ++u ) {
            if ( Utility::RandSelect() < Utility::mutationProbability ) {
                m_individuals[i]->AssignedClusters()[u] = Utility::RandBetween( 0, ( Utility::k - 1 ) );
                
                if ( Utility::debug ) {/////////////////////////////////////////////////////////////////////////////////
                    cout << "Object " << u << " inside individual " << i << " mutaded to "
                         << m_individuals[i]->AssignedClusters()[u] << "!\n";
                }///////////////////////////////////////////////////////////////////////////////////////////////////////
            }
        }
    }
}


void Population::SendBestResultToFile() {
    ofstream outClusteredObjects("clustered_objects.txt");
    ofstream outCenters("centers.txt");
    
    // Znalezienie najlepszego osobnika.
    // Jeżeli występuje więcej osobników o tej samej jakości, zostanie wybrany pierwszy wykryty.
    
    Individual* bestIndividual = m_individuals[0];
    
    for ( int i = 1; i < Utility::populationSize; ++i ) {
        if ( m_individuals[i]->Quality() < bestIndividual->Quality()) {
            bestIndividual = m_individuals[i];
        }
    }
    
    if ( Utility::debug ) {/////////////////////////////////////////////////////////////////////////////////////////////
        cout << "Best individual: " << bestIndividual->Identifier() << endl;
    }///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    cout << "Fitness: " << bestIndividual->Quality() << endl;
    
    // Wysłanie najlepszego wyniku do plików.
    
    for ( int unsigned u = 0; u < m_individualSize; ++u ) {
        outClusteredObjects << bestIndividual->AssignedClusters()[u] << " "
                            << bestIndividual->Objects()[u]->Location().x << " "
                            << bestIndividual->Objects()[u]->Location().y << endl;
    }
    
    for ( int i = 0; i < Utility::k; ++i ) {
        outCenters << i << " " << bestIndividual->Centers()[i]->x << " "
                   << bestIndividual->Centers()[i]->y << endl;
    }
}


void Population::Print() const {
    for ( int i = 0; i < Utility::populationSize; ++i ) {
        m_individuals[i]->Print();
    }
}


Individual* Population::FindParent( const Individual* avoidedParent ) const {
    double delta = (m_worstFitness - m_bestFitness);
    double quality;
    
    while ( true ) {  // Szukamy rodzica do skutku.
        // Losujemy kandydata na rodzica...
        int parentIndex = Utility::RandBetween( 0, ( Utility::populationSize - 1 ) );
        
        // ...i testujemy go.
        if ( m_individuals[parentIndex] != avoidedParent ) {  // Sprawdzamy, czy osobnik jest różny od unikanego.
        
            // Następnie biorąc pod uwagę jego jakość, dajemy mu szansę zostania rodzicem.
            
            quality = m_individuals[parentIndex]->Quality();
            
            // Jeżeli najgorsza i najlepsza jakość są identyczne, to osobniki mają taką samą szansę na zostanie
            // rodzicami. Nie jest zatem potrzebne dodatkowe losowanie. Zwracamy wstępnie wylosowanego osobnika.
            // Natomiast w przeciwnym wypadku dajemy szansę osobnikowi adekwatną do jego jakości.
            // RandSelect() produkuje wartości z przedziału (0;1). Na końcu dodajemy więc bardzo małą wartość,
            // aby dać też szansę najgorszym osobnikom.
            if ( ( m_worstFitness == m_bestFitness )
                 || ( Utility::RandSelect() < ( ( ( m_worstFitness - quality ) / delta ) + 0.05 ) ) ) {
                
                return m_individuals[parentIndex];
            }
        }
    }
}


void Population::Crossover( Individual* a, Individual* b, int* destinationA, int* destinationB ) {
    unsigned startPoint;
    bool isSuccess;
    
    for ( int i = 0; i < Utility::maxCrossoverTries; ++i ) {
        // Wylosowanie punktu startowego.
        startPoint = Utility::RandBetween( 1, ( m_individualSize - 1 ) );
        
        // Kopiujemy elementy do punktu startowego.
        for ( unsigned u = 0; u < startPoint; ++u ) {
            destinationA[u] = a->AssignedClusters()[u];
            destinationB[u] = b->AssignedClusters()[u];
        }
        
        // Od punktu startowego włącznie przypisujemy wartości odwrotnie.
        for ( unsigned u = startPoint; u < m_individualSize; ++u ) {
            destinationA[u] = b->AssignedClusters()[u];
            destinationB[u] = a->AssignedClusters()[u];
        }
        
        // Jeżeli potomstwo nie jest poprawne, to krzyżowanie nastąpi ponownie z nowo wylosowanym punktem startowym.
        if ( !( isSuccess = ( Utility::IsAssignationCorrect( destinationA, m_individualSize )
                              && Utility::IsAssignationCorrect( destinationB, m_individualSize ) ) ) ) {
            continue;
        }
        
        break;
    }
    
    // Może się zdarzyć, że zostanie wykonana maxCrossoverTries liczba prób krzyżowania
    // i wszystkie zakończą się porażką. Wtedy potomstwo będzie tożsame rodzicom.
    if ( !isSuccess ) {
        for ( unsigned u = 0; u < m_individualSize; ++u ) {
            destinationA[u] = a->AssignedClusters()[u];
            destinationB[u] = b->AssignedClusters()[u];
        }
    }
    
    if ( Utility::debug ) {/////////////////////////////////////////////////////////////////////////////////////////////
        cout << "Crossover for individuals " << a->Identifier() << " and " << b->Identifier()
             << " (start point = " << startPoint << "):\n    [";
        for ( unsigned u = 0; u < m_individualSize; ++u ) {
            cout << a->AssignedClusters()[u];
        }
        cout << "] -> [";
        for ( unsigned u = 0; u < m_individualSize; ++u ) {
            cout << destinationA[u];
        }
        cout << "]\n    [";
        for ( unsigned u = 0; u < m_individualSize; ++u ) {
            cout << b->AssignedClusters()[u];
        }
        cout << "] -> [";
        for ( unsigned u = 0; u < m_individualSize; ++u ) {
            cout << destinationB[u];
        }
        cout << "]\n";
    }///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
