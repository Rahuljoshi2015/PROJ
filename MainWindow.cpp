#include "MainWindow.h"
#include "vehicle.h"
#include "DefaultStrategy.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsView>
#include <QTimer>
#include <QDebug>
#include <QInputDialog>
#include <QDockWidget>
#include <QRandomGenerator>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), scene(new QGraphicsScene(this)), parking(nullptr),
      isPaused(false), occupiedSpots(0), totalWaitTime(0), totalCarsParked(0) {
    
    bool ok;
    numCars = QInputDialog::getInt(this, "Number of Cars", "Enter the number of cars:", 10, 1, 100, 1, &ok);
    if (!ok) numCars = 10;

    numPlaces = QInputDialog::getInt(this, "Number of Parking Spots", "Enter the number of parking spots:", 100, 1, 100, 1, &ok);
    if (!ok) numPlaces = 100;

    syncStrategy = QInputDialog::getInt(this, "Strategy", "1 = Semaphore, 2 = Mutex", 1, 1, 2, 1, &ok);
    if (!ok) syncStrategy = 1;

    setupUI();
    setupParking(numPlaces);
}

MainWindow::~MainWindow() {
    for (vehicle *car : activeCars) {
        if (car) {
            car->terminate();
            car->wait();
            delete car;
        }
    }
    delete scene;
    delete parking;
}

void MainWindow::setupUI() {
    QGraphicsView *view = new QGraphicsView(scene, this);
    view->setMinimumSize(600, 600);
    view->setRenderHint(QPainter::Antialiasing);
    view->setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);

    QDockWidget *dock = new QDockWidget("Control Panel", this);
    QWidget *controlWidget = new QWidget;
    QVBoxLayout *controlLayout = new QVBoxLayout;

    statusLabel = new QLabel("Occupied Spots: 0\nAverage Wait Time: 0 ms");
    toggleButton = new QPushButton("Pause");
    resetButton = new QPushButton("Reset");
    addCarButton = new QPushButton("Add Car");

    controlLayout->addWidget(statusLabel);
    controlLayout->addWidget(toggleButton);
    controlLayout->addWidget(resetButton);
    controlLayout->addWidget(addCarButton);
    controlLayout->addStretch();

    controlWidget->setLayout(controlLayout);
    dock->setWidget(controlWidget);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    connect(toggleButton, &QPushButton::clicked, this, &MainWindow::toggleSimulation);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetSimulation);
    connect(addCarButton, &QPushButton::clicked, this, &MainWindow::addCar);

    setCentralWidget(view);
    resize(800, 600);
}

void MainWindow::setupParking(int nbrPlaces) {
    int numColumns = qCeil(qSqrt(nbrPlaces));
    int numRows = qCeil((double)nbrPlaces / numColumns);

    scene->setSceneRect(0, 0, numColumns * 60 + 20, numRows * 60 + 20);

    QMutexLocker locker(&spotsMutex);
    parkingSpots.clear();
    for (int i = 0; i < nbrPlaces; i++) {
        int col = i % numColumns;
        int row = i / numColumns;

        CustomRectangle *spot = new CustomRectangle();
        spot->setPos(col * 60 + 10, row * 60 + 10);
        spot->setColor(Qt::green);
        scene->addItem(spot);
        parkingSpots.push_back(spot);
    }

    parking = new Parking(nbrPlaces, new DefaultStrategy(), this, syncStrategy);
    startSimulation();
}

void MainWindow::startSimulation() {
    for (int i = 0; i < numCars; ++i) {
        int randomDelay = QRandomGenerator::global()->bounded(5000, 60000); // 5-25 seconds
        QTimer::singleShot(randomDelay, this, [=]() {
            if (!isPaused) {
                vehicle *car = new vehicle(QString("Car %1").arg(i + 1), parking);
                QMutexLocker locker(&spotsMutex);
                activeCars.push_back(car);
                car->start();
            }
        });
    }
}

void MainWindow::setRectangleColor(int col, int row, QColor color) {
    int index = row * qCeil(qSqrt(numPlaces)) + col;
    QMutexLocker locker(&spotsMutex);
    if (index >= 0 && index < parkingSpots.size()) {
        qDebug() << "Setting color for spot at col:" << col << "row:" << row << "to color:" << color.name();
        parkingSpots[index]->setColor(color);
        scene->update();
        if (color == Qt::red) {
            occupiedSpots++;
            totalCarsParked++;
        } else if (color == Qt::green) {
            occupiedSpots--;
        }
        updateStatus();
    } else {
        qDebug() << "Invalid spot index:" << index << "for col:" << col << "row:" << row;
    }
}

void MainWindow::setRectangleText(int col, int row, const QString &text) {
    int index = row * qCeil(qSqrt(numPlaces)) + col;
    QMutexLocker locker(&spotsMutex);
    if (index >= 0 && index < parkingSpots.size()) {
        qDebug() << "Setting text for spot at col:" << col << "row:" << row << "to:" << text;
        parkingSpots[index]->setText(text);
        scene->update();
    }
}

void MainWindow::updateStatus() {
    double avgWaitTime = totalCarsParked > 0 ? totalWaitTime / totalCarsParked : 0;
    statusLabel->setText(QString("Occupied Spots: %1\nAverage Wait Time: %2 ms")
                         .arg(occupiedSpots)
                         .arg(avgWaitTime, 0, 'f', 1));
}

void MainWindow::toggleSimulation() {
    isPaused = !isPaused;
    toggleButton->setText(isPaused ? "Resume" : "Pause");
    if (!isPaused) {
        startSimulation();
    }
}

void MainWindow::resetSimulation() {
    isPaused = true;
    toggleButton->setText("Resume");

    QMutexLocker locker(&spotsMutex);
    for (vehicle *car : activeCars) {
        if (car) {
            car->terminate();
            car->wait();
            delete car;
        }
    }
    activeCars.clear();

    occupiedSpots = 0;
    totalWaitTime = 0;
    totalCarsParked = 0;
    updateStatus();

    for (auto spot : parkingSpots) {
        spot->setColor(Qt::green);
        spot->setText("");
    }
    scene->update();

    delete parking;
    parking = new Parking(numPlaces, new DefaultStrategy(), this, syncStrategy);
}

void MainWindow::addCar() {
    if (!isPaused) {
        vehicle *car = new vehicle(QString("Car %1").arg(++numCars), parking);
        QMutexLocker locker(&spotsMutex);
        activeCars.push_back(car);
        car->start();
    }
}