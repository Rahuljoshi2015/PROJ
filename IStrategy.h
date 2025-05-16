#pragma once

#include <QVector>

class IStrategy {
public:
    virtual ~IStrategy() = default;

    // Pure virtual method to find a parking spot
    virtual int trouverPlace(const QVector<long>& tempPlace,
                             const QVector<bool>& placeOccupe,
                             int nbrPlaces) = 0;
};
