#ifndef DRONEHEALTH_H
#define DRONEHEALTH_H

#include "objectfeature.h"

class DroneHealth : public ObjectFeature
{
    Q_OBJECT
public:
    DroneHealth(int initial_health = 100, QObject* parent = nullptr);
    void update() override;
signals:
    void death();
private:
    int m_health;
    qreal m_previous_velocity_x;
    qreal m_previous_velocity_y;
};

#endif // DRONEHEALTH_H
