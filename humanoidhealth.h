#ifndef HUMANOIDHEALTH_H
#define HUMANOIDHEALTH_H

#include "objectfeature.h"

class HumanoidHealth : public ObjectFeature
{
    Q_OBJECT
public:
    HumanoidHealth(int initial_health = 100, QObject* parent = nullptr);
    void update() override;
signals:
    void death();
private:
    int m_health;
    qreal m_previous_velocity_x;
    qreal m_previous_velocity_y;
};

#endif // HUMANOIDHEALTH_H
