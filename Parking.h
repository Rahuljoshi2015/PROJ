#ifndef PARKING_H
#define PARKING_H

#include <QObject>
#include <QVector>
#include <QSemaphore>
#include <QMutex>

class vehicle;
class MainWindow;
class IStrategy;

class Parking : public QObject {
    Q_OBJECT

public:
    Parking(int nbrPlaces, IStrategy *strategy, MainWindow *gui, int syncStrategy);

    void stationner(vehicle *vehicle);
    void sortir(vehicle *vehicle);
    QString analyzeLog(const QString &logPath);

private:
    QVector<long> tempPlace;
    QVector<bool> placeOccupe;
    QSemaphore placesSemaphore;
    QMutex placesMutex;

    IStrategy *strategy;
    int nbrPlaces;
    int syncStrategy;
    MainWindow *gui;
};

#endif // PARKING_H
