#include "Individual.hpp"
#include "Object.hpp"
#include "Utility.hpp"
#include "Vector.hpp"
#include <iostream>


bool Individual::AreSame(const Individual& a, const Individual& b) {
    for ( unsigned u = 0; u < a.m_size; ++u ) {
        if ( a.m_proposedClusters[u] != b.m_proposedClusters[u] ) {
            return false;
        }
    }
    
    return true;
}


Individual::Individual( const vector<Object*>* objects ) : m_fitness( Utility::infinity ) {
    m_size = objects->size();
    
    m_points = new Object*[m_size];
    m_proposedClusters = new int[m_size];
    
    // Zapamiętanie obiektów w tablicy m_points znajdującej się wewnątrz klasy.
    unsigned u = 0;
    for ( vector<Object*>::const_iterator it = objects->begin(); it != objects->end(); ++it, ++u ) {
        m_points[u] = *it;
    }
    
    // Losowe przyporządkowanie.
    do {
        for ( unsigned u = 0; u < m_size; ++u ) {
            m_proposedClusters[u] = Utility::RandBetween( 0, ( Utility::k - 1 ) );
        }
    } while ( !IsCorrect() );
    
    m_respectiveCenters = new Vector*[Utility::k];
    
    for ( int i = 0; i < Utility::k; ++i ) {
        m_respectiveCenters[i] = new Vector( 0.0d, 0.0d );
    }
    
    m_id = m_individualsNumber;
    m_individualsNumber++;
}


Individual::~Individual() {
    delete[] m_points;
    delete[] m_proposedClusters;
    
    for ( int i = 0; i < Utility::k; ++i ) {
        delete m_respectiveCenters[i];
    }
    delete[] m_respectiveCenters;
    
    m_individualsNumber--;
}


double Individual::CheckFitness() {
    // Aby sprawdzić jakość osobnika, trzeba najpierw znaleźć środki dla każdego z klastrów.
    FindCenters();
    
    // Sumujemy odległości wszystkich elementów do środków odpowiadających im klastrów.
    
    double sum = 0.0d;
    
    for ( unsigned u = 0; u < m_size; ++u ) {
        sum += Vector::Distance( m_points[u]->Location(), *m_respectiveCenters[m_proposedClusters[u]] );
    }
    
    m_fitness = sum;
    
    return sum;
}


bool Individual::IsCorrect() const {
    return Utility::IsAssignationCorrect( m_proposedClusters, m_size );
}


void Individual::Print() const {
    cout << "individual " << m_id << ": [";
    for ( unsigned u = 0; u < m_size; ++u ) {
        cout << m_proposedClusters[u];
    }
    cout << "]\n";
}


unsigned Individual::m_individualsNumber = 0;


void Individual::FindCenters() {
    // Tablica trzymająca ilość elementów w klastrach.
    unsigned objectsInsideClustersNumber[Utility::k];
    
    // Wyzerowanie środków i powyższej tablicy.
    for ( int i = 0; i < Utility::k; ++i ) {
        m_respectiveCenters[i]->x = 0.0d;
        m_respectiveCenters[i]->y = 0.0d;
        
        objectsInsideClustersNumber[i] = 0;
    }
    
    // Dodawanie wartości elementów do odpowiadających im środków.
    for ( unsigned u = 0; u < m_size; ++u ) {
        m_respectiveCenters[m_proposedClusters[u]]->x += m_points[u]->Location().x;
        m_respectiveCenters[m_proposedClusters[u]]->y += m_points[u]->Location().y;
        
        ++objectsInsideClustersNumber[m_proposedClusters[u]];
    }
    
    // Wyliczenie średniej.
    for ( int i = 0; i < Utility::k; ++i ) {
        m_respectiveCenters[i]->x /= objectsInsideClustersNumber[i];
        m_respectiveCenters[i]->y /= objectsInsideClustersNumber[i];
    }
    
    if ( Utility::debug ) {/////////////////////////////////////////////////////////////////////////////////////////////
        cout << "Centers for individual " << m_id << ":\n";
        
        for ( int i = 0; i < Utility::k; ++i ) {
            cout << "    center " << i << ": x=" << m_respectiveCenters[i]->x << " y="
                 << m_respectiveCenters[i]->y << endl;
        }
    }///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
