#ifndef vehicle_H
#define vehicle_H

#include <QThread>
#include <QString>
class Parking; 

class vehicle : public QThread {
    Q_OBJECT

public:
    vehicle(QString nom, Parking *parking, QObject *parent = nullptr);
    void run() override;

    QString getNom() const;
    void setPlace(int place);
    int getPlace() const;

private:
    QString nom;
    Parking *parking;
    int place;
};

#endif // vehicle_H
