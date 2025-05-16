#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QVector>
#include "Parking.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setRectangleColor(int col, int row, QColor color);
    void setRectangleText(int col, int row, const QString &text);

private:
    void setupUI();
    void setupParking(int nbrPlaces);
    void startSimulation();

    QGraphicsScene *scene;
    QVector<QGraphicsRectItem*> parkingSpots;
    QVector<QGraphicsTextItem*> spotLabels;
    Parking *parking;
    int numPlaces;
    int numCars;
    int syncStrategy;
};

#endif // MAINWINDOW_H
