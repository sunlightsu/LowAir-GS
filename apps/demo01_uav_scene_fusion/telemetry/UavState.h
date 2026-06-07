#ifndef UAVSTATE_H
#define UAVSTATE_H

#include <QString>

struct UavState {
    QString uavId;
    qint64 timestampMs;
    QString frame;
    double x;
    double y;
    double z;
    double roll;
    double pitch;
    double yaw;
    double battery;
};

#endif // UAVSTATE_H
