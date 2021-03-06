#ifndef DRONEPHYSICS_H
#define DRONEPHYSICS_H

#include "objectfeature.h"

class DronePhysics : public ObjectFeature
{
    Q_OBJECT
public:
    DronePhysics(qreal relative_mass = .50, qreal relative_speed = .50, qreal relative_acceleration = .50, int custom_attribute = -1, QObject *parent = nullptr);
    void update();

protected:
    void handleInternalsChange();

private:
    qreal m_mass;
    qreal m_speed; // tells how fast the object is in relation to the game world
    qreal m_acceleration;
    qreal m_velocity_x; // the actual velocity of the object at a given moment
    qreal m_velocity_y; // the actual velocity of the object at a given moment
    qreal m_velocity_for_next_cycle_x;
    qreal m_velocity_for_next_cycle_y;
    int m_state; // on a surface, free falling, etc
    bool m_ballistic;
};

#endif // DRONEPHYSICS_H
