#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "objectfeature.h"
#include <QMutex>

class Controller : public ObjectFeature
{
public:
    Controller(QObject *parent = nullptr);
    void update();
public slots:
    void setCommands(int movement, int actions, int modifiers);
    void setMovement(qreal dx, qreal dy);
    void disable();
    void enable();
private:
    qreal m_dx, m_dy;
    bool m_movement_vector_given;
    int m_movement;
    int m_actions;
    int m_modifiers;
    bool m_disabled;
};

#endif // CONTROLLER_H
