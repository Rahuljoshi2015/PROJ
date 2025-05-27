#include "Parking.h"
#include "vehicle.h"
#include "IStrategy.h"
#include "MainWindow.h"

#include <QElapsedTimer>
#include <QFile>
#include <QTextStream>
#include <QThread>
#include <QMetaObject>
#include <QtMath>
#include <QDebug>
#include <QRandomGenerator>

Parking::Parking(int nbrPlaces, IStrategy *strategy, MainWindow *gui, int syncStrategy)
    : QObject(nullptr),
      tempPlace(),
      placeOccupe(),
      placesSemaphore(nbrPlaces),
      strategy(strategy),
      nbrPlaces(nbrPlaces),
      syncStrategy(syncStrategy),
      gui(gui)
{
    tempPlace.resize(nbrPlaces);
    placeOccupe.resize(nbrPlaces);
    for (int i = 0; i < nbrPlaces; i++) {
        tempPlace[i] = QRandomGenerator::global()->bounded(50, 60); // 5-25 seconds
        placeOccupe[i] = false;
    }
}

void Parking::stationner(vehicle *vehicle) {
    QElapsedTimer timer;
    timer.start();
    qDebug() << "Car" << vehicle->getNom() << "is trying to park";

    if (syncStrategy == 1) {
        placesSemaphore.acquire();
    } else if (syncStrategy == 2) {
        placesMutex.lock();
    }

    int place = strategy->trouverPlace(tempPlace, placeOccupe, nbrPlaces);

    if (place == -1) {
        qDebug() << "No parking spot available for car" << vehicle->getNom();
        if (syncStrategy == 1) {
            placesSemaphore.release();
        } else if (syncStrategy == 2) {
            placesMutex.unlock();
        }
        return;
    }

    placeOccupe[place] = true;

    int numColumns = qCeil(qSqrt(nbrPlaces));
    int col = place % numColumns;
    int row = place / numColumns;

    QMetaObject::invokeMethod(gui, [=]() {
        gui->setRectangleColor(col, row, Qt::yellow);
        gui->setRectangleText(col, row, vehicle->getNom());
    }, Qt::QueuedConnection);

    QThread::msleep(3000);

    QMetaObject::invokeMethod(gui, [=]() {
        gui->setRectangleColor(col, row, Qt::red);
    }, Qt::QueuedConnection);

    qDebug() << "Car" << vehicle->getNom() << "parked at spot" << place;
    vehicle->setPlace(place);

    if (syncStrategy == 2) {
        placesMutex.unlock();
    }

    long parkingWaitTime = timer.elapsed();
    qDebug() << "Wait time for car" << vehicle->getNom() << ":" << parkingWaitTime << "milliseconds";
    QMetaObject::invokeMethod(gui, [=]() {
        gui->totalWaitTime += parkingWaitTime;
        gui->updateStatus();
    }, Qt::QueuedConnection);
}

void Parking::sortir(vehicle *vehicle) {
    qDebug() << "Car" << vehicle->getNom() << "is trying to exit";

    int place = vehicle->getPlace();
    if (place < 0 || place >= nbrPlaces) {
        qDebug() << "Car" << vehicle->getNom() << "has no valid parking spot, exit ignored.";
        return;
    }

    if (syncStrategy == 2) {
        placesMutex.lock();
    }

    QThread::sleep(tempPlace[place] / 1000);
    placeOccupe[place] = false;

    int numColumns = qCeil(qSqrt(nbrPlaces));
    int col = place % numColumns;
    int row = place / numColumns;

    QMetaObject::invokeMethod(gui, [=]() {
        gui->setRectangleColor(col, row, Qt::green);
        gui->setRectangleText(col, row, "");
    }, Qt::QueuedConnection);

    QThread::msleep(3000);

    qDebug() << "Car" << vehicle->getNom() << "has exited!";

    if (syncStrategy == 1) {
        placesSemaphore.release();
    } else if (syncStrategy == 2) {
        placesMutex.unlock();
    }
}

QString Parking::analyzeLog(const QString &logPath) {
    QFile file(logPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "Failed to open log file.";
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    return content;
}