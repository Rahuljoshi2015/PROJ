#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QLabel>
#include <QPushButton>
#include <QMutex>
#include "Parking.h"
#include "CustomRectangle.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setRectangleColor(int col, int row, QColor color);
    void setRectangleText(int col, int row, const QString &text);
    void updateStatus();

    double totalWaitTime;

private slots:
    void toggleSimulation();
    void resetSimulation();
    void addCar();

private:
    void setupUI();
    void setupParking(int nbrPlaces);
    void startSimulation();

    QGraphicsScene *scene;
    QVector<CustomRectangle*> parkingSpots;
    QVector<vehicle*> activeCars; // Track active cars
    QMutex spotsMutex; // Protect parkingSpots access
    Parking *parking;
    int numPlaces;
    int numCars;
    int syncStrategy;
    bool isPaused;
    QLabel *statusLabel;
    QPushButton *toggleButton;
    QPushButton *resetButton;
    QPushButton *addCarButton;
    int occupiedSpots;
    int totalCarsParked;
};

#endif // MAINWINDOW_H