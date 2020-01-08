#ifndef CLIMBABLEPHYSICS_H
#define CLIMBABLEPHYSICS_H

#include "objectfeature.h"

class ClimbablePhysics : public ObjectFeature
{
    Q_OBJECT
public:
    ClimbablePhysics(qreal relative_mass = .50, QObject *parent = nullptr);
    void update();

protected:
    void handleInternalsChange();

private:
    qreal m_mass;
    qreal m_velocity_x; // the actual velocity of the object at a given moment
    qreal m_velocity_y; // the actual velocity of the object at a given moment
    qreal m_velocity_for_next_cycle_x;
    qreal m_velocity_for_next_cycle_y;
    qreal m_attachment_strength;
    int m_state; // on a surface, free falling, etc
};

#endif // CLIMBABLEPHYSICS_H
