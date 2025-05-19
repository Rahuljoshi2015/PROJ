#include "MainWindow.h"
#include "vehicle.h"
#include "DefaultStrategy.h"

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QTimer>
#include <QDebug>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), scene(new QGraphicsScene(this)), parking(nullptr) {
    
    // Ask user for number of cars
    bool ok;
    numCars = QInputDialog::getInt(this, "Number of Cars", "Enter the number of cars:", 2, 1, 100, 1, &ok);
    if (!ok) numCars = 2;

    // Ask user for number of parking places
    numPlaces = QInputDialog::getInt(this, "Number of Parking Spots", "Enter the number of parking spots:", 16, 1, 100, 1, &ok);
    if (!ok) numPlaces = 16;

    // Ask user for sync strategy (1 = Semaphore, 2 = Mutex)
    syncStrategy = QInputDialog::getInt(this, "Strategy", "1 = Semaphore, 2 = Mutex", 1, 1, 2, 1, &ok);
    if (!ok) syncStrategy = 1;

    setupUI();
    setupParking(numPlaces);
}

MainWindow::~MainWindow() {
    delete scene;
}

void MainWindow::setupUI() {
    QGraphicsView *view = new QGraphicsView(scene, this);
    setCentralWidget(view);
    resize(600, 600);
}

void MainWindow::setupParking(int nbrPlaces) {
    int numColumns = qCeil(qSqrt(nbrPlaces));
    int numRows = qCeil((double)nbrPlaces / numColumns);

    for (int i = 0; i < nbrPlaces; i++) {
        int col = i % numColumns;
        int row = i / numColumns;

        QGraphicsRectItem *rectItem = scene->addRect(col * 60, row * 60, 50, 50, QPen(Qt::black), QBrush(Qt::lightGray));
        parkingSpots.push_back(rectItem);

        QGraphicsTextItem *textItem = scene->addText("");
        textItem->setPos(col * 60 + 15, row * 60 + 15);
        spotLabels.push_back(textItem);
    }

    // Initialize the parking logic.
    parking = new Parking(nbrPlaces, new DefaultStrategy(), this, syncStrategy);

    // Add all cars with delay
    for (int i = 0; i < numCars; ++i) {
        QTimer::singleShot(1000 + i * 500, this, [=]() {
            vehicle *car = new vehicle(QString("Car %1").arg(i + 1), parking);
            car->start();
        });
    }
}

void MainWindow::setRectangleColor(int col, int row, QColor color) {
    int index = row * qCeil(qSqrt(numPlaces)) + col;
    if (index >= 0 && index < parkingSpots.size()) {
        parkingSpots[index]->setBrush(color);
    }
}

void MainWindow::setRectangleText(int col, int row, const QString &text) {
    int index = row * qCeil(qSqrt(numPlaces)) + col;
    if (index >= 0 && index < spotLabels.size()) {
        spotLabels[index]->setPlainText(text);
    }
}
