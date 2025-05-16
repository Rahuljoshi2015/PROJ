#include "DefaultStrategy.h"
#include <cmath>

DefaultStrategy::DefaultStrategy() {
    // Constructor (can be used for initialization if needed)
}

int DefaultStrategy::trouverPlace(const QVector<long>& tempPlace, const QVector<bool>& placeOccupe, int nbrPlaces) 
    {
    // Calculate the number of columns based on the number of places
    int numColumns = static_cast<int>(std::ceil(std::sqrt(nbrPlaces)));
    // Calculate the number of rows based on the number of places
    int numRows = static_cast<int>(std::ceil(static_cast<double>(nbrPlaces) / numColumns));

    // Iterate through columns and rows to find the first available place
    for (int col = 0; col < numColumns; col++) {
        for (int row = 0; row < numRows; row++) {
            int place = col + row * numColumns;

            // Check if the place is within the bounds and not occupied
            if (place < nbrPlaces && !placeOccupe[place]) {
                return place;  // Return the first available place
            }
        }
    }
    return -1;  // Return -1 if no available place is found
}
