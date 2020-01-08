#include "humanoidhealth.h"
#include "attributesandstates.h"

HumanoidHealth::HumanoidHealth(int initial_health, QObject* parent) : ObjectFeature(parent)
{
    m_health = initial_health;
    if(initial_health > 100)
        initial_health = 100;
    if(initial_health <= 0)
        initial_health = 1;

    m_previous_velocity_x = 0.0;
    m_previous_velocity_y = 0.0;
}

void HumanoidHealth::update()
{
    for(int i = 0; i < m_databank->colliding_objects.length(); i++){
        if(checkAttribute(m_databank->colliding_objects.at(i)->attributes(), PIERCING)){
            m_health -= 25;
            if(m_health <= 0)
                emit death();
        }
        if(checkAttribute(m_databank->colliding_objects.at(i)->attributes(), HEALING)){
            m_health += 25;
            if(m_health >= 100)
                m_health = 100;
        }
    }
    if(abs(m_databank->physics_packet.at(VELOCITY_X) - m_previous_velocity_x) > 0.5)
        m_health -= 25;

    if(abs(m_databank->physics_packet.at(VELOCITY_Y) - m_previous_velocity_y) > 0.5)
        m_health -= 25;

    if(m_health <= 0)
        emit death();

    m_previous_velocity_x = m_databank->physics_packet.at(VELOCITY_X);
    m_previous_velocity_y = m_databank->physics_packet.at(VELOCITY_Y);

    m_databank->health = m_health;
}
