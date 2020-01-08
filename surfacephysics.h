#ifndef SURFACEPHYSICS_H
#define SURFACEPHYSICS_H

#include "objectfeature.h"

class SurfacePhysics : public ObjectFeature
{
    Q_OBJECT
public:
    SurfacePhysics(qreal relative_friction = .50, qreal relative_attachment_strength = .50, qreal relative_mass = .50, QObject *parent = nullptr);
    void update();

protected:
    void handleInternalsChange();

private:
    qreal m_mass;
    qreal m_velocity_x; // the actual velocity of the object at a given moment
    qreal m_velocity_y; // the actual velocity of the object at a given moment
    qreal m_velocity_for_next_cycle_x;
    qreal m_velocity_for_next_cycle_y;
    qreal m_friction;
    qreal m_attachment_strength;
    int m_state; // on a surface, free falling, etc
};

#endif // SURFACEPHYSICS_H
