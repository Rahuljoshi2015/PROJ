#ifndef DEFAULTSTRATEGY_H
#define DEFAULTSTRATEGY_H

#include "IStrategy.h"
#include <vector>

class DefaultStrategy : public IStrategy {
public:
    DefaultStrategy();
   
    int trouverPlace(const QVector<long>& tempPlace, const QVector<bool>& placeOccupe, int nbrPlaces) override;
};

#endif // DEFAULTSTRATEGY_H
