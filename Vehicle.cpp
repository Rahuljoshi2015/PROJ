#include <QDebug>
#include "vehicle.h"
#include "Parking.h"
#include <QRandomGenerator>
#include <QThread>

vehicle::vehicle(QString nom, Parking *parking, QObject *parent)
    : QThread(parent), nom(nom), parking(parking), place(-1) {}

void vehicle::run() {
    try {
        parking->stationner(this);
        int stayDuration = QRandomGenerator::global()->bounded(5, 50);
        QThread::sleep(stayDuration);
  // Simulate the time the car stays parked.
        parking->sortir(this);
    } catch (const std::exception &e) {
        qDebug() << e.what();
    }
}

QString vehicle::getNom() const {
    return nom;
}

void vehicle::setPlace(int place) {
    this->place = place;
}

int vehicle::getPlace() const {
    return place;
}
