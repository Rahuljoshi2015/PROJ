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
    for (int i = 0; i < nbrPlaces; i++) {
        tempPlace.append((i % 5 + 1) * 1000L);
        placeOccupe.append(false);
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
            placesSemaphore.release();  // Release if semaphore was used
        } else if (syncStrategy == 2) {
            placesMutex.unlock();       // Release mutex if locked
        }
        return; // Exit early to avoid crashing
    }

    placeOccupe[place] = true;

    int numColumns = qCeil(qSqrt(nbrPlaces));
    int col = place % numColumns;
    int row = place / numColumns;

    QMetaObject::invokeMethod(gui, [=]() {
        gui->setRectangleColor(col, row, Qt::red);
        gui->setRectangleText(col, row, vehicle->getNom());
    });

    qDebug() << "Car" << vehicle->getNom() << "parked at spot" << place;
    vehicle->setPlace(place);

    if (syncStrategy == 2) {
        placesMutex.unlock();
    }

    long parkingWaitTime = timer.elapsed();
    qDebug() << "Wait time for car" << vehicle->getNom() << ":" << parkingWaitTime << "milliseconds";
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

    // Step 1: Turn the spot green and remove label
    QMetaObject::invokeMethod(gui, [=]() {
        gui->setRectangleColor(col, row, Qt::green);
        gui->setRectangleText(col, row, "");
    });

    // Step 2: Artificial delay to show that it's free
    QThread::msleep(300);  // ← Add a small pause before releasing

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
